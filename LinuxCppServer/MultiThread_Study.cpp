// Authors: Handling
// Created by 15870 on 2020/7/15.
// This is MultiThread.cpp
#include "MultiThread_Study.h"
#include <algorithm>
#include <thread>
#include <vector>
#include <random>
#include <condition_variable>

namespace mynamespace1 {

template<typename Iter, typename Fun>
inline double GetOperatorResult(Iter beg, Iter end, Fun func) {
  double res = 0.0;
  for (; beg != end; ++beg) {
    res += func(*beg);
    std::this_thread::sleep_for(std::chrono::milliseconds(1));
  }

  return res;
}

inline double HandleData(double data) {
  return sqrt(data) + pow(data, 5);
}
inline void DivideOperation() {
  std::vector<double> dou_vec;
  dou_vec.reserve(10000);
  std::default_random_engine random_engine;
  std::uniform_real_distribution<double> real_distribution(1.0, 10000000.0);
  for (int i = 0; i < 10000; ++i) {
    dou_vec.push_back(real_distribution(random_engine));
  }

  auto beg = dou_vec.begin(), end = dou_vec.end();
  auto mid = dou_vec.begin() + dou_vec.size()/2;
  auto pre_sum = 0.0, back_sum = 0.0, thread_sum = 0.0;
  clock_t thread_beg_time = clock(), thread_end_time;

  std::thread t([&]() {
    pre_sum = GetOperatorResult(beg,mid,HandleData);
  });
  back_sum = GetOperatorResult(mid,end,HandleData);
  t.join();
  thread_sum = pre_sum + back_sum;
  thread_end_time = clock();
  std::cout << "thread Id: " << t.get_id() << " cost: " << thread_end_time - thread_beg_time << " result: "
            << thread_sum << std::endl;

  thread_beg_time = clock();
  thread_sum = GetOperatorResult(beg, end, HandleData);
  thread_end_time = clock();
  std::cout << "thread Id: " << std::this_thread::get_id() << " cost: " << thread_end_time - thread_beg_time << " result: "
            << thread_sum << std::endl;

}
}


namespace mynamespace2 {

inline
unsigned int GetThreadCountFromHardWare() {
  return std::thread::hardware_concurrency();
}

inline
void ResourceCompetitionCase() {
  //多线程编程在资源竞争（线程之间共享变量会出现很大的错误）
  std::vector<int> ivec;
  ivec.reserve(10000);
  std::default_random_engine random_engine;
  std::uniform_int_distribution<int> distribution(1, 10000);
  for (int i = 0; i != 100000; ++i) {
    ivec.push_back(distribution(random_engine));
  }

  int total_sum = 0;

  OperatorCounter main_counter;
  for (const auto &item : ivec) {
    total_sum += (item % 10);
    main_counter.add_count();
  }
  std::cout << "total_sum: " << total_sum
            << "access count: " << main_counter.count() << std::endl;

  total_sum = 0;
  OperatorCounter threads_count;
  using cIter = std::vector<int>::const_iterator;
  cIter cbeg = ivec.cbegin(), cend = ivec.cend();
  cIter iter1 = cbeg + ivec.size() / 3;
  cIter iter2 = cbeg + ivec.size() / 3 * 2;

  auto oper_sum_func = []
      (cIter cbeg, cIter cend, OperatorCounter &counter, int &total_sum) {
    for (; cbeg != cend; ++cbeg) {
      counter.add_count();
      total_sum += (*cbeg % 10);
    }
  };

  std::thread second_thread([&]() {
    oper_sum_func(cbeg, iter1, threads_count, total_sum);
  });
  std::thread third_thread([&]() {
    oper_sum_func(iter1, iter2, threads_count, total_sum);
  });

  oper_sum_func(iter2, cend, threads_count, total_sum);

  second_thread.join();
  third_thread.join();

  std::cout << "total_sum: " << total_sum
            << "access count: " << threads_count.count() << std::endl;

}

inline
void SolveResourceCompetition1() {
  //多线程编程在资源竞争（线程之间共享变量会出现很大的错误）
  std::vector<int> ivec;
  ivec.reserve(10000);
  std::default_random_engine random_engine;
  std::uniform_int_distribution<int> distribution(1, 10000);
  for (int i = 0; i != 100000; ++i) {
    ivec.push_back(distribution(random_engine));
  }

  int total_sum = 0;

  OperatorCounter main_counter;
  for (const auto &item : ivec) {
    total_sum += (item % 10);
    main_counter.add_count();
  }
  std::cout << "total_sum: " << total_sum
            << "access count: " << main_counter.count() << std::endl;

  total_sum = 0;
  OperatorCounter threads_count;
  using cIter = std::vector<int>::const_iterator;
  cIter cbeg = ivec.cbegin(), cend = ivec.cend();
  cIter iter1 = cbeg + ivec.size() / 3;
  cIter iter2 = cbeg + ivec.size() / 3 * 2;

  auto oper_sum_func = []
      (cIter cbeg, cIter cend, OperatorCounter &counter, int &total_sum) {
    for (; cbeg != cend; ++cbeg) {
      counter.add_count();
      total_sum += (*cbeg % 10);
    }
  };

  //对任务分线程片，一片一片使用其独立设定的资源，最后整合输出
  OperatorCounter thread_count1;
  int total_sum1 = 0;
  std::thread second_thread([&]() {
    oper_sum_func(cbeg, iter1, thread_count1, total_sum1);
  });

  OperatorCounter thread_count2;
  int total_sum2 = 0;
  std::thread third_thread([&]() {
    oper_sum_func(iter1, iter2, thread_count2, total_sum2);
  });

  oper_sum_func(iter2, cend, threads_count, total_sum);

  second_thread.join();
  third_thread.join();

  std::cout << "total_sum: " << total_sum + total_sum1 + total_sum2
            << "access count: " << threads_count.count() + thread_count1.count() + thread_count2.count()
            << std::endl;

}

inline
void SolveResourceCompetition2() {
  //多线程编程在资源竞争（线程之间共享变量会出现很大的错误）
  std::vector<int> ivec;
  ivec.reserve(10000);
  std::default_random_engine random_engine;
  std::uniform_int_distribution<int> distribution(1, 10000);
  for (int i = 0; i != 100000; ++i) {
    ivec.push_back(distribution(random_engine));
  }

  std::atomic<int> total_sum;     //原子操作的数据
  total_sum = 0;

  NewOperatorCounter main_counter;
  for (const auto &item : ivec) {
    total_sum += (item % 10);
    main_counter.add_count();
  }
  std::cout << "total_sum: " << total_sum
            << "access count: " << main_counter.count() << std::endl;

  total_sum = 0;
  NewOperatorCounter threads_count;
  using cIter = std::vector<int>::const_iterator;
  cIter cbeg = ivec.cbegin(), cend = ivec.cend();
  cIter iter1 = cbeg + ivec.size() / 3;
  cIter iter2 = cbeg + ivec.size() / 3 * 2;

  auto oper_sum_func = []
      (cIter cbeg, cIter cend, NewOperatorCounter &counter, std::atomic<int> &total_sum) {
    for (; cbeg != cend; ++cbeg) {
      counter.add_count();
      total_sum += (*cbeg % 10);
    }
  };

  std::thread second_thread([&]() {
    oper_sum_func(cbeg, iter1, threads_count, total_sum);
  });
  std::thread third_thread([&]() {
    oper_sum_func(iter1, iter2, threads_count, total_sum);
  });

  oper_sum_func(iter2, cend, threads_count, total_sum);

  second_thread.join();
  third_thread.join();

  std::cout << "total_sum: " << total_sum
            << "access count: " << threads_count.count() << std::endl;

 }
}

namespace mynamespace3 {

volatile std::atomic<bool> signal(false);
std::vector<int> ivec;
std::mutex m_mutex;
std::condition_variable condition_var;

void Worker(int id) {
  std::unique_lock<std::mutex> unique_lock(m_mutex);
  while(!signal) {  //等待信号量
    condition_var.wait(unique_lock);
  }
  std::cout << "thread " << id << std::endl;
}
void GoWork() {
  std::unique_lock<std::mutex> unique_lock(m_mutex);
  signal = true;
  condition_var.notify_one();
}

void InterThreadCommunicationCase() {
  auto kThreadCount = std::thread::hardware_concurrency(); //获取机器支持的线程数
  std::vector<std::thread> thread_container;
  for (int i = 0; i != kThreadCount; ++i) {
    thread_container.emplace_back(Worker, i);
  }

  GoWork();
  for (auto &thread : thread_container) {
    if(thread.joinable())
      thread.join();
  }
}
int cargo = 0;
bool shipment_available() {return cargo!=0;}

void consume (int n) {
  for (int i=0; i<n; ++i) {
    std::unique_lock<std::mutex> lck(m_mutex);
    condition_var.wait(lck,shipment_available);
    // consume:
    std::cout << cargo << '\n';
    cargo = 0;
  }
}
void MainProducerThread() {

  std::thread consume_thread(consume, 10);
  for (int i = 0; i != 10; ++i) {
    while(shipment_available())
      std::this_thread::yield();
    std::unique_lock<std::mutex> lck(m_mutex);
    cargo = i+1;
    condition_var.notify_one();
  }
  if(consume_thread.joinable())
    consume_thread.join();
}
}


