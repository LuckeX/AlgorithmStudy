#include <iostream>
#include <thread>

using namespace std;

void func2();

void func1()
{
    cout<<"fun1"<<endl;
    func2();
}

void func2()
{
    cout<<"func2"<<endl;
    char *sp = NULL;
    sp[2] = 'b'; //crash
}

int main()
{
    std::thread td1 = std::thread(&func1);
    // func1();

    td1.join();

    return 0;
}