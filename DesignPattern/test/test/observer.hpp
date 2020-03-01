#pragma once
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