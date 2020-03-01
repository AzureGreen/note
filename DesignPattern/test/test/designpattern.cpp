// designpattern.cpp : �������̨Ӧ�ó������ڵ㡣
//

//#include "stdafx.h"

#include <string>
#include <algorithm>

#include "strategy.hpp"
#include "observer.hpp"
#include "decorator.hpp"
#include "factory_method.hpp"
#include "abstract_factory.hpp"
#include "singleton.hpp"

void StrategyTest() {
	Dog dog;
	dog.MakeNoise();
	dog.SetSound(new Meow());
	dog.MakeNoise();
}

void ObserverTest() {
	WeatherData* weather_data = new WeatherData();
	FirstBand* first_band = new FirstBand(weather_data);
	weather_data->SetStates(23, 2);
	weather_data->SetStates(20, 4);
	weather_data->SetStates(22, 3);
}

void DecoratorTest() {
	Beverage* beverage = new HouseBlend();
	beverage = new Mocha(beverage);
	beverage = new Mocha(beverage);
	beverage = new Soy(beverage);
	std::cout << beverage->GetDescription() << " $" << beverage->Cost() << std::endl;
}

void FactoryMethodPattern() {
	FactoryMethod::PizzaStore* pizza_store = new FactoryMethod::NYStylePizzaStore();
	FactoryMethod::Pizza* pizza = pizza_store->OrderPizza("cheese");		// �����൱�ڴ�����һ������
	std::cout << "A customer ordered a " << pizza->GetName() << std::endl;

	// ���������Դ
	delete pizza;
	delete pizza_store;
}

void AbstractFactoryPattern() {
	AbstractFactory::PizzaStore* pizza_store = new AbstractFactory::NYStylePizzaStore();
	AbstractFactory::Pizza* pizza = pizza_store->OrderPizza("cheese");		// �����൱�ڴ�����һ������
	std::cout << "A customer ordered a " << pizza->GetName() << std::endl;

	// ���������Դ
	delete pizza;
	delete pizza_store;
}

void SingletonPattern() {
	Singleton* instance = Singleton::GetInstance();
	instance->DoSomethin();
}

int main()
{
	//StrategyTest();
	//ObserverTest();
	//DecoratorTest();
	//FactoryMethodPattern();
	//AbstractFactoryPattern();
	SingletonPattern();
    return 0;
}

