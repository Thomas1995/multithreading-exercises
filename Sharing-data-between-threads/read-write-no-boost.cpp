#include <iostream>
#include <fstream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include <vector>
#include <time.h>
#include <algorithm>

std::condition_variable data_cond;
std::mutex mut;

std::atomic<int> readers;

void reader(int id) {
  std::unique_lock<std::mutex> lk(mut);

  std::cout<<id;
  ++readers;

  lk.unlock();

  std::this_thread::sleep_for(std::chrono::milliseconds(10));

  --readers;
  std::cout<<id;

  data_cond.notify_all();
}

void writer() {
  std::unique_lock<std::mutex> lk(mut);

  data_cond.wait(lk, [] {
    return readers == 0;
  });

  std::cout<<std::endl;
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout<<"Acest ";
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout<<"mesaj ";
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout<<"a ";
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout<<"fost ";
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout<<"scris.";
  std::this_thread::sleep_for(std::chrono::milliseconds(10));
  std::cout<<std::endl;
}

int main() {
  std::vector<std::thread> threads;

  srand(time(NULL));

  for(int i=0;i<10;++i)
    if( rand() % 2 )
      threads.push_back(std::thread(reader, i));
    else
      threads.push_back(std::thread(writer));

  for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

  return 0;
}
