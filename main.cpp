// компілятор: g++ (MinGW-W64 x86_64-ucrt-posix-seh) 15.1.0
// reduce

#include <iostream>
#include <vector>
#include <numeric>
#include <random>
#include <chrono>
#include <thread>
#include <execution>

std::vector<int> make_data(size_t n) {
    std::vector<int> v(n);
    std::mt19937 gen(123);
    std::uniform_int_distribution<int> dist(1, 100);
    for (auto& x : v) x = dist(gen);
    return v;
}

double measure_manual(const std::vector<int>& v) {
    auto start = std::chrono::high_resolution_clock::now();
    long long s = 0;
    for (int x : v) s += x;
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measure_reduce_seq(const std::vector<int>& v) {
    auto start = std::chrono::high_resolution_clock::now();
    std::reduce(v.begin(), v.end(), 0LL);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measure_reduce_par(const std::vector<int>& v) {
    auto start = std::chrono::high_resolution_clock::now();
    std::reduce(std::execution::par, v.begin(), v.end(), 0LL);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measure_reduce_par_unseq(const std::vector<int>& v) {
    auto start = std::chrono::high_resolution_clock::now();
    std::reduce(std::execution::par_unseq, v.begin(), v.end(), 0LL);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

double measure_custom(const std::vector<int>& v, int k) {
    size_t n = v.size();
    size_t part = n / k;
    std::vector<long long> sums(k, 0);
    std::vector<std::thread> threads;

    auto start = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < k; ++i) {
        size_t begin = i * part, end = (i == k - 1) ? n : begin + part;
        threads.emplace_back([&, begin, end, i]() {
            for (size_t j = begin; j < end; ++j)
                sums[i] += v[j];
        });
    }
    for (auto& t : threads) t.join();
    std::reduce(sums.begin(), sums.end(), 0LL);
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double>(end - start).count();
}

void run_experiment(size_t n, unsigned hw) {
    std::cout << "size = " << n << "\n";
    auto data = make_data(n);

    std::cout << "manual: " << measure_manual(data) << " s\n";
    std::cout << "reduce: " << measure_reduce_seq(data) << " s\n";
    std::cout << "reduce(par): " << measure_reduce_par(data) << " s\n";
    std::cout << "reduce(par_unseq):  " << measure_reduce_par_unseq(data) << " s\n";

    double best = 1000000000;
    int best_k = 1;

    std::cout << "custom reduce: ";
    for (int k = 1; k <= (int)hw * 2; ++k) {
        double t = measure_custom(data, k);
        std::cout << k << ":" << t << " s  ";
        if (t < best) {
            best = t;
            best_k = k;
        }
    }
    std::cout << "\n\nbest K = " << best_k
              << " (" << best << " s)\n\n";
}

int main() {
    std::cout << "labs 2 (reduce) \n";
    unsigned hw = std::thread::hardware_concurrency();
    std::cout << "hardware threads: " << hw << "\n\n";

    for (size_t n : {100000, 500000, 1000000})
        run_experiment(n, hw);

    std::cout << "end of experiment";
    return 0;
}