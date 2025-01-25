#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
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


//compute subarray: D <- A[I;V], A monotone non-decreasing array
//If there exist no index i ∈ I with A[i] ∈ V , then set D to the empty array. Otherwise,
//let imin := min{i ∈ I : A[i] ∈ V } and imax := max{i ∈ I : A[i] ∈ V }, and set D to the
//subarray A[imin . . . imax]. Note that since A is monotone, for every i ∈ {imin, . . . , imax} we
//have A[i] ∈ V . Thus A[I; V ] returns the subarray of A with indices in I and values in V .
std::vector<int> computeSubarray(const std::vector<int> &input, const std::pair<int, int> &indices, const std::pair<int, int> &values) {
    std::vector<int> subarray;

    // Ensure the indices interval is within the bounds of the array.
    int start = indices.first;
    int end = std::min(indices.second, static_cast<int>(input.size()) - 1);

    for (int i = start; i <= end; ++i) {
        if (input[i] >= values.first && input[i] <= values.second) {
            subarray.push_back(input[i]);
        }
    }

    return subarray;
}





int main() {

    vector<int> input = {1,2,3,4,5,6,7,8,9,10};
    pair<int, int> indices = {3, 7};
    pair<int,int> values = {5, 15};

    vector<int> test_subarray = computeSubarray(input, indices, values);

    for (auto i : test_subarray) {
        cout << i << " " << endl;
    }

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