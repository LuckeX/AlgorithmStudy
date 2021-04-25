#include <iostream>
#include <random>
#include "myThreadPool.h"

std::mutex mtx;

// 添加两个数字的简单函数并打印结果
void multiply(const int a, const int b)
{
    // simulate_hard_computation();
    const int res = a * b;
    mtx.lock();
    std::cout << a << " * " << b << " = " << res << std::endl;
    mtx.unlock();
}

int main()
{
    std::shared_ptr<xyk::ThreadPool>pool = xyk::ThreadPool::create(4);

    for (int i = 1; i < 3; ++i)
    {
        for (int j = 1; j < 10; ++j)
        {
            pool->push(multiply, i, j);
        }
    }

    auto testFunc = [](int x) { return x * x; };

    auto returnValue = pool->push(testFunc, 5);

    std::cout << returnValue.get() << std::endl;

    pool->destroy();

    return 0;
}