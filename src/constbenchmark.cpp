#include <iostream>
#include <string>
#include <chrono>
#include <cstring>
#include <cstdint>

using namespace std;
using namespace std::chrono;

// Global volatile dummy variable to force evaluation.
volatile uint64_t dummy = 0;

char* convert_by_copy(const std::string &s) {
    char *copy = new char[s.size() + 1];
    strcpy(copy, s.c_str());
    return copy;
}

int main() {

    std::string s = "This is a test string used for performance comparison.";
    
    const int iterations = 10000000;

    // Benchmark 1: Using c_str() with const_cast.
    auto start1 = high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        char *p = const_cast<char*>(s.c_str());
        // Force the pointer value into the dummy variable so the call isn’t optimized away.
        dummy ^= reinterpret_cast<uintptr_t>(p);
    }
    auto end1 = high_resolution_clock::now();
    auto duration1 = duration_cast<nanoseconds>(end1 - start1).count();

    // Benchmark 2: Converting by copying the string.
    auto start2 = high_resolution_clock::now();
    for (int i = 0; i < iterations; i++) {
        char* copy = convert_by_copy(s);
        dummy ^= reinterpret_cast<uintptr_t>(copy);
        delete[] copy;
    }
    auto end2 = high_resolution_clock::now();
    auto duration2 = duration_cast<nanoseconds>(end2 - start2).count();

    cout << "Time using c_str() and const_cast: " << duration1 << " nanoseconds." << endl;
    cout << "Time using conversion by copying: " << duration2 << " nanoseconds." << endl;
    cout << "Dummy value (to avoid optimization): " << dummy << endl;

    return 0;
}
