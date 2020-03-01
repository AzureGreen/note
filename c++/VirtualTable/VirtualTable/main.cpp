
#include "DiamondInherit.hpp"
#include "VirtualInherit.hpp"
#include "VirtualTable.hpp"
#include "VirtualDestroy.hpp"


void TestDiamondInherit() {
	DSub sub;
	sub.Fun3();
	sub.Fun3();
	//DMiddle1 m1;
	DMiddle1 & m1 = dynamic_cast<DMiddle1&>(sub);
	DMiddle2 & m2 = dynamic_cast<DMiddle2&>(sub);

	m1.Fun1();
	m1.Fun2();
	m2.Fun1();
	m2.Fun2();

	int a = 0;

}

void TestVirtualTable() {
	Sub sub;
	sub.Fun2();
	int a = 0;
}

void TestVirtualInherit() {
	VSub sub;

	VBase1 & base1 = dynamic_cast<VBase1&>(sub);
	VBase2 & base2 = dynamic_cast<VBase2&>(sub);
	base1.Fun1();
	int a = 0;

}

void TestVirtualDestroy() {
	Base1 *base1 = new Derived1();
	base1->Test1();
	delete base1;
	Base2 *base2 = new Derived2();
	delete base2;
	Derived1 *derived1 = new Derived1();
	delete derived1;
}

int main() {

	//TestDiamondInherit();
	//TestVirtualTable();
	//TestVirtualInherit();
	TestVirtualDestroy();
	return 0;
}
