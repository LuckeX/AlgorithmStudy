#include <iostream>

using namespace std;

void otherdef(int & t)
{
    cout<<"lvalue"<<endl;
}

void otherdef(int && t)
{
    cout<<"rvalue"<<endl;
}

void otherdef(const int & t)
{
    cout<<"all value"<<endl;
}

template <typename T>
void function(T&& t)
{
    cout<<"T&&"<<endl;
    otherdef(t);
}

template <typename T>
void perfect_forward(T&& t)
{
    otherdef(forward<T>(t));
    otherdef(t);
}

int main()
{
    // int t = 10;
    // function(t);
    int && num = 4;
    num = 10;
    // function(num);
    // perfect_forward(4);
    perfect_forward(num);
    perfect_forward(forward<int>(num));
    // function(4);
}