#include "types.h"
#include "allfunctions.h"



int main() {

    cout << "start of program" << endl;
    KnapsackInstance test = {{{8,1}, {7,7}, {5,6}, {6,9}, {3,5}, {1,7}}, 25};

    cout << "Original instance: " << endl;
    cout << "Capacity: " << test.capacity << endl;
    for (const auto& item : test.items) {
        cout << "Profit: " << item.profit << " weight: " << item.weight << endl;
    }

    KnapsackInstance reduced = reduceToBalanced(test);
    cout << "reduced capacity: " << reduced.capacity << endl;

    for (const auto& item : reduced.items) {
        cout << "Profit: " << item.profit << " weight: " << item.weight << endl;
    }

    cout << "end of program" << endl;


    return 0;
}