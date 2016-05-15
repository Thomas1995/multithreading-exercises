#include <condition_variable>
#include <thread>
#include <mutex>
#include <queue>
#include <iostream>

std::queue<int> thingsToDo;
std::mutex mut;
std::condition_variable data_cond;

int solvedTasks;

void solveTask() {
    // lock_guard will not work with wait because it cannot be unlocked
    std::unique_lock<std::mutex> lk(mut);

    // if there is nothing to do (thingsToDo is empty), the mutex is unlocked
    // the thread is blocked and waits a notify to check again
    // when the lambda returns true, it locks the mutex and continue its task
    data_cond.wait(lk, [] {
      return !thingsToDo.empty();
    });

    ++solvedTasks;

    thingsToDo.pop();
}

void addTask(int x) {
    std::lock_guard<std::mutex> lk(mut);
    thingsToDo.push(x);

    // delay the thread
    std::this_thread::sleep_for(std::chrono::milliseconds(1));

    // notifies the other thread that one element was pushed in the queue
    // use notify_all to notify all threads that wait
    data_cond.notify_one();
}

int main() {
    std::thread t([] {
      for(int i=1;i<=20;++i)
        addTask(i);
    });

    std::thread t2([] {
      for(int i=1;i<=20;++i)
        solveTask();
    });

    t.join();
    t2.join();

    std::cout<<solvedTasks<<"/20 tasks were solved."<<std::endl;

    return 0;
}
