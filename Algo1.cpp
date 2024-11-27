#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;


double opt_tilde(int n, double t, vector<double> weight, vector<double> profit) {
    
    
    vector<pair<double, int>> profit_weight_ratios(n);
    for (int i = 0; i < n; i++) {
        profit_weight_ratios[i] = {profit[i] / weight[i], i};  //store ratio and original index
    }

    sort(profit_weight_ratios.begin(), profit_weight_ratios.end(), greater<pair<double, int>>());

    double total_profit = 0.0;
    double total_weight = 0.0;  
    for (int i = 0; i < n; ++i) {
        int index = profit_weight_ratios[i].second;
        if (total_weight + weight[index] <= t) {    //if full item fits, take completely
            total_weight += weight[index];
            total_profit += profit[index];
        } else {                                    //if not, then only a fraction
            double remaining_capacity = t - total_weight;
            total_profit += profit[index] * (remaining_capacity / weight[index]);
            break; 
        }
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