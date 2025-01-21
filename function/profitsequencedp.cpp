#include <iostream>
#include <vector>
using namespace std;

struct Item {
    int value;
    int weigth;
};

int kSackDP(int capacity, vector<Item>& items, int index, vector<vector<int>>& memo) {
     
     //base condition
     if (index < 0) {
        return 0;
     }

     if (memo[index][capacity] != -1) {
        return memo[index][capacity];
     }

     //store the value in the table before return
     if(items[index].weigth > capacity) {
        memo[index][capacity] = kSackDP(capacity, items, index-1, memo);
     } else {
        memo[index][capacity] = max(items[index].value + kSackDP(capacity-items[index].weigth, items, index-1, memo),
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
    vector<int> pSequence;
    for (int k=0;k < capacity+1; ++k) {
        pSequence[k] = kSack(k, group);
        return pSequence;
    } 
}

vector<int> extractArr(const <int>& arr, const vector<int> indices, const vector<int> values);
//count of groups = 2^q
vector<int> computeD(vector<Item>& group) {
    int k;
    vector<int> profSeq = computeProfitSequence(group,k);
    vector<int> D = extractArr(profSeq, W, P);
    return D;
}

int main() {
    vector<Item> items = {{3, 2}, {4, 3}, {5, 4}, {7, 6}};
    int capacity = 7;
    cout << kSack(capacity, items) << endl;
    return 0;
}