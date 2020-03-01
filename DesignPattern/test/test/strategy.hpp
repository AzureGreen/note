#pragma once
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

