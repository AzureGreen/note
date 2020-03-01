#pragma once
#include <iostream>
#include <string>

namespace FactoryMethod {
// ��Ʒ���ࣨ����ӿڣ�
class Pizza {
public:
	Pizza() {}
	virtual ~Pizza() {}
	virtual void Prepare() {
		std::cout << "prepare" << std::endl;
	}
	virtual void Bake() {
		std::cout << "bake" << std::endl;
	}
	virtual void Cut() {
		std::cout << "cut" << std::endl;
	}
	virtual void Box() {
		std::cout << "box" << std::endl;
	}
	std::string GetName() {
		return name_;
	}

	std::string name_;
};
// ��Ʒ���ࣨ�������
class NYStyleCheesePizza : public Pizza {
public:
	NYStyleCheesePizza() {
		name_ = "NY Style Cheese Pizza";
	}
	~NYStyleCheesePizza() {}
	// ������Ը���ĳЩ�ض����󸲸ǻ������
	void Prepare() {
		std::cout << "prepare for NY style cheese pizza" << std::endl;
	}
	void Bake() {
		std::cout << "bake for NY style cheese pizza" << std::endl;
	}
	void Cut() {
		std::cout << "cut for NY style cheese pizza" << std::endl;
	}
	void Box() {
		std::cout << "box for NY style cheese pizza" << std::endl;
	}
};
class NYStylePepperoniPizza : public Pizza {
public:
	NYStylePepperoniPizza() {
		name_ = "NY Style Pepperoni Pizza";
	}
	~NYStylePepperoniPizza() {}
	// ������Ը���ĳЩ�ض����󸲸ǻ��������ʡ��
};
class NYStyleClamPizza : public Pizza {
public:
	NYStyleClamPizza() {
		name_ = "NY Style Clam Pizza";
	}
	~NYStyleClamPizza() {}
	// ������Ը���ĳЩ�ض����󸲸ǻ��������ʡ��
};
class NYStyleVeggiePizza : public Pizza {
public:
	NYStyleVeggiePizza() {
		name_ = "NY Style Veggie Pizza";
	}
	~NYStyleVeggiePizza() {}
	// ������Ը���ĳЩ�ض����󸲸ǻ��������ʡ��
};
class PizzaStore {
public:
	PizzaStore() {};
	virtual ~PizzaStore() {};
	Pizza* OrderPizza(std::string type) {
		Pizza* pizza;
		pizza = CreatePizza(type);	// ʹ�ù��������õ�����Ķ���(��Ʒ)��������������������ڵ�ǰ������Ĵ��룬�����ں������õ�����ʲô������Ϊ�������̳����Ǹ�����Ĳ�Ʒ���ࡣ

		pizza->Prepare();
		pizza->Bake();
		pizza->Cut();
		pizza->Box();
		return pizza;
	}
	virtual Pizza* CreatePizza(std::string type) = 0;
};
// һ������ ŦԼ���ı�����
class NYStylePizzaStore : public PizzaStore {
public:
	NYStylePizzaStore() {}
	~NYStylePizzaStore() {}

	// ʵ�ֽӿڣ�������Ҫ���ؾ������(��Ʒ)
	Pizza* CreatePizza(std::string type) {
		Pizza* pizza = nullptr;
		if (type == "cheese") {
			pizza = new NYStyleCheesePizza();
		}
		else if (type == "pepperoni") {
			pizza = new NYStylePepperoniPizza();
		}
		else if (type == "clam") {
			pizza = new NYStyleClamPizza();
		}
		else if (type == "veggie") {
			pizza = new NYStyleVeggiePizza();
		}
		return pizza;
	}
};
}


