#include <iostream>
#include <string>
#include <thread>
#include <chrono>
#include <future>
#include <syncstream>

void run_task(const std::string& name, int seconds) {
    std::this_thread::sleep_for(std::chrono::seconds(seconds));
    std::osyncstream(std::cout) << name << " finished (" << seconds << " sec)\n";
}

void work() {
    std::cout << "Start working\n";
    auto start = std::chrono::high_resolution_clock::now();
    auto f_a1 = std::async(std::launch::async, run_task, "A1", 7);
    run_task("A2", 1);
    run_task("B2", 1);
    run_task("C2", 1);
    f_a1.get();

    auto f_c1 = std::async(std::launch::async, run_task, "C1", 7);
    run_task("B1", 1);
    run_task("D", 1);
    f_c1.get();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "Work is done! Time: " << elapsed.count() << " sec\n";
}

int main() {
    work();
    return 0;
}