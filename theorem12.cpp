#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
#include <random>
using namespace std;

// Function implementing Theorem 12 with optimized runtime
vector<vector<int>> computeProfitSequence(const vector<pair<int, int>> &items, int maxWeight, int maxProfit) {
    int n = items.size();
    vector<vector<int>> dp(maxWeight + 1, vector<int>(maxProfit + 1, INT_MIN));
    dp[0][0] = 0; // Base case

    // Process items using efficient subarray updates
    for (const auto &[weight, profit] : items) {
        for (int w = maxWeight; w >= 0; --w) {
            for (int p = maxProfit; p >= 0; --p) {
                if (dp[w][p] != INT_MIN && w + weight <= maxWeight && p + profit <= maxProfit) {
                    dp[w + weight][p + profit] = max(dp[w + weight][p + profit], dp[w][p] + profit);
                }
            }
        }
    }

    // Restrict computation to relevant profit subarrays using max-plus convolution
    for (int w = 0; w <= maxWeight; ++w) {
        for (int p = 1; p <= maxProfit; ++p) {
            dp[w][p] = max(dp[w][p], dp[w][p - 1]);
        }
    }

    return dp;
}

// Main function (for testing or integrating into Algorithm 1)
int main() {
    // Sample items: (weight, profit)
    vector<pair<int, int>> items = {{3, 4}, {2, 3}, {4, 2}, {4, 5}};
    int maxWeight = 10;
    int maxProfit = 10;

    auto result = computeProfitSequence(items, maxWeight, maxProfit);

    // Print the profit table
    cout << "Profit Table (Partial):" << endl;
    for (int w = 0; w <= maxWeight; ++w) {
        for (int p = 0; p <= maxProfit; ++p) {
            if (result[w][p] == INT_MIN)
                cout << "-\t";
            else
                cout << result[w][p] << "\t";
        }
        cout << endl;
    }

    return 0;
}
