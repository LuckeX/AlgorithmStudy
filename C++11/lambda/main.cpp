#include <iostream>
#include <memory>

using namespace std;

/*
[capture list] (params list) mutable exception-> return type { function body }
1. capture list：捕获外部变量列表
2. params list：形参列表
3. mutable指示符：用来说用是否可以修改捕获的变量
4. exception：异常设定
5. return type：返回类型
6. function body：函数体
其中，(params list) mutable exception-> return type 这几个可以省略
*/
void captureByValue()
{
    int x = 88;
    //ok, 用mutable关键字可以改变捕获到的外部值变量，否则会报错。当然，lambda表达式按值捕获只是x的一个复制，并不会修改外部x的值
    auto f1 = [=]() mutable {x=33;cout<<x<<endl; };
    //error，值捕获的x是只读类型，在lambda表达式中不能修改
    //auto f2 = [=]() {x=22;cout<<x<<endl;}
    cout << x << endl;
}

void captureByReference()
{
    int x = 88;
    auto f1 = [&]() {x=33;cout<<x<<endl; };
    f1();
    cout << x << endl; //x=33
}

void example()
{
    int a = 10;
    auto f1 = [=](int para) -> int {
        cout << para << endl;
        return para + 1;
    }(a); //注意这个(a)，表示立即传入参数a，并表用这个函数

    auto f2 = [=] { cout << "a = " << a << endl; };
    f2();
}

void strange()
{
    // make_unique是c++14的特性
    auto myPi = std::make_unique<double>(3.1415);
    //奇怪的语义，可以直接搞个变量赋值
    auto f1 = [pi = std::move(myPi), b = 4]() { cout << *pi << " " << b << endl; };
    f1();
    // cout<<*myPi<<endl; //this will core dump.
}

int main()
{
    // captureByReference();
    // example();

    return 0;
}