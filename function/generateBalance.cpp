#include <iostream>
#include <vector>
#include <random>
#include <cmath>

using namespace std;

struct Item {
    int index;
    int profit;
    int weight;
};

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

    // Print knapsack capacity and items
    cout << "Knapsack Capacity: " << knapsack_capacity << endl;
    cout << "Items:" << endl;
    for (const auto& item : items) {
        cout << "Index: " << item.index << ", Profit: " << item.profit
             << ", Weight: " << item.weight << endl;
    }

    return 0;
}