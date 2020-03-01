# 有关设计的一些原则记录

## 1.策略模式 Strategy Pattern
定义算法族，分别封装起来，让他们之间可以相互替换。让算法的变化独立于使用算法的客户。

### 1.1.变化分离原则
具有通用性的属性与方法可由一个基类类封装，而其他特殊的属性方法应分离出来，由单独的类去实现，然后根据需要，通过定义对应的类的对象作为成员，从而具有那些特殊的属性及方法。


### 1.2.利用多态原则
针对“普遍性方法”，我们通过定义一个接口/抽象类，提供统一接口，然后具体的不同实现由子类去派生，在使用“普遍性方法”时，利用运行时多态，仅需通过接口/抽象类对象提供的统一接口即可达到所需的效果。

### 1.3.多用组合原则
将两个类结合起来使用的方法叫做`组合`，如下例中的Animal和SoundType，就是通过定义类对象的方式来使用类的功能。相比于`继承`，使用`组合`会使系统更具有弹性。

```c++
#include <iostream>

// An abstract class / interface
class SoundType {
public:
	SoundType() {}
	virtual ~SoundType() {}
	virtual void MakeNoise() = 0;
};
// class derived from SoundType
class Bark :public SoundType {
public:
	Bark() {}
	~Bark() {}
	void MakeNoise() {
		std::cout << "know-wow" << std::endl;
	}
};
// class derived from SoundType
class Meow :public SoundType {
public:
	Meow() {}
	~Meow() {}
	void MakeNoise() {
		std::cout << "Meow" << std::endl;
	}
};

// Base Class
class Animal {
public:
	Animal() {};
	~Animal() {};

	void MakeNoise() {
		sound_type_->MakeNoise();
	}

	void SetSound(SoundType* sound_type) {
		delete sound_type_;		// release memory to prevent memory leak
		sound_type_ = sound_type;	// dynamic set behavior
	}

	SoundType* sound_type_;	// define an abstract object
};

// Sub class
class Dog : public Animal {
public:
	Dog() {
		sound_type_ = new Bark();	// Polymorphism
	};
	~Dog() {
		delete sound_type_;
	};
};

// testing
void StrategyTest() {
	Dog dog;
	dog.MakeNoise();
	dog.SetSound(new Meow());
	dog.MakeNoise();
}
```

## 2.观察者模式 Observer Pattern
定义了对象之间的一（Subject）对多（Observer）依赖，这样一来，当一个对象改变状态时，它的所有依赖者都会收到通知并自动更新。

### 2.1.松耦合设计原则
松耦合设计让对象间的依赖降到了最低，抽象到只需知道对方会实现某个`接口`即可，这样某一方具体的改变并不会影响到其他对象，从而提高复用性，使面向对象系统更有弹性。

```c++
#include <iostream>
#include <vector>

class Subject;
class Observer;

// observer
class Observer {
public:
	Observer() {}
	virtual ~Observer() {}
	virtual void Update(Subject* subject) = 0;
};

// subject 
class Subject {
public:
	Subject() {}
	~Subject() {}
	void RegisterObserver(Observer* observer) {
		observers_.push_back(observer);
	}
	void RemoveObserver(Observer* observer) {
		auto it = std::find(observers_.cbegin(),
			observers_.cend(), observer);
		if (it != observers_.cend()) {
			observers_.erase(it);
		}
	}
	virtual void NotifyObservers() {
		for (Observer*& observer : observers_) {
			observer->Update(this);		// this is push method
		}
	}
private:
	std::vector<Observer*> observers_;
};

// Implementation of subject
class WeatherData : public Subject {
public:
	WeatherData() {}
	~WeatherData() {}
	void SetStates(int temperature, int wind) {
		temperature_ = temperature;
		wind_ = wind;
		NotifyObservers();
	}
	// these getter methods prepare for pull
	int GetTemperature() const { return temperature_; }
	int GetWind() const { return wind_; }
private:
	// some states below
	int temperature_;
	int wind_;
};

// Implementation of observer
class FirstBand : public Observer {
public:
	FirstBand(Subject* weather_data) {
		weather_data_ = weather_data;
		weather_data_->RegisterObserver(this);
	}
	~FirstBand() {
		weather_data_->RemoveObserver(this);
	}
	void Update(Subject* weather_data) { 
		temperature_ = ((WeatherData*)weather_data)->GetTemperature();
		wind_ = ((WeatherData*)weather_data)->GetWind();
		Show();
	}
	void Show() {
		std::cout << "temperature:" << temperature_ << std::endl;
		std::cout << "wind:" << wind_ << std::endl;
	}
private:
	Subject* weather_data_;
	int temperature_;
	int wind_;
};

// testing
void ObserverTest() {
	WeatherData* weather_data = new WeatherData();
	FirstBand* first_band = new FirstBand(weather_data);
	weather_data->SetStates(23, 2);
	weather_data->SetStates(20, 4);
	weather_data->SetStates(22, 3);
}
```

## 3.装饰者模式
动态地给对象增加额外责任。相比于继承，装饰者提供了更具弹性（动态）的方式达到扩展功能的目的。

但是装饰者模式容易造成系统有许多小类，显得比较复杂。

### 3.1.开放-关闭原则
类应该对扩展开放，对修改关闭。我们应该在不修改现有代码的情况下，让类容易扩展，可以搭配新的行为，使设计具有弹性。装饰者模式正好满足该需求。

```c++
/*
class Component {
    void operator() {}
};
class Decorator: public Component {
    void operator() {}
};
class ConcreteComponent: public Component {
    void operator() {}
};
class ConcreteDecorator: public Decorator {
    void operator() {}
};
*/

#include <iostream>
#include <string>

// component
class Beverage {
public:
	Beverage()
		: description_("Unknown")
	{}
	virtual ~Beverage() {}
	virtual std::string GetDescription() {
		return description_;
	}
	virtual double Cost() const = 0;

	std::string description_;
};

// decorator
class CondimentDecorator : public Beverage {
public:
	CondimentDecorator() {}
	virtual ~CondimentDecorator() {}
	virtual std::string GetDescription() = 0;
};

// concrete component
class HouseBlend : public Beverage {
public:
	HouseBlend() {
		description_ = "House Blend Coffee";
	}
	~HouseBlend() {}
	double Cost() const { return 0.89; }
};

// concrete decorator
class Mocha : public CondimentDecorator {
public:
	Mocha(Beverage* beverage)
		: beverage_(beverage) 	// both concrete component & decorator all have the same super class.
	{}
	~Mocha() {
		delete beverage_;
	}
	std::string GetDescription() {
		return beverage_->GetDescription() + ", Mocha";
	}
	double Cost() const {
		return 0.20 + beverage_->Cost();
	}
private:
	Beverage* beverage_;
};

class Soy : public CondimentDecorator {
public:
	Soy(Beverage* beverage)
		: beverage_(beverage)
	{}
	~Soy() {
		delete beverage_;
	}
	std::string GetDescription() {
		return beverage_->GetDescription() + ", Soy";
	}
	double Cost() const {
		return 0.15 + beverage_->Cost();
	}
private:
	Beverage* beverage_;
};

// testing
void DecoratorTest() {
	Beverage* beverage = new HouseBlend();
	beverage = new Mocha(beverage);
	beverage = new Mocha(beverage);
	beverage = new Soy(beverage);
	std::cout << beverage->GetDescription() << " $" << beverage->Cost() << std::endl;
}
```

## 4.工厂模式

封装对象的创建，便于管理与后期维护。

### 4.1.依赖倒置原则
不论是“高层组件”亦或是“低层组件”，都应该依赖于抽象，而不是具体类型。

### 4.2.简单工厂
简单工厂不是一个设计模式，但是是一种编程习惯，作为一个简单的方法，达到客户程序与具体类解耦合。
```c++
// 简单工厂类
class SimplePizzaFactory {
public:
	Pizza CreatePizza(std::string type) {
        Pizza* pizza = nullptr;
        if (type == "cheese") {
            pizza = new CheesePizza();
        } else if (type == "pepperoni") {
            pizza = new PepperoniPizza();
        } else if (type == "clam") {
            pizza = new ClamPizza();
        } else if (type == "veggie") {
            pizza = new VeggiePizza();
        }
        return pizza;
	}
};

class PizzaStore {
public:    
    PizzaStore(SimplePizzaFactory factory)
    : factory_(factory) {}
    ~PizzaStore() {}
    
    Pizza* OrderPizza(std::string type) {
        pizza = factory_.CreatePizza(type);	// 通过工厂对象的方法创建对象
        
        pizza.Prepare();
        pizza.Bake();
        pizza.Cut();
        pizza.Box();
       	return pizza;
    }
private:
	SimplePizzaFactory* factory_;
};
```
### 4.3.工厂方法模式
在基类中定义创建对象的接口，然后由子类通过继承的方式决定实例化的具体对象，而基类并不在意具体对象是什么，仍然可以继续执行剩下代码，从而实现了产品的“实现”与“使用”的解耦合。
```c++
// 抽象产品类
class Product {
    virtual void Operation() = 0;
}
// 具体产品类
class ConcreteProduct：public Product {
    void Operation() {}
}
// 制造者
class Creator {
    virtual FactoryMethod() = 0;	// 工厂方法，产生对象的接口
    void Operation() {};		// 使用抽象产品引用，进行操作
}
class ConcreteCreator: public Creator {
    FactoryMethod() {
        // 根据需求，返回具体的对象(产品)
    }
}

/*-------------------------------*/
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
// testing
void FactoryMethodPattern() {
	FactoryMethod::PizzaStore* pizza_store = new FactoryMethod::NYStylePizzaStore();
	FactoryMethod::Pizza* pizza = pizza_store->OrderPizza("cheese");		// 这里相当于创建了一个对象
	std::cout << "A customer ordered a " << pizza->GetName() << std::endl;

	// 清除对象资源
	delete pizza;
	delete pizza_store;
}

```
### 4.4.抽象工厂模式

提供一个接口(抽象类)，其中有创建一系列对象(产品)的接口，即产品家族，然后子类去实现这些接口从而创建具体的对象，每一个创建对象的接口都可以看作是一个工厂方法。

> 基类定义接口族，子类实现接口族。

```c++
// 产品接口
class AbstractProductA {}
// 产品具体实现
class ProductA1: public AbstractProductA {}
class ProductA2: public AbstractProductA {}

// 抽象工厂接口
class AbstractFactory {
    virtual void CreateProductA() = 0; 	// 每一个产品接口都相当于是一个工厂方法
    virtual void CreateProductB() = 0;
}
// 具体工厂实现
class ConcreteFactory: public AbstractFactory {
    void CreateProductA() {};	// 返回实际对象
    void CreateProductB() {};
}
// 客户代码，只用涉及抽象工厂，运行时自动获取实际工厂

/*-------------------------------*/
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
// testing
void AbstractFactoryPattern() {
	AbstractFactory::PizzaStore* pizza_store = new AbstractFactory::NYStylePizzaStore();
	AbstractFactory::Pizza* pizza = pizza_store->OrderPizza("cheese");		// 这里相当于创建了一个对象
	std::cout << "A customer ordered a " << pizza->GetName() << std::endl;

	// 清除对象资源
	delete pizza;
	delete pizza_store;
}
```

## 5.单例模式
通过将构造函数属性设为`private`，然后提供全局访问点：`getInstance`静态方法和一个静态变量，确保一个类只有一个实例化对象，并且可以到的`延迟实例化`的效果。

```c++
// 最简单的单例模式
class Singleton {
private:
	Singleton() {}		// 构造函数定义为私有，只有类内部自己可以构造
	// 还需要考虑 拷贝构造函数 运算符重载
	~Singleton() {}
public:
	static Singleton* GetInstance() {	// 唯一实例对象的全局访问点
        if (unique_instance_ == nullptr) {
            unique_instance_ = new Singleton();
        }
        return unique_instance_;
	}
private:
	static Singleton* unique_instance_;		// 利用静态变量记录Singleton类的唯一实例
}
```
但是涉及到多线程，会出现问题：线程一进入`GetInstance`判断`unique_instance_`为空后，切换到了线程二也进入`GetInstance`判断`unique_instance_`为空，然后又切换到线程一，实例化一个对象后返回该对象，这时有切换到线程二，同样又实例化一个对象，返回对象，这样的话，整个系统就有了两个实例化对象。考虑到这里，可以给程序加个锁保证线程同步。

```c++
// 涉及线程安全的单例模式 Double Check
#include <iostream>
#include <mutex>

class Singleton {
public:
	static Singleton* GetInstance() {
		if (unique_instance_ == nullptr) {
			// 上锁
			mutex_.lock();
			if (unique_instance_ == nullptr) {
				unique_instance_ = new Singleton();
			}
			mutex_.unlock();
		}
		return unique_instance_;
	}
	void DoSomethin() {
		std::cout << "do something here!" << std::endl;
	}

private:
	Singleton() {}
	~Singleton() {}

	static Singleton* unique_instance_;
	static std::mutex mutex_;
};

Singleton* Singleton::unique_instance_ = nullptr;
std::mutex Singleton::mutex_;

void SingletonPattern() {
	Singleton* instance = Singleton::GetInstance();
	instance->DoSomethin();
}
```






