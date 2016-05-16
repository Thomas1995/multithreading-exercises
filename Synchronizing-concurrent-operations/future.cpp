#include <future>
#include <iostream>
#include <memory>

int recursive_fib(int n) {
    return (n<=2) ? 1 :
      recursive_fib(n-1) + recursive_fib(n-2);
}

int linear_fib(int n) {
    std::unique_ptr<int[]> fib(new int[n+1]);

    fib[1] = fib[2] = 1;
    for(int i=3;i<=n;++i)
      fib[i] = fib[i-1] + fib[i-2];

    return fib[n];
}

int main() {
    int n;
    std::cin>>n;

    // a new thread will start
    std::future<int> answer = std::async(std::launch::async, recursive_fib, n);

    // this task will not start right now
    // get_future must be called in order to avoid destructor throwing an exception
    std::packaged_task<int(int)> task(linear_fib);
    std::future<int> answer2 = task.get_future();

    // current thread waits 1 second for the recursive method
    // if it is too slow, it calls the packaged_task
    // however, the program will terminate when the async method is done
    // even if all the actions on this thread are done
    if(answer.wait_for(std::chrono::seconds(1)) == std::future_status::ready) {
      std::cout<<answer.get();
    }
    else {
      std::cout<<"Time limit exceeded with recursive method. Trying the linear one...\n";
      task(n);
      std::cout<<answer2.get();
    }

    std::cout<<std::endl;

    return 0;
}
