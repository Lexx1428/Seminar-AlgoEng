#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


double opt_tilde(int n, double t, vector<double> weight, vector<double> profit) {
    
    
    vector<pair<double, int>> profit_weight_ratios(n);
    for (int i = 0; i < n; ++i) {
        profit_weight_ratios[i] = {profit[i] / weight[i], i};  // Store ratio and original index
    }

    sort(profit_weight_ratios.begin(), profit_weight_ratios.end(), greater<pair<double, int>>());

    double total_profit = 0.0;
    double total_weight = 0.0;  
    for (int i = 0; i < n && total_weight + weight[profit_weight_ratios[i].second] <= t; ++i) {
        int index = profit_weight_ratios[i].second;  
        total_weight += weight[index];  
        total_profit += profit[index];  
    }

    return total_profit;
}

int main() {

    
    int n = 4;    //number of items
    double t = 7; //weight
    double result= opt_tilde(n, t, {1,2,3,4}, {1,1,1,1});
    cout<< "t = " << t << endl;
    cout<< "opt_tilde = " << result <<endl;

    
    return 0;   
}