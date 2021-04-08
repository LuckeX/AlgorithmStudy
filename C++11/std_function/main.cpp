#include <iostream>
#include <functional>
#include <memory>


using namespace std;

void printNum(int a)
{
    cout << a << endl;
}

class Foo
{
private:
    int a;

public:
    Foo(int a) { this->a = a; }
    void print_Foo(int b, int c) { cout << "Foo,a = " << a << ", b = " << b << ",c = " << c << endl; }
};

void example()
{
    std::function<void()> f1 = []() { cout << "hi,example" << endl; };
    f1();

    //声明并定义了一个返回值void，有一个int型参数的函数f2
    std::function<void(int)> f2 = printNum;
    f2(22);

    //使用站位符来表明需要传入参数
    std::function<void(int)> f3 = std::bind(printNum, std::placeholders::_1);
    f3(3333);
    //or
    std::function<void()> f4 = std::bind(printNum, 3333);
    f4();

    Foo foo(1);
    std::function<void(int)> f5 = std::bind(&Foo::print_Foo,&foo,placeholders::_1,3);//placeholders::_1表示在新函数调用时，要传入的参数
    f5(2);
    std::function<void(int)> f6 = std::bind(&Foo::print_Foo,&foo,2,placeholders::_1);
    f6(5);
}

int main()
{
    example();
    // std::shared_ptr<int>sh(new int(1));
    // std::shared_ptr<int>shcopy = sh;
    return 0;
}