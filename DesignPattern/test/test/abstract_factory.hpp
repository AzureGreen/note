#pragma once
#include <iostream>
#include <string>

namespace AbstractFactory {

// 产品
class Dough {};
class ThinCrustDough : public Dough {};
class Sauce {};
class MarinaraSauce : public Sauce {};
class Cheese {};
class ReggianoCheese : public Cheese {};
// 抽象工厂
class PizzaIngredientFactory {
public:
	PizzaIngredientFactory() {}
	virtual ~PizzaIngredientFactory() {}
	virtual Dough* CreateDough() = 0;
	virtual Sauce* CreateSauce() = 0;
	virtual Cheese* CreateCheese() = 0;
};
// 具体工厂
class NYPizzaIngredientFactory : public PizzaIngredientFactory {
public:
	Dough* CreateDough() { return new ThinCrustDough(); }
	Sauce* CreateSauce() { return new MarinaraSauce(); }
	Cheese* CreateCheese() { return new ReggianoCheese(); }
};
// Client
class Pizza {
public:
	Pizza() {}
	virtual ~Pizza() {}
	virtual void Prepare() = 0;
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
	virtual void SetName(std::string name) = 0;

	std::string name_;
	PizzaIngredientFactory* ingredient_factory_;
	Dough* dough_;
	Sauce* sauce_;
	Cheese* cheese_;
};
class CheesePizza : public Pizza {
public:
	CheesePizza(PizzaIngredientFactory* ingredient_factory) {
		ingredient_factory_ = ingredient_factory;
	}
	~CheesePizza() {
		delete dough_;	// 清除资源
		delete sauce_;
	}
	void Prepare() {
		std::cout << "prepare" << std::endl;
		dough_ = ingredient_factory_->CreateDough();
		sauce_ = ingredient_factory_->CreateSauce();
	}
	void SetName(std::string name) {
		name_ = name;
	}
};
class PizzaStore {
public:
	PizzaStore() {};
	virtual ~PizzaStore() {};
	Pizza* OrderPizza(std::string type) {
		Pizza* pizza;
		pizza = CreatePizza(type);
		pizza->Prepare();
		pizza->Bake();
		pizza->Cut();
		pizza->Box();
		return pizza;
	}
	virtual Pizza* CreatePizza(std::string type) = 0;
};
class NYStylePizzaStore : public PizzaStore {
public:
	NYStylePizzaStore() {}
	~NYStylePizzaStore() {}

	// 实现接口，根据需要返回具体对象(产品)
	Pizza* CreatePizza(std::string type) {
		Pizza* pizza = nullptr;
		PizzaIngredientFactory* factory = new NYPizzaIngredientFactory();
		if (type == "cheese") {
			pizza = new CheesePizza(factory);
			pizza->SetName("New York Style Cheese Pizza");
		}
		/*
		else if (type == "pepperoni") {
			pizza = new PepperoniPizza(factory);
			pizza->SetName("New York Style Pepperoni Pizza");
		}
		else if (type == "clam") {
			pizza = new ClamPizza(factory);
			pizza->SetName("New York Style Calm Pizza");
		}
		else if (type == "veggie") {
			pizza = new VeggiePizza(factory);
			pizza->SetName("New York Style Veggie Pizza");
		}*/
		return pizza;
	}
};
}
