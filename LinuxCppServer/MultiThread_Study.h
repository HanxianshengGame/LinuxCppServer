//
// Created by 15870 on 2020/7/6.
// This

#ifndef LINUXCPPSERVER_LINUXCPPSERVER_MULTITHREAD_STUDY_H_
#define LINUXCPPSERVER_LINUXCPPSERVER_MULTITHREAD_STUDY_H_
#include <atomic>
#include <mutex>
#include <iostream>
#include <thread>
//第一关: 线程的基本使用（对问题的拆分合并）
namespace mynamespace1 {

template <typename Iter ,typename  Fun>
//使用函数处理序列并返回结果
inline double GetOperatorResult(Iter beg, Iter end, Fun func);
inline double HandleData(double data);

//分割一个容器序列并进行双线程与单线程的时间比较
inline void DivideOperation();

}

//第二关：线程问题概述
namespace mynamespace2 {


//程序运行时，机器推荐可以开启的线程数量
inline
unsigned int GetThreadCountFromHardWare();

//多线程编程资源竞争问题：
//变量的改变通过：写入寄存器，改变其值，返回到内存，
// 如果一个线程将变量值写入寄存器，准备改变时，另一个线程也将变量值写入寄存器，
// 再继续更改其值时，彼此线程之间改变的值不能都应用到其变量上，数据改变再返回到内存，会导致一个线程的操作失效的“效果”。
class OperatorCounter {
 public:
  OperatorCounter(): count_(0) { }

  unsigned int count() const { return count_; }
  void add_count() { count_++; }
 private:
   int count_;
};
inline
void ResourceCompetitionCase();

//解决资源竞争
//1：不共享资源，每个分割的任务进行独立分配其资源，最终整合一起
inline
void SolveResourceCompetition1();
//2.共享单一数据：原子操作 保证其数据操作过程中的完整性
//变量的改变通过：写入寄存器，改变其值，返回到内存。 原子操作保证其三个步骤绑定执行，从而保证数据的安全


/* 原子操作：头文件 <atomic>
 * 我们对其数据进行设置原子操作即可：
 *  std::atomic<int> count_;
 *  std::atomic_int count_;
 */
class NewOperatorCounter {
 public:
  NewOperatorCounter(): count_(0) { }

  unsigned int count() const { return count_; }
  void add_count() { count_++; }
 private:
  std::atomic<int> count_;
};

inline
void SolveResourceCompetition2();
//原子操作之后仍存有的缺陷：只能保证数据自身 的原子操作，但对于代码的有序性仍存在缺陷，不能被保证。
//例如 a，b，其都能保证其原子操作，但是当 b 依赖于 a 的值，当 a 进行改变后，下一步b会根据a 的改变改变自身
//但因为a改变后切换线程，a再次被改变的话，此时b的值就有问题。

// 3.mutex ：互斥锁,保证其共享数据的完整性（区块的原子性），且同一时间只能有一个线程访问到该锁的区块（加锁与开锁成对组成的块）
//利用 mutex 能保证代码的顺序执行，
//lock_guard 模板类管理 mutex 且是异常安全的
class NewNewOperatorCounter {

 public:
  NewNewOperatorCounter(): count_(0) { }

  unsigned int count() const { return count_; }
  void add_count() { count_++; }

  void Func1() {
    int i = 1000;
    while(--i) {
      std::lock(mutex1_,mutex2_);
      std::lock_guard<std::mutex> lock1(mutex1_, std::adopt_lock); //该线程将 1 上锁
      std::lock_guard<std::mutex> lock2(mutex2_,  std::adopt_lock); //该线程将 2 上锁
      std::cout << i << std::endl;
    }

  }
  void Func2() {
    int i = 1000;
    while(--i) {
      std::lock(mutex1_,mutex2_);
      std::lock_guard<std::mutex> lock1(mutex2_, std::adopt_lock); //该线程将 2 上锁
      std::lock_guard<std::mutex> lock2(mutex1_, std::adopt_lock); //该线程将 1 上锁

    }

  }
 private:
  int count_;
  mutable std::mutex mutex1_;
  mutable std::mutex mutex2_;
};
//死锁的演示
void DeadlockDemo();

}

//实现一个安全析构的 thread
//thread 的标准行为是如果不指出由系统或编程人员自动管理，会直接调用 abort(errorcode)造成程序崩溃
//@1. 生成线程，搁置不顾，系统自动管理 : thread.detach() 调用后其线程管理就与我们无关了，--
//@2. 生成线程，我们主动管理 ：使用 thread.join() 由我们自动管理其完成事件后析构。

// 当我们主动管理时，异常发生时，即使捕获了异常，也不能保证将线程的管理权及时交付到我们手中，我们利用
// 类的析构函数在程序异常时也能安全执行这一特点对 thread 进行一次封装

class MyThread {
 public:
  explicit MyThread(std::thread &t) :thread_(t)  {}
  ~MyThread()  { if(thread_.joinable()) thread_.join(); }
 private:
  std::thread &thread_;
};



//第三关： 线程间同步与通信

namespace mynamespace3 {
//1、全局变量作为信号量
//进程中的线程间内存共享，这是比较常用的通信方式和交互方式。
//注：定义全局变量时最好使用volatile来定义，以防编译器对此变量进行优化。




// 使用 条件变量 condition_variable 执行线程间通信，全通知
//其中 signal作为等待的全局变量，
// condition_var.wait(unique_lock);使得当前线程处于阻塞状态，并交出所有锁住的资源权，当被通知时会继续锁住资源
// conditon_var.notify_all() ；全线程唤醒。
void Worker(int id);
void GoWork();

void InterThreadCommunicationCase();
//  condition_var.wait(lck,Pre)：使得当前线程处于阻塞状态，直到谓词等于 true，在阻塞时交出资源的使用权
// condition_variable.notify_one(): 通知单个线程被唤醒
// 生产者生成10个物品，消费者逐个消费其物品, 物品消费完毕的标志：cargo = 0, 仍在消费shipment_available() == true
bool shipment_available();

void consume (int n) ;
void MainProducerThread();
}


#endif //LINUXCPPSERVER_LINUXCPPSERVER_MULTITHREAD_STUDY_H_
