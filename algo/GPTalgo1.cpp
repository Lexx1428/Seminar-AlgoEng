#include <iostream>
#include <vector>
#include <cmath>
#include <algorithm>
#include <random>

struct Item {
    int weight;
    int profit;
};

struct Interval {
    double lower;
    double upper;
};

std::vector<std::vector<Item>> partitionItems(const std::vector<Item>& items, int q) {
    std::vector<std::vector<Item>> groups(1 << q);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, (1 << q) - 1);

    for (const auto& item : items) {
        int group = dis(gen);
        groups[group].push_back(item);
    }
    return groups;
}

std::vector<Interval> computeIntervals(double base, double delta, double eta, int q) {
    std::vector<Interval> intervals;
    for (int i = 0; i <= q; ++i) {
        double factor = std::pow(2, q - i);
        double lower = base / factor - std::sqrt(delta / factor) * eta;
        double upper = base / factor + std::sqrt(delta / factor) * eta;
        intervals.push_back({lower, upper});
    }
    return intervals;
}

std::vector<int> maxConv(const std::vector<int>& a, const std::vector<int>& b) {
    size_t n = a.size(), m = b.size();
    std::vector<int> result(n + m - 1, std::numeric_limits<int>::min());

    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < m; ++j) {
            result[i + j] = std::max(result[i + j], a[i] + b[j]);
        }
    }
    return result;
}

void knapsackAlgorithm(const std::vector<Item>& items, double t) {
    int w_max = 0, p_max = 0;
    for (const auto& item : items) {
        w_max = std::max(w_max, item.weight);
        p_max = std::max(p_max, item.profit);
    }

    double opt = p_max;
    int q = static_cast<int>(std::log2(std::min(t / w_max, opt / p_max)));
    double eta = 17 * std::log(items.size());
    double deltaW = t * w_max;
    double deltaP = opt * p_max;

    std::vector<std::vector<Item>> groups = partitionItems(items, q);

    std::vector<Interval> W_intervals = computeIntervals(t, deltaW, eta, q);
    std::vector<Interval> P_intervals = computeIntervals(opt, deltaP, eta, q);

    std::vector<std::vector<int>> D(groups.size());
    for (size_t j = 0; j < groups.size(); ++j) {
        D[j] = std::vector<int>(W_intervals.size(), 0); // Placeholder
    }

    for (int l = q - 1; l >= 0; --l) {
        std::vector<Interval> W_l = computeIntervals(t, deltaW, eta, l);
        std::vector<Interval> P_l = computeIntervals(opt, deltaP, eta, l);

        std::vector<std::vector<int>> newD(1 << l);
        for (int j = 0; j < (1 << l); ++j) {
            newD[j] = maxConv(D[2 * j], D[2 * j + 1]);
        }
        D = newD;
    }

    Interval finalW = {t - std::sqrt(t * w_max), t + std::sqrt(t * w_max)};
    Interval finalP = {opt - std::sqrt(opt * p_max), opt + std::sqrt(opt * p_max)};

    std::cout << "Final result: ";
    for (const auto& val : D[0]) {
        std::cout << val << " ";
    }
    std::cout << std::endl;
}

int main() {
    std::vector<Item> items = {{3, 20}, {2, 100}, {4, 30}};
    double t = 50.0;

    knapsackAlgorithm(items, t);

    return 0;
}
