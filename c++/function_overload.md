# 函数重载
## 定义
在同一作用域内具有**相同名称**但又有**不同形参列表**的函数称为**`重载函数`**。重载函数的调用过程中会有**`函数匹配`**，编译器根据实参与形参列表比较决定调用哪个函数。
```c++
int Add(int a, int b) {
    return a + b;
}

double Add(double a, double b) {
    return a + b;
}

int main() {
    auto v1 = Add(1, 2);    // 调用第一个
    auto v2 = Add(1.0, 2.0);    // 调用第二个
    return 0;
}
```
## 注意
- 重载函数必须具有不同的形参列表，仅有返回值不同不行
- 常量与非常量的指针或引用作用形参，可以作为重载函数，其他的常量与非常量的形参只能算重复声明
```c++
int Add(int a, int b);
int Add(const int a, const int b);  // 重复声明 int Add(int a, int b)

int Add(int& a, int& b);    // 作用于int的引用
int Add(const int& a, const int& b);    // 作用于常量的引用
int Add(int* a, int* b);    // 作用于指向int的指针
int Add(const int* a, const int* b);    // 作用于指向常量的指针
```
- 函数重载是在**同一作用域**下的产生的，不同作用域下会隐藏声明的同名实体，因此某一作用域下无法重载另一作用域里的函数。
```c++
void Print(int p) {
    std::cout << "int " << p << std::endl;
}
void Print(std::string p) {
    std::cout << "string " << p << std::endl;
}
void Print(double p) {
    std::cout << "double " << p << std::endl;
}

int main() {
    void Print(double p);
    Print(1);   // 通过，调用Print(double)，因为Print(int)被隐藏了
    Print(1.0); // 通过，调用Print(double)
    // Print("HelloWorld");    // 报错，因为 Print(std::string) 被隐藏了
    return 0;
}
```
## 遇到的问题
同样，在类继承中也有相同的情况，**子类的作用域嵌套在基类的作用域内**，如果子类有同名函数将隐藏基类的同名函数

```c++
class Base {
public:
    void sub(int p1) {
        std::cout << "Base int" << std::endl;
    }

    virtual void sub(std::string s1) {
        std::cout << "Base string" << std::endl;
    }

    void sub(void) {
        std::cout << "Base void" << std::endl;
    }

};

class Child1: public Base {
public:
    void sub(std::string s1) {
        std::cout << "child1 string" << std::endl;
    }
};

class Child2: public Base {
public:
    void sub(void) {
        std::cout << "child2 void" << std::endl;
    }
};

int main() {
    Child1 c1;
    c1.sub("hello");	// 通过，调用子类 sub(string s1)
    // c1.sub(1);	// 报错，因为基类sub函数被隐藏，子类没有匹配函数
    c1.Base::sub(1);    // 通过，显示调用基类sub方法

    Child2 c2;
    c2.sub();	// 通过，调用子类 sub(void)
    // c2.sub(1);	// 报错，因为基类sub函数被隐藏
    c2.Base::sub(1);
    return 0;
}
```

这个问题可以通过在子类中声明父类的所有重载，比较好的方法是通过**using**声明语句指定函数名称（不带形参列表），即可将基类的所有重载实例添加到子类作用域中（有private方法行不通）。
```c++
class Base {
public:
    void sub(int p1) {
        std::cout << "Base int" << std::endl;
    }

    virtual void sub(std::string s1) {
        std::cout << "Base string" << std::endl;
    }

    void sub(void) {
        std::cout << "Base void" << std::endl;
    }

};

class Child1: public Base {
public:
    using Base::sub;	// 将基类重载函数加入自己作用域
    void sub(std::string s1) {
        std::cout << "child1 string" << std::endl;
    }
};

int main() {
    Child1 c1;
    c1.sub("hello");	// 调用子类sub(string)
    c1.sub(1);	// 调用基类sub(int)
    c1.sub();    // 调用基类sub(void)
    return 0;
}
```