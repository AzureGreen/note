#pragma once

#include <iostream>
#include <mutex>

class Singleton {
public:
	static Singleton* GetInstance() {
		if (unique_instance_ == nullptr) {
			// иокЬ
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