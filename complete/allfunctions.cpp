#include "allfunctions.h"
#include "types.h"


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

std::vector<int> computeSubarray(const std::vector<int> &input, const std::pair<int, int> &indices, const std::pair<int, int> &values) {
    std::vector<int> subarray;

    // Ensure the indices interval is within the bounds of the array.
    int start = std::max(0, indices.first);
    int end = std::min(indices.second, static_cast<int>(input.size()) - 1);

    for (int i = start; i <= end; ++i) {
        if (input[i] >= values.first && input[i] <= values.second) {
            subarray.push_back(input[i]);
        }
    }

    return subarray;
}



int computeTildeOPT(int t, vector<Item> items) {
    
    int n = items.size();
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
        cout << "--end of row--"<< endl; // Newline after each row
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

vector<Item> generateDataset(const int &numItems, const int &maxWeight, const int &maxValue) {
    vector<Item> items;
    items.reserve(numItems);
    srand(time(0));    //seed for randomness
    for (int i = 0; i < numItems; ++i) {
        int weight = rand() % maxWeight + 1;
        int value = rand() % maxValue + 1;
        items.push_back({value, weight});
    }
    return items;
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

vector<int> Algo1(const vector<Item>& items, int t){
    int n = items.size();
    int wMax = findWmax(items);
    int pMax = findWmax(items);

    int opt = computeTildeOPT(t ,items);

    //Determine number of partitions
    int q = log2(min(t / wMax, opt / pMax));
    if (q < 0) q = 0;
    int numPartitions = 1 << q;

    int eta = 17 * log10(n); 
    int deltaW = t * wMax;
    int deltaP = opt * pMax;

    //Split items into 2^q groups uniformly at random
    vector<vector<Item>> groups;
    partitionGroups(items, groups, numPartitions);

    //Determine interval for each partition
    int WqMin = t / numPartitions - sqrt(deltaW/numPartitions) * eta;
    int WqMax = t / numPartitions + sqrt(deltaW/numPartitions) * eta;
    int PqMin = opt / numPartitions - sqrt(deltaP/numPartitions) * eta;
    int PqMax = opt / numPartitions + sqrt(deltaP/numPartitions) * eta;

    pair<int,int> Wq(WqMin,WqMax);
    pair<int,int> Pq(PqMin,PqMax);

    pair<int,int> wStar(0, WqMax);
    pair<int,int> pStar(0, PqMax);

    vector<vector<int>> Dq(numPartitions);
    vector<vector<int>> Cq(numPartitions);
    vector<vector<int>> CCq(numPartitions);
    
    //Compute profit sequence for each partition
    for (int j = 0; j < numPartitions; ++j) {
        Dq[j] = computeProfitSequence(groups[j], WqMax);
        Cq[j] = computeSubarray(Dq[j], wStar, pStar);
        CCq[j] = computeSubarray(Cq[j], Wq, Pq);
    }

    //Compute max-plus convolution in a tree-like fashion 
    for (int level = q - 1; level >= 0; --level) {
        int WlMax = t / (1 << level) + sqrt(deltaW/numPartitions) * eta ;
        int PlMax = opt / (1 << level) + sqrt(deltaP/numPartitions) * eta;
        int WlMin = t / (1 << level) - sqrt(deltaW/numPartitions) * eta;
        int PlMin = opt / (1 << level) - sqrt(deltaP/numPartitions) * eta;

        pair<int, int> Wl(WlMin, WlMax);
        pair<int, int> Pl(PlMin, PlMax);

        vector<vector<int>> next_level_arrays((1 << level));

        for (int j = 0; j < (1 << level); ++j) {
            vector<int> convolved = maxPlusConv(CCq[2 * j], CCq[2 * j + 1]);
            vector<int> filtered = computeSubarray(convolved, Pl, Wl);
            next_level_arrays[j] = filtered;
        }

        // Safely update CCq
        CCq.clear();
        CCq = next_level_arrays;
    }


    int interval_Tmax = t + sqrt(t * wMax);
    int interval_Tmin = t - sqrt(t * wMax);

    int interval_Pmax = opt + sqrt(opt * pMax);
    int interval_Pmin = opt - sqrt(opt * pMax);

    pair<int, int> interval_T(interval_Tmin,t);
    pair<int, int> interval_P(interval_Pmin, interval_Pmax);


    CCq[0] = computeSubarray(CCq[0], interval_T, interval_P);
    return CCq[0];
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

        items.push_back({profit, weight});
    }

    return items;
}