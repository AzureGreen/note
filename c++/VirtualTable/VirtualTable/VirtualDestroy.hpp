#pragma once

#include <iostream>

using namespace std;

class Base1 {

public:
	Base1() {
		cout << "This is Base1 Constructor Func" << endl;
	}
	~Base1() {
		cout << "This is Base1 Destroy Func" << endl;
	}
};

class Base2 {
public:
	Base2() {
		cout << "This is Base2 Constructor Func" << endl;
	}
	virtual ~Base2() {
		cout << "This is Base2 Destroy Func" << endl;
	}
};

class Derived1: public Base1 {
public:
	Derived1() {
		cout << "This is Derived1 Constructor Func" << endl;
	}
	~Derived1() {
		cout << "This is Derived1 Destroy Func" << endl;
	}
};


class Derived2 : public Base2 {
public:
	Derived2() {
		cout << "This is Derived2 Constructor Func" << endl;
	}
	~Derived2() {
		cout << "This is Derived2 Destroy Func" << endl;
	}
};
