#pragma once
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