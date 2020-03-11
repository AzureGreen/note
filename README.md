# 浏览常见问题整理

## cpp

1. 为什么空类大小为1?

   编译器插进去了一个char，保证还class实例化的对象配置独立的地址

2. 内存对齐

   计算机系统对基本类型数据在内存中存放有位置限制，会要求其数据首地址为4/8的倍数。因为处理器会以双字节（4/8字节）作为存取粒度读取内存。
   
   > pragma pack(n)可以设置对齐粒度，参考：[C/C++内存对齐详解](https://zhuanlan.zhihu.com/p/30007037)

3. 大端和小端

   大端：高位字节在前，低位在后。除计算机系统，其他场景使用大端，譬如网络。

   小端：低位直接在前，高位在后。计算机从低位读取效率更高，故计算机系统采用小端。

   ```
   给定数值：0x1234567
   内存中大端：01|23|45|67
   内存中小端：67|45|23|01
   ```

   网络API：`htons`以及`ntohs`完成网络字节序与本地字节序的转换。

   > 参考：[理解字节序](https://www.ruanyifeng.com/blog/2016/11/byte-order.html)

4. 多态，虚函数，虚表

   > 参考：[c++/虚函数](c++/c++.md#2.虚函数)

5. 智能指针，循环引用，解决方案

   > 参考：[c++/智能指针](c++/c++.md#3.智能指针)

6. new和malloc区别

   > 参考：[c++/new和malloc](c++/c++.md#new和malloc)

7. operator new和placement new区别

   > 参考：[operator new和placement new](c++/c++.md/#operator new和placement new)
   
8. 程序编译过程

   - 预编译：展开头文件，宏等
   - 编译：翻译程序为汇编代码（这里会优化代码）
   - 汇编：翻译汇编代码成机器指令，生成目标文件
   - 链接：把目标文件和库文件链接成可执行文件（填充引用符号链接的地址）

9. 死锁的调试

   > 参考：[gdb+pstack，定位死锁调试](g++&gdb/#6.gdb+pstack，定位死锁调试)

## 数据库

1. b树，b+树

    为什么数据库，文件系统使用b树，b+树，不同平衡树？

    - 文件系统等需要频繁读写磁盘，树的高度越深，需要读写磁盘的次数越多，相比于查找速度更快的平衡树，b树的高度更低，磁盘读写所需时间开销更小
    - 磁盘预读特性，磁盘预读过程会顺序读取，一次IO读取一个节点，邻近节点也会被顺序预先载入。

    为什么大部分使用b+树？

    - b+树非叶子节点不做数据存储，仅做索引，所有数据都存放在叶子节点，b树每个索引节点都会有数据域，为了把数据读取出来，在每层遍历时会增大磁盘IO次数，影响性能。
    - b+树的数据域都在叶子节点，而且通过一个链表指针连接起来，这样可以方便区间访问，适用于**范围查询**频繁使用的数据库。

2. 索引

   数据库索引：数据库管理系统中一个排序的数据结构，协助快速查询，更新数据库表中数据。

   索引的好处？

   - 加快查找速度，不用全表扫描

   - 将随机IO变成顺序IO（b+树索引有序，将相邻的数据存储在一起）

   聚集索引？

   - 索引中键值的逻辑顺序决定了表中相应行的物理顺序。
   - 由于聚集索引规定数据在表中的物理存储顺序，因此一个表只能包含一个聚集索引。

   非聚集索引

   - 索引中的索引逻辑顺序与在磁盘上行的物理存储顺序不同。

3. 事务的特点

   事务：一个数据库操作序列
   
   ACID：
   
   	- 原子性：对数据库的操作要么全部成功提交要么全部失败回滚；
   	- 一致性：事务执行结果一定是从一个一致性状态到另一个一致性状态；
   	- 隔离性：一个事务执行，不能被其他事务干扰；
   	- 持久性：事务成功提交将被写入数据库永久保存。


4. 一致性问题

   - 丢失修改：两个事务A,B，A修改数据，B也修改数据，A的修改被B的提交覆盖；
   - 脏读：事务A,B，A更新了数据，B读取了数据，然后A撤销了操作，B读到了不一致的值；
   - 不可重读：事务A,B，A读了某个数据，B修改了该数据，A再次读该数据，发现数据不一致；
   - 幻影读：事务A,B，A读取了某个范围内的数据，B在该范围内插入了一条数据，A再次读取该范围内的数据，发现不一致。

5. 隔离级别 各自实现 优缺点

   - 未提交读：事务未提交前的操作对其他事务是可见的；不加读锁，加行级写锁，事物结束解锁；会造成脏读；
   - 提交读：事务提交的操作对其他事务才可见；加行级读锁，读完即解锁，加行级写锁，事务结束解锁；会造成不可重复读；
   - 可重复读：保证事务在多次读取同一数据的结果一致；加行级读锁，加行级写锁，事务结束解锁；会造成幻影读；
   - 可串行化：强制事务串行执行；加表级读锁，加表级写锁，事务结束解锁；开销大，效率低；

6. MVCC和next-key-locks

   - `MVCC`多版本控制，针对的是数据**行**，事务对数据行的操作会通过链表机制记录在`undo`日志中，针对读操作直接采取`快照读`，避免加锁，针对修改操作用`当前读`，需要加锁。

     > 快照读：读取旧的快照，或者事务自己的新快照，不用加锁，select使用。
     >
     > 当前读：会读取最新的快照记录，然后将数据加锁，写操作使用。
     >
     > 参考：[当前读与快照读](https://www.jianshu.com/p/27352449bcc0)

   - next-key-locks是innodb的锁实现，它不仅锁定一个记录上的索引，同时**锁定索引之间的间隙**。

   -  在可重复读（REPEATABLE READ，mysql默认隔离级别）隔离级别下，使用 MVCC + Next-Key Locks 可以解决幻读问题 

7. 悲观锁与乐观锁

   悲观锁：强制级别较高，只要对临界数据操作就上锁了，开销大；适用于多写场景。

   乐观锁：总是乐观觉得不会冲突，对读操作不上锁，而是通过版本控制，当发现版本序号一致才允许更新操作，否则重试。(MVCC就像是个乐观锁机制)，适用于多读场景。

   > 参考：[面试必备之乐观锁与悲观锁](https://juejin.im/post/5b4977ae5188251b146b2fc8)

8. inodb myisam

   

9. 数据库优化的常规流程







## 网络
1. tcp udp区别

   突出tcp面向连接的，可靠的服务，以及为了实现可靠的一些机制，相应udp是无连接，不可靠，最大可能交付的服务

   [TCP与UDP区别](network/network.md#TCP与UDP区别)

2. tcp可靠机制

   超时重传：超时时间内为收到确认，则重传报文

   滑动窗口，流量控制：控制发送方的发送速率，保证接收方接的过来

   拥塞控制：控制发送方的发送速率，保证网络可以承载那么多数据

   [CS-Notes/notes/计算机网络-传输层](https://github.com/CyC2018/CS-Notes/blob/master/notes/计算机网络 - 传输层.md#tcp-可靠传输)

3. 浏览器访问网页发生了什么

   [终极之问](network/network.md#终极之问)

4. https相关问题

   [HTTPS相关](network/network.md#HTTPS相关)

5. put与post的区别，get与post的区别

   1. put与post

      - put是幂等的，多次执行结果一样，也就是后面请求会覆盖前面的请求

      - post不是幂等的，

   2. get与post
      - 参数：get通过url传参，只能是ASCII；post在body中，支持标准字符集；
      - 可缓存：get请求会被浏览器主动cache；post需要手动设置才cache；
      - 安全：get是只读请求，不改变服务器状态，安全；post会传输实体，可能修改数据库，因此服务器状态改变，不安全；
      - 幂等性：get是；post不是

6. UDP怎么构建可靠通信

   > 就需要参考TCP的可靠机制，在传输层上面，或是应用层实现可靠传输机制。
   >
   > 超时重传：定时器
   >
   > 有序接受：添加包序号
   >
   > 应答确认：实现seq/ack应答机制
   >
   > 滑动窗口等流量控制机制

   参考QUIC

7. http缓存机制

   参考下面的缓存部分，以及终极之问

8. tcp粘包

   原因：发送方
   
9. tcp心跳机制

   

## 操作系统

1. 进程和线程的联系与区别

   - 联系

     一个程序至少有一个进程，一个进程至少有一个线程。

     进程是系统资源分配与调度的独立单位，拥有完备的数据空间，代码空间，虚拟地址空间。

     线程是CPU调度的基本单位，是进程的一个实体，与所属进程的其他线程共享该进程的全部代码空间与全局变量，但线程拥有独立的堆栈。

   - 区别

     调度：进程是资源分配的基本单位，而线程是调度和分派的基本单位，在一个进程中，线程的切换不会引起进程切换，而从一个进程中的线程切换到另一个进程中的线程，会引起进程切换。

     系统开销：进程创建与撤销，系统都需要为之分配和回收资源，导致系统的开销远大于线程的开销；同样的，进程切换的开销也比线程切换开销大，线程切换仅涉及保存设置少量寄存器内容。

     资源管理：进程有独立的地址空间，一个进程挂掉，在保护模式下，不会影响其他进程，而一个线程挂掉，整个进程也都挂掉，因为线程没有独立的地址空间。

2. 用户级线程与内核支持线程

   - 内核支持：内核支持线程对操作系统内核是感知的，用户及线程对操作系统内核不感知
   - 处理器分配：对用户级线程，内核一次只为一个进程分配一个处理器，而内核支持线程，内核可调度一个应用的多个线程同时在多个处理器上并行运行；
   - 调度和线程执行时间：内核支持线程调度方式与进程类似，调度单位是线程，而用户级线程，调度单位仍旧是进程；
   - 中断：用户级线程通过系统调用导致进程被中断，而内核支持线程执行系统调用仅导致该线程中断；

   > 简单说用户级线程就是用户层的线程库创建的，而内核支持线程通过系统调用，有操作系统创建的线程

3. 多进程和多线程的使用场景

   多线程：占用内存少，CPU利用率高，创建销毁切换开销小，速度快，但是同步复杂，且一个线程出问题会导致整个进程挂掉。

   多进程：共享数据复杂，需要IPC，但是同步简单（与多线程相反），多进程调试简单，且进程间出问题互不影响。

   多进程：适合多核多机分布；多线程：适合多核分布式。

4. 进程通信

   管道，有名管道，信号量，消息队列，信号，共享内存，套接字

   > 共享内存是最快IPC，不用系统调用或者切入内核态，同时避免了不必要的数据复制

5. 线程通信（主要用于同步）

   锁机制：互斥锁，条件变量，读写锁

   信号量机制

   信号机制

6. 线程调度与进程调度算法一样：

   FCFS，短作业优先，时间片轮转，优先级队列，多级反馈队列

7. 线程生命周期

   线程从创建便开始了生命周期，直到终止，会经历阻塞，就绪，运行状态。

8.  线程的私有数据有哪些，公有有哪些

   - 私有数据：
   - 算法

   此外，

9. 进程fork子进程，内存占用情况(写时复制)

   一开始子进程与父进程是共享资源的，只有当子进程需要修改资源，才拷贝那一部分到自己进程空间。

10. 进程的内存映像

    进程控制块PCB，代码段，数据段，堆栈

11. 程序的加载过程

    - 内核将程序读入内存，为程序分配内存空间
    - 内核保存该进程的PID及状态信息，加入运行队列等待调度执行。

    > 参考[Linux下的进程2——进程的内存映像，进程PCB，task_struct详解](https://blog.csdn.net/lixungogogo/article/details/52210750)

    1. 创建独立虚拟地址空间，从虚拟内存到物理内存的映射
    2. 读取可执行文件头，简历可执行文件各个段与虚拟内存中各个段的映射
    3. 将CPU指令寄存器设置为当前可执行文件的入口

    > 参考：[LINUX内核研究----深入程序加载执行过程](https://blog.csdn.net/RUN32875094/article/details/79329517)

12. 虚拟内存

    内存管理技术，通过一个逻辑转换将可能隔断不连续的物理地址映射成一块连续的地址空间，让应用程序认为其拥有连续可用的内存。

    虚拟地址转物理地址：分段和分页，由MMU完成。虚拟内存空间与物理内存空间按相同大小页面分割映射，发生缺页时通过页面置换算法更新页表。

## io

非阻塞io与阻塞io



## 缓存
- http请求头和响应头跟缓存有关的字段

  - **强缓存**：Expires，Cache-control

  1. Expires：GMT时间，告知浏览器在此期间内，可信任并使用该文件缓存，即浏览器下次请求时间只要小于该时间，可直接从缓存中获取数据，不再向服务器请求。
  2. Cache-control：private，public，no-cache，max-age，no-store
     - private：客户端可缓存
     - public：客户端和代理服务器均可缓存
     - max-age：缓存存储的最大周期，http/1.1起取代Expires
     - no-cache：每次请求浏览器需向服务器发送请求，让服务器判断缓存的有效性
     - no-store：不缓存

  - **对比缓存**：Last-Modified，Etag
  1. Last-Modified：GMT时间，告知浏览器资源的最后修改时间
  
     - If-Modified-Since：比较资源最后更新时间是否一致 （304）
     - If-Unmodified-Since：比较资源最后更新时间是否不一致 （412）
  
  2. Etag：文件实体的签名，资源唯一标识符，文件发生变化，Etag值也会变化
  
     - If-Match：比较Etag是否一致 （304）
     - If-None-Match：比较Etag是否不一致  （412）
  
     > Etag比Last-Modified更加严格
     >
     > Last-Modified检测粒度是秒级
     >
     > 文件被修改，但内容并未修改，Last-Modified会改变，但Etag并不会。
     
     对比缓存的定义：服务器对比判断文件是否修改，告诉浏览器是否可以使用本地缓存，如果可以使用，服务器会返回浏览器304，否则完成一次正常的200请求。
  
- 浏览器缓存的工作流程

  1. 判断浏览器是否缓存，若没有则向服务器请求资源，若有走2；
  2. 判断缓存是否有效（强缓存），若有则使用本地资源，否则走3；
  3. 向服务器请求检测资源是否更新，若没有则仅返回304，浏览器使用本地资源，否则服务器返回200并返回请求资源。

  > 参考：[彻底理解浏览器的缓存机制](https://juejin.im/entry/5ad86c16f265da505a77dca4)

- 缓存访问场景

  - 缓存穿透

    查询一个一定不存在的数据，导致缓存一定产生缺失，从而会向后端数据库查询，从而缓存失效。如果是恶意攻击，将会导致流量过大，后台数据库扛不住，最后崩溃。

    1. 通过布隆过滤器，把所有可能存在的数据全保存起来，这样就可以过来不存在的访问。
    2. 另一种简单方法是，直接将该次请求保存在缓存中，对应value为null，过期时间设短点。

  - 缓存雪崩

    设置缓存过期时间相同，导致缓存在某一时刻有大量文件同时失效，因而请求将全部转发到后端数据库上，导致数据库瞬间访问压力过大从而雪崩。

    1. 简单的处理就是设置缓存失效时间分散开，设置过期时间加上一个随机数。

  - 缓存击穿

    同时有大量请求同一个文件，但是该文件刚好缺失或失效了，这时大量请求都会去走数据库请求，从而导致数据库又扛不住了，崩溃。

    1. 针对这个问题，采用互斥锁解决。因为这类请求是多线程请求同一个资源，因为第1个线程请求时，加锁，等第一个线程从数据库拉取回数据，缓存下来后，解锁，这样其他线程的请求就可以直接读缓存了。

  > 参考：[关于【缓存穿透、缓存击穿、缓存雪崩、热点数据失效】问题的解决方案](https://juejin.im/post/5c9a67ac6fb9a070cb24bf34)

- 数据库缓存

  > 推荐：[响应速度不给力？解锁正确缓存姿势](https://mp.weixin.qq.com/s/Qjb39osqW4__tTh_7jZ6EA)

  1. 为什么不是更新缓存而是失效缓存

     - 并发写容易写覆盖造成脏数据问题
     - 双写不同数据源容易造成数据不一致（物理失败，更新操作失败）
     - 更新缓存可能有不必要的计算开销

  2. 缓存与数据库更新顺序

     - 先删除缓存，再更新数据库：并发状态下可能产生数据不一致

     > A写请求先失效缓存，B读请求发现缓存缺失，向数据库拉取数据保存缓存，A再更新数据库，这样的时序导致缓存与数据库不一致
     >
     > 1. 延时双删：在更新数据库后睡眠一小段时间，等可能的操作都结束，然后再删除缓存（还可以异步双删）
     > 2. 设置过期时间，到期失效再载入最新数据。需要系统容忍一段时间数据不一致

     - 先更新数据库，再删除缓存：（推荐）也会在并发状态下产生数据不一致

     > A读请求，发现缓存缺失，读取数据库返回，然后B写请求，先更新数据库，再删除缓存，此时A再更新缓存，这时候更新的是旧的数据。（这场景感觉不管什么顺序都会出问题）
     >
     > 不过上面发生的可能性很低，因为读的操作时间一般小于写操作

  3. 数据更新的方式

     1. Cache Aside Pattern：有关问题参见上面1和2
     2. Write/Read Through：数据库管理交由缓存层**同步**处理，调用方仅与缓存层交互
     3. Write Behind Cache：直接更新缓存数据，然后**异步**更新数据库。响应快，吞吐量大，但是一致性保障有难度，如果数据库更新失败需要较好的重试策略，还要考虑数据持久化问题。。

  4. 数据不一致性

     - 缓存与数据库两地的数据不一致。

     1. 逻辑失败导致：并发情况下的异步操作导致的不一致，解决办法是异步操作串行化
     2. 物理失败导致：因删除缓存失败导致的不一致，解决办法是重试

     - 解决方案

       逻辑失败：一般思路就是通过异步消息队列去同步删除缓存
     
       物理失败：不断重试删除缓存
       
       结合以上两个，就是不断将需要删除的缓存的key加入到消息队列中。
  
  

## 分布式

1. 负载均衡算法

   - 轮询

   - 随机

   - 源地址hash：每次服务的是用一台机器，可以保存session

   - 加权轮询

   - 加权随机

     > 两版加权算法考虑到了不同服务器的负载情况

   - 最小连接数

2. 一致性哈希算法

   构造一个一致性HASH环（`2^32`），将服务器节点分散在环上，请求被hash后，派发给对应环上负责该区间的服务器。

   - 扩容：将新加入的结点映射到Hash环上，这样仅影响到环中一小段。

     > 但是新加入的结点与受影响的一小段那个服务器结点将共同承担以前这一小段的负载，而其他结点仍旧承受以前那么多的负载，为新加入结点的两倍，导致负载并不均衡。

   - 虚拟节点：将物理结点映射成多个虚拟节点，然后将虚拟节点映射到一致性Hash环上，请求首先找到虚拟节点，然后再得到物理结点信息，而且各个节点很分散均匀的瓜分了请求，这样新加入结点，也会被分散在Hash环的各个部分。

   > 参考：《大型网站技术架构》

3. 分布式锁

   - 基于数据库的实现

     在数据库中建一个表，表中包含方法名等，且方法名字段创建**唯一索引**。如果想要执行某个方法，则将该方法名插入数据库表中，成功插入表明获取到锁，执行结束删除记录，即为释放锁。其他想要获取锁会插入不成功。

   - 基于redis实现
   - 基于Zookeeper实现

   > 参考：[再有人问你分布式锁，这篇文章扔给他](https://juejin.im/post/5bbb0d8df265da0abd3533a5)



## 服务 
rpc



### 设计

 https://blog.csdn.net/qq_35190492/article/details/103105780?depth_1-utm_source=distribute.pc_relevant.none-task&utm_source=distribute.pc_relevant.none-task 