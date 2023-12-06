#include <chrono>
#include <iostream>

class Timer {
public:
    Timer() {
        start = clock.now();
    }
    ~Timer() {
        end = clock.now();
        long micros = std::chrono::duration_cast<std::chrono::microseconds>(end -start).count();
        std::cout << "Took: " << micros << "us" << std::endl;
    }

private:
    std::chrono::high_resolution_clock clock;
    std::chrono::steady_clock::time_point start;
    std::chrono::steady_clock::time_point end;
};
