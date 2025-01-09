#include <iostream>
using namespace std;

int knapsack(int capacity, int weight[], int value[], int n){
    cout << "knapsack(" << capacity << "," << n << ")" << endl;  
    if (capacity == 0 || n == 0){ //base case
        return 0;
    }

    else if (weight[n] > capacity){ //exclude current item since weight > capacity
        return knapsack(capacity, weight, value, n - 1);
    }

    else{
        int include_item = value[n] + knapsack(capacity - weight[n], weight, value, n - 1);
        int exclude_item = knapsack(capacity, weight, value, n - 1);

        return max(include_item, exclude_item);

    }

};

int main(){

    int weights[] = {2, 4, 6};
    int value[] = {10, 14, 20};
    int weight = 6;

    int length = sizeof(weights) / sizeof(*weights);
    int highest = knapsack(weight, weights, value, length);

    cout << " " << endl;

    return 0;
}

