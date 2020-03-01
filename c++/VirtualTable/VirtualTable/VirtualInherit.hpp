#pragma once
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
