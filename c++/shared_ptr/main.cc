
#include <cstddef>
#include <iostream>

// here do not handle base & derived class situation.
template <typename T>
class MySharedPtr {
public:
	MySharedPtr() : p_(nullptr), reference_(nullptr) {}

	MySharedPtr(T *p) {
		if (p != nullptr) {
			p_ = p;
			reference_ = new size_t(1);
		}
	}

	MySharedPtr(MySharedPtr<T> &msp) {
		p_ = msp.p_;
		reference_ = msp.reference_;
		Increase();
	}

	MySharedPtr(MySharedPtr<T> &&msp) {
		p_ = msp.p_;
		reference_ = msp.reference_;
		msp.p_ = nullptr;
		msp.reference_ = nullptr;
	}

	~MySharedPtr() {
		Decrease();
	}
	
	MySharedPtr &operator=(T *p) {
		if (p_) Decrease();
		p_ = p;
		reference_ = new size_t(1);
		return *this;
	}

	MySharedPtr &operator=(MySharedPtr &msp) {
		if (this != &msp) {
			// if already reference to a pointer
			if (p_) Decrease();
			p_ = msp.p_;
			reference_ = msp.reference_;
			Increase();
		}
		return *this;
	}

	MySharedPtr &operator=(MySharedPtr &&msp) {
		if (this != &msp) {
			// if already reference to a pointer
			if (p_) Decrease();
			std::cout << "in" << std::endl;
			p_ = msp.p_;
			reference_ = msp.reference_;
			msp.p_ = nullptr;
			msp.reference_ = nullptr;
		}
		return *this;
	}

	operator bool() { return reference_ != nullptr; }

	T* operator->() { return p_; }

	T& operator*() { return *p_; }

	size_t use_count() { return *reference_; }

private:

	void Increase() {
		if (reference_) ++*reference_;
	}

	void Decrease() {
		if (reference_ && --*reference_ == 0) {
			delete p_;
			delete reference_;
			p_ = nullptr;
			reference_ = nullptr;
		}
	}

	T *p_;
	size_t *reference_;
};


int main() {

	MySharedPtr<int> msp1(new int(1));
	{
		MySharedPtr<int> msp2(msp1);	// 拷贝构造
		MySharedPtr<int> msp3;
		msp3 = msp2;					// 拷贝赋值
	}
	{
		MySharedPtr<char> msp4;
		msp4 = new char('a');		// 赋值
		MySharedPtr<char> msp5;	
		msp5 = MySharedPtr<char>(new char('b'));	// 移动赋值
	}
	{
		MySharedPtr<int> msp6;
		MySharedPtr<int> msp7(msp6);
		MySharedPtr<int> msp8 = msp7;
		MySharedPtr<int> msp9 = std::move(msp1);	// 移动构造
	}	

}