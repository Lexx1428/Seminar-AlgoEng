#include "types.h"
#include "allfunctions.h"



int main() {
    int num_items = 300;
    int max_weight = 20;
    int max_profit = 20;
    int t = 600;
    
    vector<Item> items = generateDataset(num_items, max_profit, max_weight);


    
    vector<int> opt;
    KnapsackInstance test = {items, t};

    //KnapsackInstance reduced = reduceToBalanced(test);
    
    /*
    cout << "reduced capacity: " << reduced.capacity << endl;
    for (const auto& item : reduced.items) {
        cout << "Profit: " << item.profit << " weight: " << item.weight << " ratio: " << item.profitToWeightRatio() << endl;
    }
    */

    opt = Algo1_half(items, t);

    /*
    for(const auto& elem : opt){
        cout << elem << " ";
    }
    */

    //measureRuntime(items, t);
    cout << endl;
    cout << "num of items (original) = " << num_items <<endl;
    //cout << "num of items (reduced) = " << reduced.items.size() << endl;
    cout << endl;
    cout << "capacity (original)= " << t << endl;
    //cout << "capacity (reduced)= " << reduced.capacity << endl;
    cout << endl;
    cout << "OPT tilde (original)= " << computeTildeOPT(num_items, t, items)<< endl;
    cout << "DP opt (original)= " << kSack(t, items) << endl;
    cout << endl;
    //cout << "OPT tilde (reduced)= " << computeTildeOPT(reduced.items.size(), reduced.capacity, reduced.items) << endl;
    //cout << "DP opt (reduced)= " << kSack(reduced.capacity, reduced.items);

    return 0;
}