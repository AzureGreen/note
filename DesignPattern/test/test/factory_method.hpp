#pragma once
#include <iostream>
#include <string>

namespace FactoryMethod {
// 产品基类（抽象接口）
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
// 产品子类（具体对象）
class NYStyleCheesePizza : public Pizza {
public:
	NYStyleCheesePizza() {
		name_ = "NY Style Cheese Pizza";
	}
	~NYStyleCheesePizza() {}
	// 下面可以根据某些特定需求覆盖基类操作
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
	// 下面可以根据某些特定需求覆盖基类操作，省略
};
class NYStyleClamPizza : public Pizza {
public:
	NYStyleClamPizza() {
		name_ = "NY Style Clam Pizza";
	}
	~NYStyleClamPizza() {}
	// 下面可以根据某些特定需求覆盖基类操作，省略
};
class NYStyleVeggiePizza : public Pizza {
public:
	NYStyleVeggiePizza() {
		name_ = "NY Style Veggie Pizza";
	}
	~NYStyleVeggiePizza() {}
	// 下面可以根据某些特定需求覆盖基类操作，省略
};
class PizzaStore {
public:
	PizzaStore() {};
	virtual ~PizzaStore() {};
	Pizza* OrderPizza(std::string type) {
		Pizza* pizza;
		pizza = CreatePizza(type);	// 使用工厂方法拿到具体的对象(产品)，但后面操作还是依赖于当前抽象类的代码，并不在乎具体拿到的是什么对象，因为具体对象继承自那个抽象的产品基类。

		pizza->Prepare();
		pizza->Bake();
		pizza->Cut();
		pizza->Box();
		return pizza;
	}
	virtual Pizza* CreatePizza(std::string type) = 0;
};
// 一个例子 纽约风格的比萨店
class NYStylePizzaStore : public PizzaStore {
public:
	NYStylePizzaStore() {}
	~NYStylePizzaStore() {}

	// 实现接口，根据需要返回具体对象(产品)
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


