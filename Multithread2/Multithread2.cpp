#include <iostream>
#include <vector>
#include <thread>
#include <chrono>
#include <iomanip>
#include <Windows.h>

void sumVectors(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int start, int end) {
    for (int i = start; i < end; ++i) {
        result[i] = a[i] + b[i];
    }
}

void measureSumTime(const std::vector<int>& a, const std::vector<int>& b, std::vector<int>& result, int num_threads, std::vector<double>& times) {
    std::vector<std::thread> threads;
    int length = a.size();
    int chunk_size = length / num_threads;

    auto start_time = std::chrono::high_resolution_clock::now();
    for (int i = 0; i < num_threads; ++i) {
        int start = i * chunk_size;
        int end = (i == num_threads - 1) ? length : (i + 1) * chunk_size;
        threads.emplace_back(sumVectors, std::ref(a), std::ref(b), std::ref(result), start, end);
    }

    for (auto& thread : threads) {
        thread.join();
    }
    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end_time - start_time;

    times.push_back(duration.count());
}

int main() {
    setlocale(LC_ALL, "RU");

    std::vector<int> sizes = { 1000, 10000, 100000, 1000000 };
    std::vector<int> threads = { 1, 2, 4, 8, 16 };

    int num_cores = std::thread::hardware_concurrency();
    std::cout << "Количество аппаратных ядер - " << num_cores << "\n\n";

    std::cout << std::setw(15) << " "
        << std::setw(10) << "1000"
        << std::setw(10) << "10000"
        << std::setw(10) << "100000"
        << std::setw(10) << "1000000" << std::endl;

    for (int num_threads : threads) {
        std::cout << std::setw(2) << num_threads << " потоков ";
        for (int size : sizes) {
            std::vector<int> a(size, 1); 
            std::vector<int> b(size, 2); 
            std::vector<int> result(size);
            std::vector<double> times;

            measureSumTime(a, b, result, num_threads, times);

            std::cout << std::setw(10) << times.back() << "s";
        }
        std::cout << std::endl;
    }

    return 0;
}
