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
    int i; // Use this variable to keep track of the last fully included index
    for (i = 0; i < n; ++i) {
        int index = profit_weight_ratios[i].second;
        if (total_weight + weight[index] <= t) {  // If full item fits, take it completely
            total_weight += weight[index];
            total_profit += profit[index];
        } else {
            break;  // Stop when capacity is exceeded
        }
    }

    // Include the whole (i* + 1)-th item if it exists
    if (i < n) {  // Check if there is an (i* + 1)-th item
        int index = profit_weight_ratios[i].second;
        total_profit += profit[index];  // Add the full profit of the (i* + 1)-th item
    }

    return total_profit;
}

int compute_q(int n, double t, vector<double> weight, vector<double> profit) {
    double w_max = *max_element(weight.begin(), weight.end());
    double p_max = *max_element(profit.begin(), profit.end());

    double opt_tild = opt_tilde(n, t, weight, profit);

    double min_value = min(t/w_max, opt_tild/p_max);

    int q = 0;

    while((1 << q) <= min_value) {
        q++;
    }
    
    return q - 1;
}

int main() {

    
    int n = 4;    //number of items
    double t = 7; //weight
    double result= opt_tilde(n, t, {1,2,3,4}, {1,1,1,1});
    cout<< "t = " << t << endl;
    cout<< "opt_tilde = " << result <<endl;

    int q = compute_q(n, t, {1,2,3,4}, {1,1,1,1});
    cout<< "q = " << q << endl;

    
    return 0;   
}