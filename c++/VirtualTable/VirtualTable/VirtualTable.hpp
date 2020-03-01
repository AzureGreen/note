#pragma once
#include <iostream>

using namespace std;

//class Base
//{
//public:
//	Base()
//	{
//	}
//	virtual void Fun1()
//	{
//		cout << "Base::Fun1()" << endl;
//	}
//	virtual void Fun2()
//	{
//		cout << "Base::Fun2()" << endl;
//	}
//	int base_data_ = 1;
//};


class Middle1 //: public Base
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