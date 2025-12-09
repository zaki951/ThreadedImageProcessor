#include <iostream>
#include <thread>
#include <functional>
#include <chrono>
#include <mutex>
#include <algorithm>
#include "clock.h"
#include "workers.h"
#include <future>

std::mutex m;
constexpr int IT_N = 4000;



bool is_prime(uint64_t n) {
    if (n < 2) return false;
    for (uint64_t i = 2; i*i <= n; ++i)
        if (n % i == 0) return false;
    return true;
}

uint64_t count_primes(uint64_t start, uint64_t end) {
    uint64_t count = 0;
    for (uint64_t i = start; i < end; ++i)
        if (is_prime(i)) ++count;
    return count;
}

void task_prime() {
	count_primes(0, 10000);
}



void producer(Workers& w) {
	for (int i = 0; i < IT_N; ++i) {
		w.add_task(task_prime);
	}
}

void single_thread() {
	for (int i = 0; i < IT_N; ++i) {
		task_prime();
	}
}


void async_producer() {
	std::vector<std::future<void>> futures;
	
	for (int i = 0; i < IT_N; ++i) {
		auto f = std::async(std::launch::async, task_prime);
		futures.push_back(std::move(f));
	}
	for (auto& f : futures) {
		f.wait();
	}
}

void bad_thread_producer() {
	std::vector<std::thread> threads;

	for (int i = 0; i < IT_N; ++i) {
		auto f = std::thread(task_prime);
		threads.push_back(std::move(f));
	}
	for (auto& t : threads) {
		t.join();
	}
}



int main(int argc, char** argv) {
	{
		Clock ck;
		const uint32_t N_THREADS = std::max(2u, std::thread::hardware_concurrency());
    	std::cout << "Avalaible threads : " << N_THREADS << std::endl;

		Workers w {N_THREADS};
	
		producer(w);
		std::cout <<"Multithreading => ";
	}
	{
		Clock ck;
		single_thread();
		std::cout << "Single thread => ";
	}
	{
		Clock ck;
		async_producer();
		std::cout << "async => ";
	}
	{
		Clock ck;
		bad_thread_producer();
		std::cout << "bad thread => ";
	}
	return 0;
}


