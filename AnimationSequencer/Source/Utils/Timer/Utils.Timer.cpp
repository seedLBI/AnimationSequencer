#include "Utils.Timer.h"

void Timer::start() {
    if (!running) {
        start_time = std::chrono::high_resolution_clock::now();
        running = true;
    }
}

void Timer::stop() {
    if (running) {
        stop_time = std::chrono::high_resolution_clock::now();
        running = false;
    }
}

long long Timer::elapsedNanoseconds() const {
    if (!running) {
        return std::chrono::duration_cast<std::chrono::nanoseconds>(stop_time - start_time).count();
    }
    return 0;
}

long long Timer::elapsedMicroseconds() const {
    if (!running) {
        return std::chrono::duration_cast<std::chrono::microseconds>(stop_time - start_time).count();
    }
    return 0;
}

long long Timer::elapsedMilliseconds() const {
    if (!running) {
        return std::chrono::duration_cast<std::chrono::milliseconds>(stop_time - start_time).count();
    }
    return 0;
}

double Timer::elapsedSeconds() const {
    if (!running) {
        return std::chrono::duration<double>(stop_time - start_time).count();
    }
    return 0.0;
}