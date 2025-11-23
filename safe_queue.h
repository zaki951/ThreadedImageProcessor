#pragma once

#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <memory>
#include <atomic>

template <typename T>
class safe_queue {
public :
	void push(std::shared_ptr<T> p) {
		std::unique_lock l {_m};
		_q.push(std::move(p));
		_cv_push.notify_one();
	}

	std::shared_ptr<T> pop() {
		std::unique_lock l {_m};
		_cv_push.wait(l, [&](){
			return !_q.empty() || _stop;
		});
		if (_q.empty()) {
			return nullptr;
		}
		auto tmp = _q.front();
		_q.pop();
		_cv_pop.notify_one(); // stop_empty()
		return tmp;
	}
	inline void stop() {
		_stop = true;
		_cv_push.notify_all();
	}

	std::shared_ptr<T> try_pop() {
		std::lock_guard l {_m};
		if (_q.empty()) {
			return nullptr;
		}
		auto tmp = _q.front();
		_q.pop();
		_cv_pop.notify_one(); // stop_empty()
		return tmp;
	}
	bool empty() {
		std::lock_guard l {_m};
		return _q.empty();
	}
	void stop_empty() {
		std::unique_lock l {_m};
		_cv_pop.wait(l, [&](){
			return _q.empty();
		});
		stop();
	}

private:
	std::atomic<bool> _stop {false};
	std::queue<std::shared_ptr<T>> _q;
	std::condition_variable _cv_push;
	std::condition_variable _cv_pop;
	std::mutex _m;
};
