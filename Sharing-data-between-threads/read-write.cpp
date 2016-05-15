// run with -lboost_system -lboost_thread

#include <stdlib.h>
#include <time.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <thread>
#include <mutex>
#include <boost/thread/shared_mutex.hpp>

boost::shared_mutex _access;

void reader() {
    // reader can be accessed by multiple threads at the same time
    boost::shared_lock<boost::shared_mutex> lk(_access);

    std::ifstream file("data.in");
    int tmp, countIntegers = 0;

    while(file>>tmp)
      ++countIntegers;

    file.close();
}

void writer(int x) {
    // blocks access for both reader and writer
    std::lock_guard<boost::shared_mutex> lk(_access);

    std::ofstream file("data.in", std::ios_base::app);

    file<<x<<' ';

    file.close();
}

int main() {
    std::vector<std::thread> threads;

    srand(time(NULL));

    for(int i=1;i<=100;++i)
      if( rand() % 2 )
        threads.push_back(std::thread(reader));
      else
        threads.push_back(std::thread(writer, i));

    for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return 0;
}
