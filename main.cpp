#include <iostream>
#include <thread>
#include <mutex>
#include <latch>
#include <syncstream>

std::latch latch_a{1};
std::latch latch_d{1};
std::latch done{4};
std::mutex m;

void f(char setName, int index) {
    std::osyncstream(std::cout) << "from set " << setName << " action " << index << " completed\n";
}

void perform_actions(char setName, int count) {
    for (int i = 1; i <= count; ++i) f(setName, i);
}

void worker1() {
    perform_actions('a', 7);
    latch_a.count_down();
    latch_d.wait();
    perform_actions('h', 9);
    done.count_down();
}

void worker2() {
    latch_a.wait();
    perform_actions('b', 4);
    perform_actions('e', 7);
    done.count_down();
}

void worker3() {
    latch_a.wait();
    perform_actions('c', 5);
    perform_actions('f', 4);
    perform_actions('g', 7);
    done.count_down();
}

void worker4() {
    latch_a.wait();
    perform_actions('d', 8);
    latch_d.count_down();
    perform_actions('i', 5);
    done.count_down();
}

int main() {
    std::osyncstream(std::cout) << "calculation started \n";

    std::thread t1(worker1);
    std::thread t2(worker2);
    std::thread t3(worker3);
    std::thread t4(worker4);
    done.wait();

    t1.join();
    t2.join();
    t3.join();
    t4.join();

    std::osyncstream(std::cout) << "calculation completed \n";
}