#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Item {
    int value;
    int weight;
};

ostream& operator<<(ostream &os, const Item &item) {
    os << "Value: " << item.value << ", Weight: " << item.weight;
    return os;
}

int getMaxWeight(const vector<Item>& items) {
    if (items.empty()) {
        return -1;  
    }

    int maxWeight = items[0].weight;  

    for (const auto& item : items) {
        if (item.weight > maxWeight) {
            maxWeight = item.weight; 
        }
    }

    return maxWeight;
}

int getMaxValue(const vector<Item>& items) {
    if (items.empty()) {
        return -1; 
    }

    int maxValue = items[0].value; 

    for (const auto& item : items) {
        if (item.value > maxValue) {
            maxValue = item.value;  
        }
    }

    return maxValue;
}

double opt_tilde(int n, double t, vector<Item> items) {
    
    
    vector<pair<double, int>> value_weight_ratios(n);
    for (int i = 0; i < n; i++) {
        value_weight_ratios[i] = {static_cast<double>(items[i].value) / items[i].weight, i};  //store ratio and original index
    }

    sort(value_weight_ratios.begin(), value_weight_ratios.end(), greater<pair<double, int>>());

    double total_value = 0.0;
    double total_weight = 0.0;  
    int i; 
    for (i = 0; i < n; ++i) {
        int index = value_weight_ratios[i].second;
        if (total_weight + items[index].weight <= t) {  // If full item fits, take it completely
            total_weight += items[index].weight;
            total_value += items[index].value;
        } else {
            break;  // Stop when capacity is exceeded
        }
    }

    // Include the whole (i* + 1)-th item if it exists
    if (i < n) {  // Check if there is an (i* + 1)-th item
        int index = value_weight_ratios[i].second;
        total_value += items[index].value;  // Add the full value of the (i* + 1)-th item
    }

    return total_value;
}

int compute_q(int n, double t, vector<Item> items) {
    double w_max = getMaxWeight(items);
    double p_max = getMaxValue(items);

    double opt_tild = opt_tilde(n, t, items);

    double min_value = min(t/w_max, opt_tild/p_max);

    int q = 0;

    while((1 << q) <= min_value) {
        q++;
    }
    
    return q - 1;
}

int main() {

    vector<Item> item = {{1,1}, {1,2}, {1,3}, {1,4}};
    int n = 4;    //number of items
    double t = 7; //weight
    double result= opt_tilde(n, t, item);
    cout<< "t = " << t << endl;
    cout<< "opt_tilde = " << result <<endl;

    int q = compute_q(n, t, item);
    cout<< "q = " << q << endl;

    
    return 0;   
}