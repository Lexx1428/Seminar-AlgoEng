#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

struct Item {
    int weight;
    int value;
};

int kp_BruteForce(const vector<Item>& items, int capacity, int index) {

    //base case: no items left or no capacity left
    if (index == items.size() || capacity == 0){
        return 0;
    }

    //int binary_string[items.size()];

    // Case 1: Exclude the current item and move to the next
    int exclude = kp_BruteForce(items, capacity, index + 1);

    // Case 2: Include the current item (if it fits into the knapsack)
    int include = 0;
    if (items[index].weight <= capacity) {
        include = items[index].value + kp_BruteForce(items, capacity - items[index].weight, index + 1);
    }

    return max(include, exclude);
}

int main() {

    vector<Item> items = {    // {weight,value}
        {2, 3},
        {3,4},
        {4,5},
        {6,7}
    };

    int capacity = 7;
    int max_value = kp_BruteForce(items, capacity, 0);
    cout << "The max value is: " << max_value << endl;



    return 0;
}
