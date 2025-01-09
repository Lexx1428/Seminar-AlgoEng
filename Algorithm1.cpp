#include <iostream>
#include <vector>
#include <algorithm>
#include <climits>
#include <cmath>
#include <random>
using namespace std;

// Struct to represent an item
struct Item {
    int index;
    int weight;
    int profit;
};

// Function to compute an approximate optimal value using the greedy approach
double computeTildeOPT(int n, double t, vector<Item> items) {
    
    
    vector<pair<double, int>> value_weight_ratios(n);
    for (int i = 0; i < n; i++) {
        value_weight_ratios[i] = {static_cast<double>(items[i].profit) / items[i].weight, i};  //store ratio and original index
    }

    sort(value_weight_ratios.begin(), value_weight_ratios.end(), greater<pair<double, int>>());

    double total_value = 0.0;
    double total_weight = 0.0;  
    int i; 
    for (i = 0; i < n; ++i) {
        int index = value_weight_ratios[i].second;
        if (total_weight + items[index].weight <= t) {  // If full item fits, take it completely
            total_weight += items[index].weight;
            total_value += items[index].profit;
        } else {
            break;  // Stop when capacity is exceeded
        }
    }

    // Include the whole (i* + 1)-th item if it exists
    if (i < n) {  // Check if there is an (i* + 1)-th item
        int index = value_weight_ratios[i].second;
        total_value += items[index].profit;  // Add the full value of the (i* + 1)-th item
    }

    return total_value;
}


// Function to compute profit sequence
vector<vector<int>> computeProfitSequence(const vector<Item>& items, int maxWeight, int maxProfit) {
    vector<vector<int>> dp(maxWeight + 1, vector<int>(maxProfit + 1, INT_MIN));
    dp[0][0] = 0;
    for (const auto& item : items) {
        for (int w = maxWeight; w >= 0; --w) {
            for (int p = maxProfit; p >= 0; --p) {
                if (dp[w][p] != INT_MIN && w + item.weight <= maxWeight && p + item.profit <= maxProfit) {
                    dp[w + item.weight][p + item.profit] = max(dp[w + item.weight][p + item.profit], dp[w][p] + item.profit);
                }
            }
        }
    }
    for (int w = 0; w <= maxWeight; ++w) {
        for (int p = 1; p <= maxProfit; ++p) {
            dp[w][p] = max(dp[w][p], dp[w][p - 1]);
        }
    }
    return dp;
}

// Max-plus convolution
vector<vector<int>> maxPlusConvolution(const vector<vector<int>>& A, const vector<vector<int>>& B) {
    int maxWeight = A.size() + B.size() - 2;
    int maxProfit = A[0].size() + B[0].size() - 2;
    vector<vector<int>> result(maxWeight + 1, vector<int>(maxProfit + 1, INT_MIN));

    for (int w1 = 0; w1 < A.size(); ++w1) {
        for (int p1 = 0; p1 < A[0].size(); ++p1) {
            if (A[w1][p1] != INT_MIN) {
                for (int w2 = 0; w2 < B.size(); ++w2) {
                    for (int p2 = 0; p2 < B[0].size(); ++p2) {
                        if (B[w2][p2] != INT_MIN) {
                            int newW = w1 + w2;
                            int newP = p1 + p2;
                            if (newW <= maxWeight && newP <= maxProfit) {
                                result[newW][newP] = max(result[newW][newP], A[w1][p1] + B[w2][p2]);
                            }
                        }
                    }
                }
            }
        }
    }
    return result;
}

// Extract relevant subarray
vector<vector<int>> extractSubarray(const vector<vector<int>>& dp, int wMin, int wMax, int pMin, int pMax) {
    wMin = max(wMin, 0);
    wMax = min(wMax, (int)dp.size() - 1);
    pMin = max(pMin, 0);
    pMax = min(pMax, (int)dp[0].size() - 1);

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

// Main implementation of Algorithm 1
vector<vector<int>> algorithm1(const vector<Item>& items, int t) {
    int n = items.size();
    int wMax = 0, pMax = 0;
    for (const auto& item : items) {
        wMax = max(wMax, item.weight);
        pMax = max(pMax, item.profit);
    }
    int opt = computeTildeOPT(items.size(),t ,items);
    cout << "Approximate OPT: " << opt << endl;

    int q = log2(min(t / wMax, opt / pMax));
    if (q < 0) q = 0; // Ensure q is non-negative
    int numPartitions = 1 << q;
    int eta = max(17 * log(max(n, 2)), 1.0); // Prevent log(0) or small eta
    int deltaW = t * wMax;
    int deltaP = opt * pMax;

    vector<vector<Item>> partitions(numPartitions);
    random_device rd;
    mt19937 gen(rd());
    uniform_int_distribution<> dis(0, numPartitions - 1);

    for (const auto& item : items) {
        int idx = dis(gen);
        partitions[idx].push_back(item);
    }

    int WqMin = t / numPartitions - deltaW / (numPartitions * eta);
    int WqMax = t / numPartitions + deltaW / (numPartitions * eta);
    int PqMin = opt / numPartitions - deltaP / (numPartitions * eta);
    int PqMax = opt / numPartitions + deltaP / (numPartitions * eta);

    WqMin = max(WqMin, 0); // Ensure bounds are valid
    PqMin = max(PqMin, 0);

    vector<vector<vector<int>>> Dq(numPartitions);
    for (int j = 0; j < numPartitions; ++j) {
        Dq[j] = computeProfitSequence(partitions[j], WqMax, PqMax);
        Dq[j] = extractSubarray(Dq[j], WqMin, WqMax, PqMin, PqMax);
    }

    while (numPartitions > 1) {
        vector<vector<vector<int>>> nextLevel(numPartitions / 2);
        for (int j = 0; j < numPartitions / 2; ++j) {
            auto merged = maxPlusConvolution(Dq[2 * j], Dq[2 * j + 1]);
            int WlMin = t / (numPartitions / 2) - deltaW / ((numPartitions / 2) * eta);
            int WlMax = t / (numPartitions / 2) + deltaW / ((numPartitions / 2) * eta);
            int PlMin = opt / (numPartitions / 2) - deltaP / ((numPartitions / 2) * eta);
            int PlMax = opt / (numPartitions / 2) + deltaP / ((numPartitions / 2) * eta);
            nextLevel[j] = extractSubarray(merged, max(WlMin, 0), WlMax, max(PlMin, 0), PlMax);
        }
        Dq = nextLevel;
        numPartitions /= 2;
    }

    int finalWMin = max(t - sqrt(t * wMax), 0.0);
    int finalWMax = t + sqrt(t * wMax);
    int finalPMin = max(opt - sqrt(opt * pMax), 0.0);
    int finalPMax = opt + sqrt(opt * pMax);

    cout << "Final Weight Range: [" << finalWMin << ", " << finalWMax << "]" << endl;
    cout << "Final Profit Range: [" << finalPMin << ", " << finalPMax << "]" << endl;

    if (Dq[0].empty() || Dq[0][0].empty()) {
        cout << "Root DP table D^0_1 is empty!" << endl;
        return {};
    }

    auto result = extractSubarray(Dq[0], finalWMin, finalWMax, finalPMin, finalPMax);
    return result;
}


int main() {
    vector<Item> items = {{1, 3, 10}, {2, 2, 3}, {3, 4, 2}, {4, 4, 5}};
    int t = 6;
    auto result = algorithm1(items, t);

    // Print the number of items in the result
    cout << "Number of entries in the result: " << result.size() << endl;

    // Check if the result is empty
    if (result.empty()) {
        cout << "No valid results found." << endl;
    } else {
        cout << "C0_1[T;P]:" << endl;
        for (const auto& row : result) {
            for (const auto& value : row) {
                if (value == INT_MIN)
                    cout << "-\t";
                else
                    cout << value << "\t";
            }
            cout << endl;
        }
    }

    return 0;
}