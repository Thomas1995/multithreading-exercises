#include <iostream>
#include <mutex>
#include <thread>
#include <vector>
#include <algorithm>

std::once_flag connection_init_flag;

void initialize() {
    std::cout<<"Initialization"<<std::endl;
}

void do_something() {
    // Initialization will occur only once
    std::call_once(connection_init_flag, initialize);
    std::cout<<"Do something\n";
}

int main() {
    std::vector<std::thread> threads;

    // hardware_concurrency can return 0
    int threadsNumber = std::max(2u, std::thread::hardware_concurrency());

    for(int i=1;i<=threadsNumber;++i)
      threads.push_back(std::thread(do_something));

    std::for_each(threads.begin(), threads.end(), std::mem_fn(&std::thread::join));

    return 0;
}
