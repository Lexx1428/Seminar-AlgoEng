#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip> // For setting precision in output
#include <random>

using namespace std;

// Structure to represent an item
struct Item {
    int index;
    int profit;
    int weight;

    // Constructor
    Item(int index, int profit, int weight) : index(index), profit(profit), weight(weight) {}
};

// Comparator function to sort items by profit-to-weight ratio in descending order
bool compare(const Item &a, const Item &b) {
    double r1 = (double)a.profit / a.weight;
    double r2 = (double)b.profit / b.weight;
    return r1 > r2;
}

// Function to solve the fractional knapsack problem
double fractionalKnapsack(int capacity, const std::vector<Item> &items) {
    // Sort items by profit-to-weight ratio in descending order
    std::vector<Item> sortedItems = items;
    std::sort(sortedItems.begin(), sortedItems.end(), compare);

    double totalProfit = 0.0; // Total profit in the knapsack
    int currentCapacity = capacity; // Remaining capacity of the knapsack

    for (const auto &item : sortedItems) {
        if (currentCapacity == 0) break; // Stop if knapsack is full

        if (item.weight <= currentCapacity) {
            // Take the full item
            totalProfit += item.profit;
            currentCapacity -= item.weight;
        } else {
            // Take the fractional part of the item
            totalProfit += item.profit * ((double)currentCapacity / item.weight);
            break; // After taking the fraction, the knapsack is full
        }
    }

    return totalProfit;
}

vector<Item> generateBalancedKnapsackItems(int num_items, int max_profit, int max_weight, int knapsack_capacity) {
    vector<Item> items;

    // Set random seed for reproducibility
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> profit_dist(10, max_profit);
    uniform_int_distribution<> weight_dist(1, max_weight);

    // Generate items
    for (int i = 0; i < num_items; ++i) {
        int profit = profit_dist(gen);
        int weight = weight_dist(gen);

        // Ensure balanced profit-to-weight ratio
        while (static_cast<double>(profit) / weight > 2.0 || static_cast<double>(profit) / weight < 0.5) {
            profit = profit_dist(gen);
            weight = weight_dist(gen);
        }

        items.push_back({i + 1, profit, weight});
    }

    return items;
}

int main() {
    const int num_items = 1000;
    const int max_profit = 100;
    const int max_weight = 50;
    const int knapsack_capacity = 25000;

    vector<Item> items = generateBalancedKnapsackItems(num_items, max_profit, max_weight, knapsack_capacity);


    double maxProfit = fractionalKnapsack(knapsack_capacity, items);

    // Print the result with 2 decimal points
    std::cout << "Maximum profit in the knapsack: " << std::fixed << std::setprecision(2) << maxProfit << std::endl;

    return 0;
}