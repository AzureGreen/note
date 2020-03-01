# c++11左值右值及引用
## 左值与右值
### 基础概念
- 左值：可以取地址的表达式，如变量名和函数名，具有**持久性**。
- 右值：字面常量或者表达式求值产生的**临时对象**，不可取地址。
	在c++11中，右值又可进一步分为纯右值和将亡值，不必刻意区分。
	- 纯右值：字面常量（字符串除外，如2, true...），求值结果的临时对象，返回非引用类型的函数调用（包括后++）。
	- 将亡值：随**右值引用**产生，因为右值引用通过移动本该销毁的临时对象的资源，而延长了其生命周期。

如下 *c++* 代码以及其反汇编可以看出，*v1* 就是一个左值，作为变量在内存中持久存在，而 1+1 算术表达式的结果 2 就是一个右值，临时存在，给 *v1 *赋完值就没了，并未保存在内存中。
```c++
int v1 = 1 + 1;
```
```asm
00183858 C7 45 F4 02 00 00 00 mov         dword ptr [v1],2  
```
## 左值引用与右值引用
### 左值引用
左值引用就是对左值的引用。通俗说法就是给左值变量起了个别名，都对应着左值的那个内存地址，对左值引用操作就是对左值操作。
#### 常量左值引用
左值引用不能引用右值，而常量左值引用是无敌的存在，可以引用右值。 

下述代码显示，当使用常量左值引用去引用一个右值时，会把该右值保存在栈区，而常量左值引用就对应着这个地址。
```c++
const int &cl_v1 = 1 + 1;
```
```asm
0018385F C7 45 DC 02 00 00 00 mov         dword ptr [ebp-24h],2  
00183866 8D 45 DC             lea         eax,[ebp-24h]  
00183869 89 45 E8             mov         dword ptr [cl_v1],eax  
```
### 右值引用
右值引用指向将要销毁的临时对象，这样右值引用可以接管该临时对象的所有资源，从而延长了该临时对象的生命周期。

从如下代码可以看到，其效果跟上述的常量左值引用效果相同，把右值保存在栈区，通过右值引用可以对该右值进行操作。
```c++
int &&r_v1 = 1 + 1;
```
```asm
0018386C C7 45 C4 02 00 00 00 mov         dword ptr [ebp-3Ch],2  
00183873 8D 45 C4             lea         eax,[ebp-3Ch]  
00183876 89 45 D0             mov         dword ptr [r_v1],eax 
```

### 移动与转发
在c++11标准中，有移动`std::move`和转发`std::forward`两种方法（都在`<utility>`中），针对于左值和右值，用他们的返回值作为其他函数传参都能实现不产生多余的拷贝。
- `std::move`：显示地将**左值转成右值引用**返回，不管左值是左值引用还是右值引用。
- `std::forward<T>(v)`：转发引用参数的原始值类别，如果传入左值引用，返回左值引用，如果传入右值引用，则返回右值引用，实现**完美转发**，相当于类型转换`static_cast<T&&>(v)`。
```c++
void reference(int &v) {
    std::cout << "left value reference" << std::endl;
}

void reference(int &&v) {
    std::cout << "right value reference" << std::endl;
}

template <typename T>
void pass(T &&v) {
    reference(v);
    reference(std::move(v));
    reference(std::forward<T>(v));
}

int main() {
    pass(233);	// 传递右值
    int v = 233;
    std::cout << "------" << std::endl;
    pass(v);	// 传递左值
    return 0;
}
```
上述代码输出如下，当传递右值时，v是右值引用，但同时v也是左值。
```
left value reference
right value reference
right value reference
------
left value reference
right value reference
left value reference
```
#### 移动语义
有了移动操作后，可以定义移动拷贝构造函数以及移动赋值运算符，这样他们将从给定的对象里“**搬移**”资源，而不是通过拷贝，这样将**节省时间与空间**。  
需要注意的是，在完成资源移动后，还需要保证**源对象不再指向被移动的资源**，因为资源已经属于新对象了，也就是将源对象至于“销毁无害”状态，即析构不影响新对象。

```c++
CDemo::CDemo(CDemo &&demo) noexcept	// 移动操作不应抛出异常
	: member_(demo.member_) 	// 接管demo中的资源
{
	demo.member_ = nullptr;	// 将demo至于销毁无害状态，对其析构不会影响新的对象
}

CDemo &CDemo::operator=(CDemo &&rhs) noexcept
{
	// 如果this与rhs地址相同，代表同一对象，不做任何操作
    if (this != &rhs) {
        // free();	
        member_ = rhs.member_;
        rhs.member_ = nullptr;
    }
    return *this;
}
```

## 参考资料
[1] C++ Primer中文版  
[[2]话说C++中的左值、纯右值、将亡值--同勉共进](https://www.cnblogs.com/zpcdbky/p/5275959.html)  
[[3]modern-cpp-tutorial](https://github.com/changkun/modern-cpp-tutorial)  