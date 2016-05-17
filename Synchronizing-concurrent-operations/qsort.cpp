#include <iostream>
#include <vector>
#include <stdlib.h>
#include <thread>
#include <atomic>

std::atomic<int> sortedElem;
std::atomic<int> availableThreads;

template <typename T>
void qsort(const T &begin, const T &end, const bool release = false) {
    if(begin == end) {
      availableThreads += release;
      return;
    }

    auto pivot = *begin;
    T poz = begin;
    for(T it = begin; it < end; ++it) {
        if(*it < pivot) {
            ++poz;
            std::swap(*poz, *it);
        }
    }
    std::swap(*begin, *poz);

    ++sortedElem;

    static auto launch = [] (const T &begin, const T &end) {
      if(availableThreads >= 1) {
          --availableThreads;
          std::thread t(qsort<T>, begin, end, true);
          t.detach();
      }
      else {
        qsort(begin, end);
      }
    };

    launch(begin, poz);
    launch(poz+1, end);

    availableThreads += release;
}

int main() {

    srand(time(NULL));

    availableThreads = std::thread::hardware_concurrency()-1;

    int n = 1e7;
    std::vector<int> v(n);
    for(auto &x : v)
      x = rand();

    qsort(v.begin(), v.end());

    while(sortedElem < v.size())
        std::this_thread::sleep_for(std::chrono::milliseconds(100));

    return 0;
}
