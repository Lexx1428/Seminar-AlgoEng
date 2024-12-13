#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
using namespace std;

struct Item {
    int index;
    int value;
    int weight;
};

ostream& operator<<(ostream &os, const Item &item) {
    os << "Index: " << item.index << ", Value: " << item.value << ", Weight: " << item.weight;
    return os;
}

vector<Item> generateDataset(const int &numItems, const int &maxWeight, const int &maxValue) {
    vector<Item> items;
    items.reserve(numItems);
    srand(time(0));    //seed for randomness
    for (int i = 0; i < numItems; ++i) {
        int index = i;
        int weight = rand() % maxWeight + 1;
        int value = rand() % maxValue + 1;
        items.push_back({index, value, weight});
    }
    return items;
}

void partitionGroups(const vector<Item> &items, vector<vector<Item>> &groups, int count_of_groups) {

    size_t length = items.size();
    groups.resize(count_of_groups);
    int size_of_group = length/count_of_groups;

    int remainder = length % count_of_groups;

    int item_index = 0;

    for (int i = 0; i < count_of_groups; ++i) {
        
        int current_group_size = size_of_group + (i < remainder ? 1 : 0);

        for (int j = 0; j < current_group_size; ++j) {
            groups[i].push_back(items[item_index]);
            item_index++;
        }
    }
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
        cout << "Index: " << item.index << ", Value: " << item.value << ", Weight: " << item.weight << endl;
    }

    cout << endl;

    vector<vector<Item>> groups;
    int numGroups;
    cout << "How many different groups?" << endl;
    cin >> numGroups;
    partitionGroups(dataset, groups, numGroups);

    for (int i = 0; i < groups.size(); ++i) {
        cout << "Group " << i << ": " << endl;
        for (int j = 0; j < groups[i].size(); ++j) {
            cout << groups[i][j] <<  " " << endl;
        }
        cout << endl;
    }
    return 0;

}