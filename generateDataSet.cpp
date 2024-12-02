#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

struct Item {
    int value;
    int weight;
};

vector<Item> generateDataset(int numItems, int maxWeight, int maxValue) {
    vector<Item> items;
    srand(time(0));    //seed for randomness
    for (int i = 0; i < numItems; ++i) {
        int weight = rand() % maxWeight + 1;
        int value = rand() % maxValue + 1;
        items.push_back({value, weight});
    }
    return items;
}

int main() {

    int number;
    int maxWeight;
    int maxValue;

    cout << " how many items do you want to generate?" << endl;
    cin >> number;
    cout << "what should be the maxWeight?" << endl;
    cin >> maxWeight;
    cout << "what should be the max value? " << endl;
    cin >> maxValue;

    vector<Item> dataset = generateDataset(number, maxWeight, maxValue);

    cout << "Generated dataset:" << endl;
    for (const auto& item : dataset) {
        cout << "Value: " << item.value << ", Weight: " << item.weight << endl;
    }


    return 0;

}