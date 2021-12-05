#pragma clang diagnostic push
#pragma ide diagnostic ignored "cppcoreguidelines-narrowing-conversions"
#include <algorithm>
#include <iostream>
#include <future>
#include <thread>
#include <mutex>

std::mutex m;

struct Threads {
    template<typename Rep, typename Period>
    int thread1(std::chrono::duration<Rep, Period> time, int threadid) {
        auto start = std::chrono::high_resolution_clock::now();
        m.lock();
        std::cout << "Starting thread " << threadid << std::endl;
        m.unlock();
        std::this_thread::sleep_for(time);
        m.lock();
        std::cout << "Terminating thread " << threadid << std::endl;
        m.unlock();
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end-start);
        return duration.count();
    }
};

int main() {
    std::cout << "Hello, World!" << std::endl;
    Threads threads;

    auto a1 = std::async([&threads]() -> int {
        return threads.thread1(std::chrono::seconds(4), 1);
    });
    auto a2 = std::async([&threads]() -> int {
        return threads.thread1(std::chrono::seconds(6), 2);
    });

    a1.wait();
    a2.wait();
    std::cout << a1.get() << std::endl;
    std::cout << a2.get() << std::endl;
    return 0;
}

#pragma clang diagnostic pop