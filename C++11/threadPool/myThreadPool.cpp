#include "myThreadPool.h"

using namespace xyk;

std::shared_ptr<ThreadPool> ThreadPool::threadPoolInstance = nullptr;
std::mutex ThreadPool::_mutex;

std::shared_ptr<ThreadPool> ThreadPool::create(int numThreads)
{
    if (nullptr == threadPoolInstance)
    {
        std::lock_guard<std::mutex> lock(_mutex);
        if (nullptr == threadPoolInstance)
        {
            threadPoolInstance = std::shared_ptr<ThreadPool>(new ThreadPool(numThreads));
        }
    }
    return threadPoolInstance;
}

void ThreadPool::destroy()
{
    std::lock_guard<std::mutex> lock(_mutex);
    if (threadPoolInstance)
    {
        threadPoolInstance->shutdown();
        threadPoolInstance.reset();
        threadPoolInstance = nullptr;
    }
}

void ThreadPool::shutdown()
{
    mShutdown = true;
    mConditionalJobs.notify_all();
    for (int i = 0; i < mNumThreads; i++)
    {
        if (mThreads[i].joinable())
        {
            mThreads[i].join();
        }
    }
}


inline uint8_t ThreadPool::getThreadCount()
{
    return mNumThreads;
}

void ThreadPool::threadManager()
{
    while (!mShutdown)
    {

        std::unique_lock<std::mutex> lock(mJobMutex);
        mConditionalJobs.wait(lock, [this] { return !mJobQueue.empty(); });

        //strange bug where it will continue even if the job queue is empty
        if (mJobQueue.size() < 1)
            continue;

        mJobQueue.front()->execute();

        mJobQueue.pop();
    }
}

ThreadPool::ThreadPool(uint8_t mNumThreads) : mNumThreads(mNumThreads), mShutdown(false)
{
    mThreads.reserve(mNumThreads);
    for (int i = 0; i != mNumThreads; ++i)
    {
        mThreads.emplace_back(std::thread(&ThreadPool::threadManager, this));
        mThreads.back().detach(); //thread run in backend, not block main thread
    }
}
