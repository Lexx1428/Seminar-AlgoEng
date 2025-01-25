#include "allfunctions.h"

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

int findWmax(const vector<Item>& items) {
    int wmax = 0;
    for (const auto& item : items) {
        if (item.weight > wmax) {
            wmax = item.weight;
        }
    }
    return wmax;
}

int findPmax(const vector<Item>& items) {
    int pmax = 0;
    for (const auto& item : items) {
        if (item.profit > pmax) {
            pmax = item.profit;
        }
    }
    return pmax;
}

KnapsackInstance reduceToBalanced(const KnapsackInstance& original) {
    vector<Item> sortedItems = original.items;
    sort(sortedItems.begin(), sortedItems.end(), [](const Item &a, const Item &b){
        return a.profitToWeightRatio() > b.profitToWeightRatio();
    });


    //find rho (p) from the prefix solution
    //prefix solution: items {1,..,j} for maximum j such that w1+...+wj <= t (original capacity)
    //rho = profit-to-weight ratio of least profitable (last) item in prefix solution
    int currentWeight = 0;
    int j = 0;
    double rho = 0.0;

    for (; j < sortedItems.size(); ++j) {
        if (currentWeight + sortedItems[j].weight > original.capacity) {
            break;
        }
        currentWeight += sortedItems[j].weight;
    }

    if (j > 0 && j <= sortedItems.size()) {
        rho = sortedItems[j-1].profitToWeightRatio();
    }

    //partition items into good, medium and bad
    vector<Item> good;
    vector<Item> medium;
    vector<Item> bad;

    for (const auto& item : sortedItems) {
        double ratio = item.profitToWeightRatio();
        if (ratio > 2*rho) {
            good.push_back(item);
        }
        else if (ratio < rho/2) {
            bad.push_back(item);
        }
        else { medium.push_back(item);}
    } 

    cout << "good items: " << endl;
    for (const auto& item : good) {
        cout << item << endl;
    }

    cout << "Medium items: " << endl;
    for (const auto& item : medium) {
        cout << item << endl;
    }

    cout << "bad items: " << endl;
    for (const auto& item : bad) {
        cout << item << endl;
    }

    int newCapacity = 0;
    int weightTracker = 0;
    for (const auto& item : sortedItems) {
        if (weightTracker + item.weight > original.capacity) {
            break;
        }
        weightTracker += item.weight;
        if (item.profitToWeightRatio() < 2*rho && item.profitToWeightRatio() > rho/2) {
            newCapacity += item.weight;
        }
    }

    KnapsackInstance balancedInstance = {medium, newCapacity};
    return balancedInstance;
}
