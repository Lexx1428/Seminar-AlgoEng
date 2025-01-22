#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
#include <algorithm>
#include <cmath>
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


//compute subarray: D <- A[I;V], A monotone non-decreasing array
//If there exist no index i ∈ I with A[i] ∈ V , then set D to the empty array. Otherwise,
//let imin := min{i ∈ I : A[i] ∈ V } and imax := max{i ∈ I : A[i] ∈ V }, and set D to the
//subarray A[imin . . . imax]. Note that since A is monotone, for every i ∈ {imin, . . . , imax} we
//have A[i] ∈ V . Thus A[I; V ] returns the subarray of A with indices in I and values in V .


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
    vector<int> pSequence;
    for (int k=0;k < capacity+1; ++k) {
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

vector<vector<int>> Algo1_half(const vector<Item>& items, int t){
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
    int eta = 17 * log(n); // Prevent log(0) or small eta
    int deltaW = t * wMax;
    int deltaP = opt * pMax;

    vector<vector<Item>> groups;
    partitionGroups(items, groups, numPartitions);

    int WqMin = t / numPartitions - sqrt(deltaW / numPartitions) * eta;
    int WqMax = t / numPartitions + sqrt(deltaW / numPartitions) * eta;
    int PqMin = opt / numPartitions - sqrt(deltaP / numPartitions) * eta;
    int PqMax = opt / numPartitions + sqrt(deltaP / numPartitions) * eta;

    pair<int,int> weight_interval(WqMin,WqMax);
    pair<int,int> profit_interval(WqMin,WqMax);

    vector<vector<int>> Dq(numPartitions);
    vector<vector<int>> Cq(numPartitions);

    cout << "1st" << endl;

    for (int j = 0; j < numPartitions; ++j) {
        Dq[j] = computeProfitSequence(groups[j], WqMax);
        Cq[j] = computeSubarray(Dq[j], weight_interval, profit_interval);
    } 
    
    cout << "Ende" << endl;

    return Cq;
} 

void print2DVector(const vector<vector<int>>& vec) {
    for (const auto& row : vec) { // Iterate through each row
        for (const auto& element : row) { // Iterate through each element in the row
            cout << element << " ";
        }
        cout << endl; // Newline after each row
    }
}



int main() {

    vector<Item> items = {
        {1, 5, 10},
        {2, 8, 14},
        {3, 6, 12},
        {4, 7, 13},
        {5, 9, 16},
        {6, 4, 8},
        {7, 10, 20},
        {8, 5, 9},
        {9, 8, 15},
        {10, 6, 11},
        {11, 12, 24},
        {12, 7, 14},
        {13, 5, 10},
        {14, 8, 18},
        {15, 9, 17},
        {16, 10, 19},
        {17, 6, 13},
        {18, 7, 12},
        {19, 4, 7},
        {20, 5, 9}
    };

    vector<vector<int>> opt;
    int t = 30;

    opt = Algo1_half(items,t); 
    print2DVector(opt);


}