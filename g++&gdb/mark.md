# gdb调试常用命令记载

## 1.带参数调试
**set args** + 要传入的参数

```gdb
set args --algorithm="LRU" --capacity=20
```

## 2.执行
**run**：简写为r，启动程序，遇到断点才停下。`相当于vs的F5启动调试`
**continue**：简写为c，继续运行程序直到遇到断点或者程序结束。`相当于vs的F5`
**next**：简写为n，单步跟踪，不进入函数体。`相当于vs的F10`
**step**：简写为s，单步调试，会进入自定义函数体。`相当于vs的F11`
**until**：运行直到退出循环体。
**finish**：运行程序，知道当前函数完成返回，并打印函数返回时的堆栈地址、返回值和参数值等。
**call**：调用程序中可见的函数
**quit**：简写为q，退出gdb


## 3. 断点管理
### 3.1下断点
**break**或者**tbreak**用来在指定行、指定文件的行、指定函数、指定文件的函数下断点，其中tbreak更像设置一个临时断点，只停留一次后自动删除断点，break需要手动删除。

```gdb
break [linenum] or [filename:linenum] or [function] or [filename:function] or [*address] | [condition]

eg:
break 10
break test.cc:10
break main
break test.cc:main
break *0x80484624
break 10 if index == 3
```
### 3.2查看断点信息
**info b**（info breakpoint）：显示当前程序的断点设置情况

### 3.3删除断点
**delete**和**clear**可以用来删除断点。

```gdb
delete n		删除第n个断点
delete breakpoints	删除所有断点
clear n		清除第n行的断点
```

### 3.4暂停断点
**disable**可以暂停某个断点
```gdb
disable n	暂停第n个断点
```
## 4.查看内存
**x**指令用来查看内存 ，具体用法如下：
```gdb
x/<n/f/u> <addr>  nfu为可选参数 addr为内存地址
```
- n：表示要显示的内存单元个数
- f：显示格式：
	- x：十六进制
	- d：十进制
	- u：十六进制，无符号整型
	- o：八进制
	- t：二进制
	- a：十六进制，还会显示内容
	- c：字符格式
	- s：字符串格式
	- f：浮点格式
- u：表示从当前地址往后请求的字节数，，默认4字节
	- b：一字节 byte
	- h：两字节 word
	- w：四字节 dword
	- g：八字节 qword

```gdb
x\4xg 0x7fffffffe0f0	显示当前位置后的四个单元内存，每个单元大小为八字节，以十六进制显示
0x7fffffffe0f0: 0x00007ffff7dcb410      0x0000000000000000
0x7fffffffe100: 0x00007ffff7dcb318      0x000055555576b0a0 
```
## 5.调试release版本
release版本会进行优化，如果需要调试release版本，需要在编译选项上做修改，添加上**`-g -O2`**就有了符号表，可以进行调试。

## 6.gdb+pstack，定位死锁调试

1. 多次尝试 pstack + 死锁进程id，找到一直处于等锁状态的线程，一般会有两个线程一直没有变化，且处于等锁状态（重入？只有一个？）
2. gdb attach 死锁进程，然后 info thread，查看线程状态，找到哪个线程在等锁
3. thread + 线程序号，切入到线程，where 查看函数调用堆栈
4. 找到去调用锁的函数，frame + 帧号查看传参，是哪个互斥体，
5. p 互斥体变量，查看已拥有该互斥体的owner是谁，则可发现死锁的另一个线程是谁。
6. 同样的对另一个死锁线程操作，查看因那两个锁因资源抢占互斥导致了死锁。

> 参看 [linux 上分析死锁的简单方法](https://blog.csdn.net/horstlinux/article/details/7857068?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task)
>
> 以及 [gdb调试多线程 如何解死锁问题](https://www.jianshu.com/p/0b0be78e3e1c)

