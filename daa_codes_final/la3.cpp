#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>

using namespace std;

// Item struct to store value, weight, and value per weight
struct Item {
    int value, weight;
    double valuePerWeight;
};

// Comparator to sort items by value per weight in descending order
bool compareItems(const Item& a, const Item& b) {
    return a.valuePerWeight > b.valuePerWeight;
}

// Function to solve the fractional Knapsack problem
double fractionalKnapsack(vector<Item>& items, int capacity) {
    sort(items.begin(), items.end(), compareItems);
    double totalValue = 0.0;
    int currentWeight = 0;

    cout << "Fraction of each item taken and corresponding value:" << endl;

    for (const auto& item : items) {
        if (currentWeight + item.weight <= capacity) {
            totalValue += item.value;
            currentWeight += item.weight;
                            
            cout << "Item " << &item - &items[0] + 1 << ": 1.0 (" << item.value << ")" << endl;
        } else {
            int remainingCapacity = capacity - currentWeight;
            double fraction = static_cast<double>(remainingCapacity) / item.weight;
            totalValue += item.value * fraction;
            cout << "Item " << &item - &items[0] + 1 << ": " << fraction << " (" << item.value * fraction << ")" << endl;
            break;
        }
    }

    return totalValue;
}

int main() {
    int n, W;
    cout << "Enter the number of items: ";
    cin >> n;
    cout << "Enter the knapsack capacity: ";
    cin >> W;

    vector<Item> items(n);
    for (int i = 0; i < n; i++) {
        cout << "Enter the value and weight of item " << i + 1 << ": ";
        cin >> items[i].value >> items[i].weight;
        items[i].valuePerWeight = static_cast<double>(items[i].value) / items[i].weight;
    }

    auto startTime = chrono::high_resolution_clock::now();
        double maxValue = fractionalKnapsack(items, W);
    auto endTime = chrono::high_resolution_clock::now();






    cout << "Maximum value that can be put in the knapsack: " << maxValue << endl;
    auto duration = chrono::duration_cast<chrono::microseconds>(endTime - startTime).count();
    cout << "Time complexity: " << duration << " microseconds" << endl;
    cout << "Space complexity: O(n)" << endl;

    return 0;
}