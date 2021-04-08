### 完美转发
指的是函数模板可以将自己的参数“完美”地转发给内部调用的其它函数。所谓完美，即不仅能准确地转发参数的值，还能保证被转发参数的左、右值属性不变。   


```
template<typename T>
void function(T t) {
}
```
这不是一个完美转发，一方面，实参在传递给funtion的形参t的时候，需要进行一次拷贝操作；另一方面，无论t接受的是左值还是右值，在函数内部t都是有地址的（也即t是一个左值）。

```
template <typename T>
void function(const T& t) {
}
```
const T& t 即可接收左值，也可接收右值，但在函数内部，t还是一个左值。而且传递给otherdef函数的t要求形参t也是const类型。

```
template <typename T>
void function(T&& t) {
}
```
比较好的是这种形式，T&& t, 它可接收左值和右值。
- 当实参为左值或者左值引用（A&）时，函数模板中 T&& 将转变为 A&（A& && = A&）
- 当实参为右值或者右值引用（A&&）时，函数模板中 T&& 将转变为 A&&（A&& && = A&&）

> 在传递参数的时候，如果直接传递，会丢失它的左右值属性，
```
template <typename T>
void perfect_forward(T&& t)
{
}
int && num = 4;//num 虽然是一个右值引用，但实际上是一个左值，它有地址
perfect_forward(num);//这样传递的是一个左值
perfect_forward(forward<int>(num));//传递的右值
```
> 在c++中需要完美转发的，需要借助std::forward<类型>(参数)这样的形式，它会保持参数的左右值属性。