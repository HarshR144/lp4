#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

// Function to solve 0/1 Knapsack using Dynamic Programming
int knapsackDP(vector<int>& values, vector<int>& weights, int n, int W, vector<int>& selectedItems) {
    vector<vector<int>> dp(n + 1, vector<int>(W + 1, 0));

    // DP Table filling
    for (int i = 1; i <= n; i++) {
        for (int w = 1; w <= W; w++) {
            if (weights[i - 1] <= w) {
                dp[i][w] = max(dp[i - 1][w], values[i - 1] + dp[i - 1][w - weights[i - 1]]);
            } else {
                dp[i][w] = dp[i - 1][w];
            }
        }
    }

    // Backtrack to find selected items
    int w = W;
    for (int i = n; i > 0; i--) {
        if (dp[i][w] != dp[i - 1][w]) {
            selectedItems.push_back(i - 1);
            w -= weights[i - 1];
        }
    }

    return dp[n][W];
}

// Function to solve 0/1 Knapsack using Branch and Bound

int main() {
    int n, W;
    cout << "Enter the number of items: ";
    cin >> n;
    cout << "Enter the knapsack capacity: ";
    cin >> W;

    vector<int> values(n), weights(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter the value and weight of item " << i + 1 << ": ";
        cin >> values[i] >> weights[i];
    }

    // Dynamic Programming
    auto startTimeDP = chrono::high_resolution_clock::now();
    vector<int> selectedItemsDP;
    int maxValueDP = knapsackDP(values, weights, n, W, selectedItemsDP);
    auto endTimeDP = chrono::high_resolution_clock::now();

    cout << "Dynamic Programming:" << endl;
    cout << "Maximum value that can be put in the knapsack: " << maxValueDP << endl;
    cout << "Selected items: ";
    for (int item : selectedItemsDP) {
        cout << item + 1 << " ";  // Output item number (1-indexed)
    }
    cout << endl;

    auto durationDP = chrono::duration_cast<chrono::microseconds>(endTimeDP - startTimeDP).count();
    cout << "Time complexity: " << durationDP << " microseconds" << endl;
    cout << "Space complexity: O(n * W)" << endl;


    return 0;
}
