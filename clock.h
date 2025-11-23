#pragma once

#include <chrono>
#include <iostream>

class Clock {
public:
    Clock() {
        _start = std::chrono::steady_clock::now();

    }
    ~Clock() {
        auto now = std::chrono::steady_clock::now();
        auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(now - _start).count();
        std::cout << "Elapsed time: " << elapsed << " ms\n";
    }
private:
    std::chrono::time_point<std::chrono::steady_clock> _start;
};