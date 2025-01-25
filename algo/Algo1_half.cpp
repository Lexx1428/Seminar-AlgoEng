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

vector<int> Algo1_half(const vector<Item>& items, int t){
    int n = items.size();
    int wMax = 0, pMax = 0;
    for (const auto& item : items) {
        wMax = max(wMax, item.weight);
        pMax = max(pMax, item.profit);
    }

    cout << "pMax = " << pMax << endl; 
    cout << "wMax = " << wMax << endl;
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
    print2DVector(Dq);

    cout << "Dq :" << endl; 
    print2DVector(Cq);
    
    cout << "CCq :" << endl;
    print2DVector(CCq);
    
    cout << "Ccq length = " << CCq.size() << endl;
    cout << "Dq length = " << Dq.size() << endl;
    cout << "END PROFIT SEQ" << endl;
    cout << endl;
    
    cout << "START MAXCONV" << endl;
    for (int level = q - 1; level >= 0; --level) {
        int WlMax = t / (1 << level);
        int PlMax = opt / (1 << level);
        cout << "WlMax = " << WlMax << endl;
        cout << "PlMax = " << PlMax << endl;

        pair<int, int> Wl(0, WlMax);
        pair<int, int> Pl(0, PlMax);

        vector<vector<int>> next_level_arrays((1 << level));
        cout << "next level array length = " << next_level_arrays.size() << endl;

        for (int j = 0; j < (1 << level); ++j) {
            cout << "Inside inner for loop, j = " << j;
            vector<int> convolved = maxPlusConv(CCq[2 * j], CCq[2 * j + 1]);
            //vector<int> filtered = computeSubarray(convolved, Pl, Wl);
            /*
            for (const auto& num: filtered){
                cout << num << " ";
            }
            */
            cout << endl;
            cout << "convolved length = " << convolved.size() << endl;
            //cout << "filtered length = " << filtered.size() << endl;
 
            next_level_arrays[j] = convolved;
        }
        cout << endl;

        // Safely update CCq
        CCq.clear();
        CCq = next_level_arrays; // Ensure no unexpected optimizations
    }


    cout << "END MAXCONV" << endl;
    
    cout << "CCq length = " << CCq.size() << endl;
    cout << "CCq[0] length = " << CCq[0].size() << endl;
    int interval_Tmax = t + sqrt(t * wMax);
    int interval_Tmin = t - sqrt(t * wMax);

    int interval_Pmax = opt + sqrt(opt * pMax);
    int interval_Pmin = opt - sqrt(opt * pMax);

    cout << " " << endl;
    cout << "OPT TILDE = "  << opt << endl;
    cout << "interval Tmax = " << interval_Tmax << " sqrt part = " << sqrt(t * wMax) <<endl;
    cout << "interval Tmin = " << interval_Tmin << " sqrt part = " << sqrt(t * wMax)<< endl;
    cout << "interval Pmax = " << interval_Pmax << " sqrt part = " << sqrt(opt * pMax) <<endl;
    cout << "interval Pmin = " << interval_Pmin << " sqrt part = " << sqrt(opt * pMax)<< endl;

    pair<int, int> interval_T(interval_Tmin, interval_Tmax);
    pair<int, int> interval_P(interval_Pmin, interval_Pmax);

    //return CCq;

    
    cout << "CCq[0] : " << endl; 
    for(const auto& num : CCq[0]){
        cout << num << " ";
    }

    vector<int> final_opt = computeSubarray(CCq[0], interval_T, interval_P); 
;
    return final_opt;
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


    vector<Item> items = {
        {1, 56, 28},
        {2, 63, 32},
        {3, 74, 37},
        {4, 52, 26},
        {5, 68, 34},
        {6, 59, 29},
        {7, 62, 31},
        {8, 70, 36},
        {9, 48, 24},
        {10, 77, 39},
        {11, 65, 33},
        {12, 50, 25},
        {13, 58, 29},
        {14, 73, 36},
        {15, 61, 31},
        {16, 66, 34},
        {17, 72, 35},
        {18, 64, 32},
        {19, 60, 30},
        {20, 53, 27},
        {21, 69, 34},
        {22, 55, 27},
        {23, 76, 38},
        {24, 49, 24},
        {25, 67, 33},
        {26, 75, 37},
        {27, 57, 28},
        {28, 71, 36},
        {29, 54, 27},
        {30, 68, 34},
        {31, 63, 32},
        {32, 74, 37},
        {33, 52, 26},
        {34, 66, 33},
        {35, 59, 29},
        {36, 62, 31},
        {37, 70, 36},
        {38, 48, 24},
        {39, 77, 39},
        {40, 65, 33},
        {41, 50, 25},
        {42, 58, 29},
        {43, 73, 36},
        {44, 61, 31},
        {45, 66, 34},
        {46, 72, 35},
        {47, 64, 32},
        {48, 60, 30},
        {49, 53, 27},
        {50, 69, 34},
        {51, 55, 27},
        {52, 76, 38},
        {53, 49, 24},
        {54, 67, 33},
        {55, 75, 37},
        {56, 57, 28},
        {57, 71, 36},
        {58, 54, 27},
        {59, 68, 34},
        {60, 63, 32},
        {61, 74, 37},
        {62, 52, 26},
        {63, 66, 33},
        {64, 59, 29},
        {65, 62, 31},
        {66, 70, 36},
        {67, 48, 24},
        {68, 77, 39},
        {69, 65, 33},
        {70, 50, 25},
        {71, 58, 29},
        {72, 73, 36},
        {73, 61, 31},
        {74, 66, 34},
        {75, 72, 35},
        {76, 64, 32},
        {77, 60, 30},
        {78, 53, 27},
        {79, 69, 34},
        {80, 55, 27},
        {81, 76, 38},
        {82, 49, 24},
        {83, 67, 33},
        {84, 75, 37},
        {85, 57, 28},
        {86, 71, 36},
        {87, 54, 27},
        {88, 68, 34},
        {89, 63, 32},
        {90, 74, 37},
        {91, 52, 26},
        {92, 66, 33},
        {93, 59, 29},
        {94, 62, 31},
        {95, 70, 36},
        {96, 48, 24},
        {97, 77, 39},
        {98, 65, 33},
        {99, 50, 25},
        {100, 58, 29}
    };

    int t = 1000;
    vector<int> opt;
    

    opt = Algo1_half(items,t);

    /*
    for(const auto& elem : opt){
        cout << elem << " ";
    }
    */

    cout << "OPT tilde = " << computeTildeOPT(100, t, items) << endl;

}