// Sorokin Daniil, K26, Variant 13

#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <thread>
#include <vector>
#include <mutex>
#include <chrono>
#include <cstdlib>

class SafeData {
private:
    int fields[2];
    std::mutex mtx[2];
    std::mutex global_mtx;

public:
    SafeData() {
        fields[0] = 0;
        fields[1] = 0;
    }

    int get(int index) {
        std::lock_guard<std::mutex> lock(mtx[index]);
        return fields[index];
    }

    void set(int index, int value) {
        std::lock_guard<std::mutex> lock(mtx[index]);
        fields[index] = value;
    }

    std::string toString() {
        std::lock_guard<std::mutex> lock(global_mtx);
        std::lock_guard<std::mutex> lock1(mtx[0]);
        std::lock_guard<std::mutex> lock2(mtx[1]);
        std::stringstream ss;
        ss << "[" << fields[0] << ", " << fields[1] << "]";
        return ss.str();
    }
};

void processFile(const std::string& filename, SafeData& data) {
    std::ifstream fin(filename);
    if (!fin.is_open()) {
        std::cout << "failed to open file: " << filename << std::endl;
        return;
    }
    std::vector<std::string> lines;
    std::string line;
    while (std::getline(fin, line)) lines.push_back(line);
    fin.close();

    auto start = std::chrono::high_resolution_clock::now();
    for (auto& l : lines) {
        std::stringstream ss(l);
        std::string command;
        int index, value;
        ss >> command;

        if (command == "write") {
            ss >> index >> value;
            data.set(index, value);
        }
        else if (command == "read") {
            ss >> index;
            data.get(index);
        }
        else if (command == "string") {
            data.toString();
        }
    }
    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> dur = end - start;
    std::cout << "file: " << filename << " execution time: " << dur.count() << " sec." << std::endl;
}

void generateFile(const std::string& filename, int N) {
    std::ofstream fout(filename);
    for (int i = 0; i < N; i++) {
        int x = rand() % 100;
        if (x < 10) fout << "read 0\n";
        else if (x < 20) fout << "write 0 1\n";
        else if (x < 70) fout << "read 1\n";
        else if (x < 80) fout << "write 1 1\n";
        else fout << "string\n";
    }
    fout.close();
}

void runTest(int threadCount) {
    SafeData data;
    std::vector<std::string> files = {"file_a.txt", "file_b.txt", "file_c.txt"};
    std::vector<std::thread> threads;
    std::cout << threadCount << std::endl;

    for (int i = 0; i < threadCount; i++) {
        threads.emplace_back(processFile, files[i], std::ref(data));
    }
    for (auto& t : threads) t.join();
}

int main() {
    srand(time(0));
    int N = 100000;
    generateFile("file_a.txt", N);
    generateFile("file_b.txt", N);
    generateFile("file_c.txt", N);

    runTest(1);
    runTest(2);
    runTest(3);
    return 0;
}