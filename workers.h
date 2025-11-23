#pragma once
#include <thread>
#include <mutex>
#include <vector>
#include <atomic>
#include <future>
#include <functional>
#include <iostream>
#include "safe_queue.h"

class Workers {
public:
	using FUNC = std::function<void()>;
	Workers(uint32_t n)  {
		_workers.reserve(n);
		for (uint32_t i = 0; i < n; ++i) {
			_workers.emplace_back([this](){
				while (true) {
					auto t = _queue.pop();
					if (!t) {
						return;
					}
					(*t)();
				}
			});
		}
	}
	void add_task(FUNC task) {
		_queue.push(std::make_shared<FUNC>(std::move(task)));
	}
	~Workers() {
		_queue.stop_empty();
		for (auto& el : _workers) {
			el.join();
		}
	}

private:
	std::vector<std::thread> _workers;
	safe_queue<FUNC> _queue;	
};
