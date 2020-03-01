# 虚表
**虚表**产生于有虚函数的类中，它存储了所有虚函数的地址。带有虚函数的类对象创建后，存储的第一个成员（\*this）为一个指针，也就是**虚表指针**。

## 虚表的继承关系
当涉及到类继承时，在构造子类时会调用父类的构造函数，给this指针赋上父类虚表指针，回到子类构造函数后，子类虚表指针又将覆盖父类虚表指针，析构过程相反。虚表有如下性质：
1. 子类对象的this指针首成员即为虚表指针，指向子类的虚函数表，虚函数表里首先按声明顺序存着父类的虚函数地址，如果子类有覆盖父类虚函数，则替换为子类虚函数地址，接着是子类自己的虚函数。

    > （<font  color='red'>父类虚函数在前，子类虚函数在后</font >）

2. 如果是多继承，子类对象的this指针首成员为第一个虚表指针，指向的虚表保存着第一个父类和子类自己的虚函数地址，规则与（1）相同。this + 第一个父类大小（考虑类成员变量）偏移后内存保存着第二个虚表指针，指向的虚表保存着第二个父类的虚函数，规则与（1）相同，剩下父类以此类推。

    > （<font  color='red'>第一个父类和自己虚函数在第一张虚表里，第二个父类虚函数在第二张虚表，以此类推</font >）

3. 每一张虚表以<font color='red'>0x00</font>作为结束标识，类对象以4/8字节的<font  color='red'>0xCC</font >作为结束标识符，。
   
```
                                 虚 表
this------>第一个虚表指针地址------>虚函数1
           |			 | ----->虚函数2
           |			 | ----->虚函数3
           |			 |		 ......
           |             |         NULL
           （继承第一个类成员变量）
           第二个虚表指针地址 ----->虚函数1
           |			 | ----->虚函数2
           |		     | ----->虚函数3
           |			 |	   	 ......
           | 			 |         NULL
           （继承第二个类成员变量）
                 ......	
            （类自己的成员变量）
            0xCCCCCCCC标识类的结束
```
> 详细分析流程可以在vs中调试反汇编查看
```c++
#include <iostream>

using namespace std;

class Base
{
public:
	Base()
	{
	}
	virtual void Fun1()
	{
		cout << "Base::Fun1()" << endl;
	}
	virtual void Fun2()
	{
		cout << "Base::Fun2()" << endl;
	}
	int base_data_ = 1;
};

class Middle1 : public Base
{
public:
	Middle1()
	{
	}
	virtual void Fun1()
	{
		cout << "Middle1::Fun1()" << endl;
	}
	virtual void Fun3()
	{
		cout << "Middle1::Fun3()" << endl;
	}
	int middle1_data_ = 2;
};

class Middle2
{
public:
	Middle2()
	{
	}
	virtual void Fun2()
	{
		cout << "Middle2::Fun2()" << endl;
	}
	virtual void Fun3()
	{
		cout << "Middle2::Fun3()" << endl;
	}
	int middle2_data_ = 3;
};

class Sub : public Middle1, public Middle2
{
public:
	Sub()
	{
	}
	virtual void Fun5() {
		cout << "Sub::Fun5()" << endl;
	}
	int sub_data_ = 5;
};
```
```c++
% 查看sub对象内存
0x00EFF7D4  80 a0 16 00 	% 第一张虚表地址，保存第一个父类以及自己的虚函数
0x00EFF7D8	01 00 00 00		% 第一个父类的成员变量（父类的父类）Base::base_data_
0x00EFF7DC	02 00 00 00 	% 第一个父类的成员变量 Middle1::middle1_data_
0x00EFF7E0	64 9b 16 00 	% 第二张虚表地址，保存第二个父类虚函数
0x00EFF7E4	03 00 00 00 	% 第二个父类的成员变量 Middle2::middle2_data_
0x00EFF7E8	05 00 00 00 	% 自己的成员变量 Sub::sub_data_
0x00EFF7EC	cc cc cc cc		% 标识类结束
```
```c++
% 查看虚表内容
0x0016A080  c7 11 16 00 	% 第一个父类的虚函数 Middle1::Fun1()
0x0016A084	32 10 16 00 	% 第一个父类的虚函数（父类的父类） Base::Fun2()
0x0016A088	60 14 16 00 	% 第一个父类的虚函数 Middle1::Fun3()
0x0016A08C	dd 14 16 00 	% 自己的虚函数 Sub::Fun5()
0x0016A090	00 00 00 00

0x00169B64  38 14 16 00 	% 第二个父类的虚函数 Middle2::Fun2()
0x00169B68	5b 14 16 00 	% 第二个父类的虚函数 Middle2::Fun3()
0x00169B6C	00 00 00 00
```

# 虚基类表
在虚继承时会出现虚基类表概念，虚继承即在继承时加上`virtual`关键字，用于解决重复继承产生的存储空间浪费与二义性问题。
## 虚基类表与虚继承
涉及到虚继承时，在构造子类时会先给this偏移地址大小（4/8字节）的位置保存**虚基类表指针**，然后按继承顺序先进父类构造函数完成一系列初始化与虚表指针赋值等操作，最后给this指向的首地址赋值类自己的虚表指针（仅保存子类自己的虚函数）。虚继承有如下性质：
1. 子类对象的this指针首成员是自己的虚表指针，对应的虚表**仅**保存着自己**独有**的虚函数。
2. 子类对象的this指针偏移4/8字节是虚基类表指针，对应的虚基类表保存着自己以及虚继承的父类相对于虚基类表指针的偏移。

    > <font color='red'>多态时，根据对应的偏移找到对应的对象</font>
3. 根据虚基类表中的偏移以及虚基类表指针计算出父类们的地址，保存着父类相应的虚表指针，指向的虚表保存着虚函数，其中子类如有覆盖父类虚函数，虚函数地址会替换为子类虚函数地址。

```c++
this------>自己的虚表指针地址------>虚函数1
           |			 |		 ......
           |             |         NULL
           （继承第一个类成员变量）
			 虚基类表指针地址 ----->子类相对于虚基类表指针偏移
           |			 | ----->第一个父类相对于虚基类表指针偏移
           |		     | ----->第二个......
           | 			 |         NULL
           （继承第二个类成员变量）
				......	
           （类自己的成员变量）
           第一个父类虚表指针------>虚函数1
           |			 |		 ......
           |             |         NULL
           （继承第一个类成员变量）
			第二个虚表指针地址----->虚函数1
		   |			 |	   	 ......
           | 			 |         NULL
           （继承第二个类成员变量）
                 ......	
			0xCCCCCCCC标识类的结束
```
> 详细分析流程可以在vs中调试反汇编查看
```c++
#include <iostream>

using namespace std;

class VBase1
{
public:
	VBase1()
	{
	}
	virtual void Fun1()
	{
		cout << "Base1::Fun1()" << endl;
	}
	virtual void Fun2()
	{
		cout << "Base1::Fun1()" << endl;
	}
	int base_data_ = 1;
};

class VBase2
{
public:
	VBase2()
	{
	}
	virtual void Fun1()
	{
		cout << "Base2::Fun1()" << endl;
	}
	virtual void Fun3()
	{
		cout << "Base2::Fun3()" << endl;
	}
	int base_data1_ = 2;
	int base_data2_ = 3;
};

class VSub : virtual public VBase1, virtual public VBase2
{
public:
	VSub()
	{
	}
	virtual void Fun2()
	{
		cout << "Sub::Fun2()" << endl;
	}
	virtual void Fun3()
	{
		cout << "Sub::Fun3()" << endl;
	}
	virtual void Fun5()
	{
		cout << "Sub::Fun5()" << endl;
	}
	int sub_data_ = 3;
};
```
```c++
% 查看Sub对象内存
0x0076F6E8  8c 9c 1e 00 	% 子类自己的虚表地址
0x0076F6EC	b4 9c 1e 00 	% 虚基类表地址
0x0076F6F0	03 00 00 00 	% 子类的成员变量 VSub::sub_data_
0x0076F6F4	00 00 00 00 
0x0076F6F8	98 9c 1e 00 	% 包含第一个父类虚函数的表地址（会有子类覆盖虚函数）
0x0076F6FC	01 00 00 00 	% 第一个父类的成员变量 VBase1::base_data_
0x0076F700	00 00 00 00 
0x0076F704	a8 9c 1e 00 	% 包含第二个父类虚函数的表地址（同样会有子类覆盖虚函数）
0x0076F708  02 00 00 00 	% 第二个父类的成员变量 VBase2::base_data1_
0x0076F70C	03 00 00 00 	% 第二个父类的成员变量 VBase2::base_data2_
0x0076F710	cc cc cc cc		% 类的结束标识
```
```c++
% 查看虚基类表内容
0x001E9CB4  fc ff ff ff 	% 子类对象相对于虚基类表的偏移（this相对于虚基类表的偏移）
0x001E9CB8	0c 00 00 00 	% 第一个父类相对于虚基类表的偏移
0x001E9CBC	18 00 00 00		% 第二个父类相对于虚基类的偏移
0x001E9CC0	00 00 00 00
```

# 菱形继承
形如“B1、B2都继承于B，而D继承于B1、B2”的情况，会形成菱形继承，那样会导致调用D的成员时会导致二义性，因此使用前面提到的**虚继承**解决该问题。
```
         B
        / \ 
       B1 B2  
        \ /
         D
```
```c++
#include <iostream>

using namespace std;

class DBase
{
public:
	DBase()
	{
	}
	virtual void Fun1() 
	{
		cout << "Base::Fun1()" << endl;
	}
	virtual void Fun2()
	{
		cout << "Base::Fun2()" << endl;
	}
	virtual void Fun3()
	{
		cout << "Base::Fun3()" << endl;
	}
	int base_data_ = 1;
};

class DMiddle1 : public virtual DBase
{
public:
	DMiddle1()
	{
	}
	virtual void Fun1()
	{
		cout << "Middle1::Fun1()" << endl;
	}
	int middle2_data_ = 2;
};

class DMiddle2 : public virtual DBase
{
public:
	DMiddle2()
	{
	}
	virtual void Fun2()
	{
		cout << "Middle2::Fun2()" << endl;
	}
	int middle2_data_ = 3;
};

class DMiddle3
{
public:
	DMiddle3()
	{
	}
	virtual void Fun6() {
		cout << "Middle3::Fun6()" << endl;
	}
	virtual void Fun5()
	{
		cout << "Middle3::Fun5()" << endl;
	}
	int middle3_data_ = 5;
};

class DSub : public DMiddle1, public DMiddle2, public DMiddle3
{
public:
	DSub()
	{
	}
	virtual void Fun5() {
		cout << "Sub::Fun5()" << endl;
	}
	int sub_data_ = 4;
};
```
```c++
最后的内存结构为：
0x00EFFB7C  08 9c 04 00 	% D的第一个虚表指针
0x00EFFB80	05 00 00 00 	% D的第一个父类成员变量 DMiddle3::middle3_data_
							% D的第二个虚表指针
							% D的第二个父类成员变量
							% ......
0x00EFFB84	28 9c 04 00 	% D的第一个虚基类表指针 DMiddle1
0x00EFFB88	02 00 00 00 	% D的第一个虚基类成员变量...
0x00EFFB8C	38 9c 04 00 	% D的第二个虚基类表指针 DMiddle2
0x00EFFB90	03 00 00 00 	% D的第二个虚基类成员变量...
0x00EFFB94	04 00 00 00 	% D自己的成员变量
0x00EFFB98	00 00 00 00 	
0x00EFFB9C	14 9c 04 00 	% D保存虚基类虚函数的虚表的指针（DMiddle1、DMiddle2、DBase）
0x00EFFBA0	01 00 00 00 	% D的虚基类虚继承的类成员变量
0x00EFFBA4	cc cc cc cc
```