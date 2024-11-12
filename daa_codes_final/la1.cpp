#include <iostream>
#include <vector>
#include <chrono>
#include<unordered_map>
using namespace std;
using namespace std::chrono;
// Iterative Fibonacci function using memoization
void iStepFibonacci(int n) {
    vector<int> f;
    int prev0 = 0;
    int prev1 = 1;

    f.push_back(prev0);
    if (n > 0) f.push_back(prev1);

    for (int i = 2; i <= n; i++) {
        f.push_back(f[i - 1] + f[i - 2]);
    }

    cout << "Fibonacci Series (Iterative): ";
    for (int num : f) {
        cout << num << " ";
    }
    cout << endl;
}

// Counter for recursive steps
int rSteps = 0;

// Recursive Fibonacci calculation
int rStepFibonacci(int n) {
    rSteps++;
    if (n <= 1) return n;
    return rStepFibonacci(n - 1) + rStepFibonacci(n - 2);
}

// Recursive function to print Fibonacci sequence
void printFibonacciRecursive(int n) {
    for (int i = 0; i <= n; i++) {
        cout << rStepFibonacci(i) << " ";
    }
    cout << endl;
}

// with recurisve memorization
int fibonacci_recursive_memo(int n, unordered_map<int, int>& cache) {
    // Check if the value is already in the cache
    if (cache.find(n) != cache.end()) {
        return cache[n];
    }
    
    // Base cases
    if (n <= 1) {
        return n;
    }
    
    // Recursive calculation with memoization
    cache[n] = fibonacci_recursive_memo(n - 1, cache) + fibonacci_recursive_memo(n - 2, cache);
    return cache[n];
}




int main() {
    int n;
    cout << "Enter the value of n to print fibonacci numbers till Fn: ";
    cin >> n;
    cout << "-------------------------------------------------------" << endl;

    // Measure time for the recursive approach
    cout << "Executing recursive Fibonacci algorithm:" << endl;
    
    cout << "Fibonacci Series (Recursive): ";

    auto start = high_resolution_clock::now();
        printFibonacciRecursive(n);
    auto end = high_resolution_clock::now();
    
    auto rDuration = duration_cast<microseconds>(end - start).count();

    cout << "Time elapsed for recursion: " << rDuration << " microseconds" << endl;
    cout << "Recursive Calls: " << rSteps << endl;
    cout << "-------------------------------------------------------" << endl;

    // Measure time for the iterative approach
    cout << "Executing iterative Fibonacci algorithm:" << endl;
    start = high_resolution_clock::now();
        iStepFibonacci(n);
    end = high_resolution_clock::now();
    auto iDuration = duration_cast<microseconds>(end - start).count();

    cout << "Time elapsed for iteration: " << iDuration << " microseconds" << endl;
    cout << "Estimated Space Used for Iterative Vector: " << sizeof(int) * (n + 1) << " bytes" << endl;

    cout << "-------------------------------------------------------" << endl;
    return 0;
}
