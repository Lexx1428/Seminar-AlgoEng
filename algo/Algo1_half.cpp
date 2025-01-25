#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
#include <algorithm>
#include <cmath>
#include <random>

using namespace std;

struct Item {
    int index;
    int profit;
    int weight;
};

ostream& operator<<(ostream &os, const Item &item) {
    os << "Index: " << item.index << ", Value: " << item.profit << ", Weight: " << item.weight;
    return os;
}

vector<Item> generateDataset(const int &numItems, const int &maxWeight, const int &maxValue) {
    vector<Item> items;
    items.reserve(numItems);
    srand(time(0));    //seed for randomness
    for (int i = 0; i < numItems; ++i) {
        int index = i;
        int weight = rand() % maxWeight + 1;
        int value = rand() % maxValue + 1;
        items.push_back({index, value, weight});
    }
    return items;
}

void partitionGroups(const vector<Item> &items, vector<vector<Item>> &groups, int count_of_groups) {

    size_t length = items.size();
    groups.resize(count_of_groups);
    int size_of_group = length/count_of_groups;

    int remainder = length % count_of_groups;

    int item_index = 0;

    for (int i = 0; i < count_of_groups; ++i) {
        
        int current_group_size = size_of_group + (i < remainder ? 1 : 0);

        for (int j = 0; j < current_group_size; ++j) {
            groups[i].push_back(items[item_index]);
            item_index++;
        }
    }
}

int kSackDP(int capacity, vector<Item>& items, int index, vector<vector<int>>& memo) {
     
     //base condition
     if (index < 0) {
        return 0;
     }

     if (memo[index][capacity] != -1) {
        return memo[index][capacity];
     }

     //store the value in the table before return
     if(items[index].weight > capacity) {
        memo[index][capacity] = kSackDP(capacity, items, index-1, memo);
     } else {
        memo[index][capacity] = max(items[index].profit + kSackDP(capacity-items[index].weight, items, index-1, memo),
                                    kSackDP(capacity, items, index-1, memo));
     }

     return memo[index][capacity];
}

int kSack(int capacity, vector<Item>& items) {
    int n = items.size();
    vector<vector<int>> memo(n, vector<int>(capacity+1, -1));
    return kSackDP(capacity, items, n-1, memo);
}

//function to compute profit sequence for one group, k = capacity
vector<int> computeProfitSequence(vector<Item>& group, int capacity) {
    vector<int> pSequence(capacity+1, 0);
    for (int k=0;k <= capacity; ++k) {
        pSequence[k] = kSack(k, group);
    }
    return pSequence;
}

vector<int> computeSubarray(const vector<int> &input, const pair<int, int> &indices, const pair<int, int> values) {
    vector<int> subarray;
    for (int i = indices.first; i < indices.second + 1; ++i) {
        if (input[i]>= values.first && input[i] <= values.second) {
            subarray.push_back(input[i]);
        }
    }
    return subarray;
}

int computeTildeOPT(int n, double t, vector<Item> items) {
    
    
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

vector<int> maxPlusConv(const vector<int> &a, const vector<int> &b) {
    size_t n = a.size();
    size_t m = b.size();

    vector<int> c(n+m-1, 0);

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            c[i+j] = max(c[i+j], a[i] + b[j]);
        }
    }
    return c;
}

void print2DVector(const vector<vector<int>>& vec) {
    for (const auto& row : vec) { // Iterate through each row
        for (const auto& element : row) { // Iterate through each element in the row
            cout << element << " ";
        }
        cout << "row"<< endl; // Newline after each row
    }
}

vector<vector<int>> Algo1_half(const vector<Item>& items, int t){
    int n = items.size();
    int wMax = 0, pMax = 0;
    for (const auto& item : items) {
        wMax = max(wMax, item.weight);
        pMax = max(pMax, item.profit);
    }

    cout << "pMax = " << pMax << endl; 

    int opt = computeTildeOPT(items.size(),t ,items);
    cout << "Approximate OPT: " << opt << endl;

    int q = log2(min(t / wMax, opt / pMax));
    if (q < 0) q = 0; // Ensure q is non-negative
    int numPartitions = 1 << q;
    int eta = 17 * log(n); // Prevent log(0) or small eta
    int deltaW = t * wMax;
    int deltaP = opt * pMax;

    vector<vector<Item>> groups;
    partitionGroups(items, groups, numPartitions);

    cout << "group size = " << groups.size() << endl;
    cout << "Partitions = " << numPartitions << endl;

    double error_margin_p = sqrt(double(deltaP / numPartitions)) * eta;
    double error_margin_w = sqrt(double(deltaW / numPartitions)) * eta;

    cout << "delta p = " << deltaP << endl;
    cout << "delta w = " << deltaW << endl;
    cout << "eta= " << eta << endl; 
    cout << "Without squareroot = " << deltaP / numPartitions << endl;
    cout << "error margin of P = " << error_margin_p << endl;
    cout << "error margin of W = " << error_margin_w << endl;
    cout << "t/2^l = " << t / numPartitions << endl;
    cout << "opttilde/2^l = " << opt / numPartitions << endl;

    int WqMin = t / numPartitions - sqrt(deltaW / numPartitions) * eta;
    int WqMax = t / numPartitions;
    int PqMin = opt / numPartitions - sqrt(deltaP / numPartitions) * eta;
    int PqMax = opt / numPartitions;

    cout << "WqMin = "<< WqMin << endl;
    cout << "WqMax = "<< WqMax << endl;
    cout << "PqMin = "<< PqMin << endl;
    cout << "PqMax = "<< PqMax << endl;

    pair<int,int> Wq(0,WqMax);
    pair<int,int> Pq(0,PqMax);

    pair<int,int> wStar(0, WqMax);
    pair<int,int> pStar(0, PqMax);

    vector<vector<int>> Dq(numPartitions);
    vector<vector<int>> Cq(numPartitions);
    vector<vector<int>> CCq(numPartitions);

    
    cout << endl;
    cout << "START PROFIT SEQ" << endl;

    for (int j = 0; j < numPartitions; ++j) {
        Dq[j] = computeProfitSequence(groups[j], WqMax);
        Cq[j] = computeSubarray(Dq[j], wStar, pStar);
        CCq[j] = computeSubarray(Cq[j], Wq, Pq);
    }
    cout << "Dq :" << endl; 
    //print2DVector(Dq);

    cout << "Cq :" << endl;
    //print2DVector(Cq);
    
    cout << "Ccq length = " << CCq.size() << endl;
    cout << "Dq length = " << Dq.size() << endl;
    cout << "END PROFIT SEQ" << endl;
    cout << endl;
    
    cout << "START MAXCONV" << endl;
    for (int level = q - 1; level >= 0; --level) {
        int WlMax = t / (1 << level);
        int PlMax = opt / (1 << level);

        pair<int, int> Wl(0, WlMax);
        pair<int, int> Pl(0, PlMax);

        vector<vector<int>> next_level_arrays((1 << level));
        cout << "next level array length = " << next_level_arrays.size() << endl;

        for (int j = 0; j < (1 << level); ++j) {
            cout << "Inside inner for loop, j = " << j << endl;
            vector<int> convolved = maxPlusConv(CCq[2 * j], CCq[2 * j + 1]);
            vector<int> filtered = computeSubarray(convolved, Pl, Wl);
            next_level_arrays[j] = filtered;
        }

        // Safely update CCq
        CCq.clear();
        CCq = next_level_arrays; // Ensure no unexpected optimizations
        cout << "After inner for loop" << endl;
    }
    for(const auto& elem : CCq){
        elem.size();
    }
    cout << "END MAXCONV" << endl;

    return CCq;
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

void printVectorItem(const vector<Item>& vec) {
    for (const auto& element : vec) {
        cout << element.index << " " << element.profit << " " << element.weight << endl;
    }
}

int main() {

    const int num_items = 1000;
    const int max_profit = 100;
    const int max_weight = 50;
    const int knapsack_capacity = 25000;

    vector<Item> items = generateBalancedKnapsackItems(num_items, max_profit, max_weight, knapsack_capacity);
    int t = 50;
    vector<vector<int>> opt;
    opt = Algo1_half(items,knapsack_capacity);

    cout << "OPT tilde = " << computeTildeOPT(num_items, knapsack_capacity, items) << endl;

}