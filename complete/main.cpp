#include "types.h"
#include "allfunctions.h"



int main() {
    int num_items = 300;
    int max_weight = 30;
    int max_profit = 30;
    int t = 600;

    vector<Item> items = generateDataset(num_items, max_profit, max_weight);
    KnapsackInstance test = {items, t};
    KnapsackInstance reduced_instance = reduceToBalanced(test);

    vector<int> opt = Algo1_half(reduced_instance.items, reduced_instance.capacity);
    cout << "Algo 1 solution = " << opt.back() << endl;
    cout << "OPT tilde = " << computeTildeOPT(reduced_instance.capacity, reduced_instance.items)<< endl;
    cout << "Dynamic Programming opt = " << kSack(reduced_instance.capacity, reduced_instance.items) << endl;

    return 0;
}