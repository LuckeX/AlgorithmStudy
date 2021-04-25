#pragma once

#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <future>
#include <memory>

namespace xyk
{
//portable way to null the copy and assignment operators
#define NULL_COPY_AND_ASSIGN(T)        \
    T(const T &other) { (void)other; } \
    void operator=(const T &other) { (void)other; }

    class Job
    {
    private:
        std::packaged_task<void()> func;

    public:
        virtual ~Job() {}
        virtual void execute() = 0;
    };

    template <typename RetType>
    class AnyJob : public Job
    {
    private:
        std::packaged_task<RetType()> func;

    public:
        AnyJob(std::packaged_task<RetType()> func) : func(std::move(func)) {}
        void execute()
        {
            func();
        }
    };

    class ThreadPool
    {
    public:
        static std::shared_ptr<ThreadPool> create(int numThreads);

        static void destroy();

        void shutdown();

        /* 模板函数的申明与定义必须放在一起。
           标准要求编译器在实例化模板时必须在上下文中可以查看到其定义实体；
           而反过来，在看到实例化模板之前，编译器对模板的定义体是不处理的——原因很简单，
           编译器怎么会预先知道typename 实参是什么呢？因此模板的实例化与定义体必须放到同一翻译单元中
        */
        template <typename Func, typename... Args>
        auto push(Func &&f, Args &&... args) -> std::future<decltype(f(args...))>
        {
            //get return type of the function
            typedef decltype(f(args...)) retType;

            //package the task
            std::packaged_task<retType()> task(std::move(std::bind(f, args...)));

            // lock mJobQueue mutex, add job to the job queue
            std::unique_lock<std::mutex> lock(mJobMutex);

            //get the future from the task before the task is moved into the mJobQueue
            std::future<retType> future = task.get_future();

            //place the job into the queue
            mJobQueue.emplace(std::make_shared<AnyJob<retType>>(std::move(task)));

            //notify a thread that there is a new job
            mConditionalJobs.notify_one();

            //return the future for the function so the user can get the return value
            return future;
        }

        inline uint8_t getThreadCount();

    private:
        static std::shared_ptr<ThreadPool> threadPoolInstance;
        static std::mutex _mutex;
        uint8_t mNumThreads; // number of threads in the mThreads
        std::vector<std::thread> mThreads;
        std::queue<std::shared_ptr<Job>> mJobQueue;
        std::condition_variable mConditionalJobs; // used to notify threads about available jobs
        std::mutex mJobMutex;                     // used to push/pop jobs to/from the queue
        bool mShutdown;

        void threadManager();
        ThreadPool() = delete; //prevent default constructor from being called
        ThreadPool(uint8_t mNumThreads);

        NULL_COPY_AND_ASSIGN(ThreadPool);
    }; /* end ThreadPool Class */

} //namespace xyk