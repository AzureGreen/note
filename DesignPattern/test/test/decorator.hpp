#pragma once
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