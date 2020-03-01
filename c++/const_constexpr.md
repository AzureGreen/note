# const 与 constexpr
1. 对于修饰变量来说：
	- `const`：**不区分**编译时和运行时，不论编译时或运行时都保证所修饰表达式不可被修改，说明修饰的是不可修改的变量。
	- `constexpr`：比const要求更严苛，限定修饰在**编译时**可以可知的表达式，说明修饰的就是编译时常量。
```c++
#include <iostream>
using namespace std;

int main()
{
    int a;
    cin >> a;
    const int b = a + 1;        // OK
    constexpr int c = a + 1;    // ERROR
    return 0;
}
```
如上代码，在编译时并不知道a的值，故`constexpr`修饰的c编译不过，而`const`修饰的b编译通过。

2. 针对于修饰函数：
	- 针对于`constexpr`修饰的函数，如果在编译时传入常量值，那么该函数会产生编译时常量，否则该函数就和普通函数一样。
	- `const`修饰的函数，不同于`constexpr`，无法在编译时被优化为常量，仅保证返回值为不可修改的变量，为运行时常量。
```c++
#include <array>

constexpr int foo1(int i) { return i + 5; }

const int foo2(int i) { return i + 5; }

const int g1 = 1;

int main()
{
    int i = 10;
    std::array<int, foo1(5)> a1;    // OK
    // std::array<int, foo2(5)> b1;    // ERROR 
    std::array<int, g1> c1;         // OK const 修饰变量不区分编译时和运行时

    foo1(i);    // 作为普通函数
    foo2(i);

    // std::array<int, foo1(i)> a2;    // ERROR

    return 0;
}
```

> 另：模板参数需要的是编译时常量

