#include <future>
#include <iostream>

int recursive_fib(int n) {
    return (n<=2) ? 1 :
      recursive_fib(n-1) + recursive_fib(n-2);
}

int main() {
    int n;
    std::cin>>n;

    std::future<int> answer = std::async(std::launch::async, recursive_fib, n);

    if(answer.wait_for(std::chrono::seconds(1)) == std::future_status::ready)
      std::cout<<answer.get();
    else
      std::cout<<"Time limit exceeded";

    std::cout<<std::endl;

    return 0;
}
