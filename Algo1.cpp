#include <iostream>
#include <vector>
using namespace std;


vector<double> opt_tilde(int n, int t, vector<int> weight, vector<int> profit) {
    
    vector<double> profit_weight_ratio(n);

    for(int i=0; i<n; i++){
        profit_weight_ratio[i] = profit[i] / weight[i];
    }

    return profit_weight_ratio;
}

int main() {

    cout<<"Does this work \n";

    vector<double> ratio = opt_tilde(4, 7, {1,2,3,4}, {1,2,3,4});
    cout<<"Does this work2 \n";
    for(int i=0; i<ratio.size();i++){
        cout << ratio[i] << endl;
    }
    

    cout<<"Why not printing";
    
    return 0;   
}