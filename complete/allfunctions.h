#ifndef ALLFUNCTIONS_H
#define ALLFUNCTIONS_H

#include "types.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <utility>
#include <random>

using namespace std;

void partitionGroups(const vector<Item> &items, vector<vector<Item>> &groups, int count_of_groups);

int kSackDP(int capacity, vector<Item>& items, int index, vector<vector<int>>& memo);

int kSack(int capacity, vector<Item>& items);

vector<int> computeProfitSequence(vector<Item>& group, int capacity);

vector<int> computeSubarray(const std::vector<int> &input, const std::pair<int, int> &indices, const std::pair<int, int> &values);

int computeTildeOPT(int t, vector<Item> items);

vector<int> maxPlusConv(const vector<int> &a, const vector<int> &b);

void print2DVector(const vector<vector<int>>& vec);

int findWmax(const vector<Item>& items);

int findPmax(const vector<Item>& items);

vector<Item> generateDataset(const int &numItems, const int &maxWeight, const int &maxValue);


KnapsackInstance reduceToBalanced(const KnapsackInstance& original);

vector<int> Algo1_half(const vector<Item>& items, int t);

vector<Item> generateBalancedKnapsackItems(int num_items, int max_profit, int max_weight, int knapsack_capacity);


#endif