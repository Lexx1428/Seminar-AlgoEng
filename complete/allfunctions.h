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

void partitionGroups(const vector<Item> &items, vector<vector<Item>> &groups, int count_of_groups) {}

int kSackDP(int capacity, vector<Item>& items, int index, vector<vector<int>>& memo) {}

int kSack(int capacity, vector<Item>& items) {}

vector<int> computeProfitSequence(vector<Item>& group, int capacity) {}

vector<int> computeSubarray(const vector<int> &input, const pair<int, int> &indices, const pair<int, int> values) {}

int computeTildeOPT(int n, double t, vector<Item> items) {}

vector<int> maxPlusConv(const vector<int> &a, const vector<int> &b) {}

void print2DVector(const vector<vector<int>>& vec) {}

int findWmax(const vector<Item>& items) {}

int findPmax(const vector<Item>& items) {}

KnapsackInstance reduceToBalanced(const KnapsackInstance& original) {}


#endif