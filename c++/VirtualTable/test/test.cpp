// test.cpp : 定义控制台应用程序的入口点。
//

#include <iostream>

using namespace std;

class Base {
public:
	Base() : v2_(2) {
		v3_ = 3;
	}

	virtual void Hide(int a) {
		cout << "Base Hide(int)" << endl;
	}

	int v1_ = 1;
	int v2_;
	int v3_;
};


class Derived : public Base {
public:
	Derived() {}
	~Derived() {}

	void Hide() {
		//Hide(1);
		cout << "Derived Hide()" << endl;
	}
};


int main()
{	
	//Base b1;
	Derived d1;
	//d1.Hide(1);

    return 0;
}

