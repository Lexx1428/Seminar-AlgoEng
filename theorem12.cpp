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

// Function to extract subarray Cqj from Dqj
vector<vector<int>> extractSubarray(const vector<vector<int>>& dp, int wMin, int wMax, int pMin, int pMax) {
    vector<vector<int>> subarray;

    for (int w = wMin; w <= wMax; ++w) {
        vector<int> row;
        for (int p = pMin; p <= pMax; ++p) {
            row.push_back(dp[w][p]);
        }
        subarray.push_back(row);
    }

    return subarray;
}

// Main function (for testing or integrating into Algorithm 1)
int main() {
    // Sample items: (weight, profit)
    vector<pair<int, int>> items = {{3, 4}, {2, 3}, {4, 2}, {4, 5}};
    int maxWeight = 10;
    int maxProfit = 10;

    auto result = computeProfitSequence(items, maxWeight, maxProfit);

    // Define intervals Wq and Pq for extraction
    int wMin = 2, wMax = 5;
    int pMin = 3, pMax = 8;

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

    auto subarray = extractSubarray(result, wMin, wMax, pMin, pMax);

    // Print the extracted subarray with labels
    cout << "\nExtracted Subarray (Weights: " << wMin << " to " << wMax << ", Profits: " << pMin << " to " << pMax << "):\n\t";
    for (int p = pMin; p <= pMax; ++p) {
        cout << p << "\t";
    }
    cout << endl;

    for (int i = 0; i < subarray.size(); ++i) {
        cout << (wMin + i) << "\t";
        for (int value : subarray[i]) {
            if (value == INT_MIN)
                cout << "-\t";
            else
                cout << value << "\t";
        }
        cout << endl;
    }

    return 0;
}
