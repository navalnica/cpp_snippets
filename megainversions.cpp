/*
 * Solution to Megainversion problem:
 * Find number of megainversions given a permutation of length `n`.
 * 
 * Using 2 segment trees to efficiently find the answers.
 *
 * --- Input ---
 * First line: `n` - integer.
 * Next `n` lines contain one integer each
 * and together they define permutation P of length `n`.
 *
 * --- Output ---
 * Number of megainversions:
 * such triplets (P_i, P_j, P_k) that
 * i < j < k and P_i > P_j > P_k.
 * 
 */


#include <iostream>
#include <vector>
#include <unordered_map>
#include <fstream>


using namespace std;

struct Node {
    int sum = 0;

    Node() {
        sum = 0;
    }

    explicit Node(int sum) {
        this->sum = sum;
    }
};


struct Tree {
    int size = 0;
    vector<Node> tree;

    explicit Tree(vector<int> &vals) {
        size = vals.size();
        tree = vector<Node>(size * 4);
        _build(vals, 1, 0, size - 1);
    }

private:
    void _build(vector<int> &vals, int cur_tree_ix, int left, int right) {
        if (left == right) {
            tree[cur_tree_ix] = Node(vals[left]);
        } else {
            int middle = (left + right) / 2;
            _build(vals, cur_tree_ix * 2, left, middle);
            _build(vals, cur_tree_ix * 2 + 1, middle + 1, right);

            int child_sum = tree[cur_tree_ix * 2].sum + tree[cur_tree_ix * 2 + 1].sum;
            tree[cur_tree_ix].sum = child_sum;
        }
    }

    int _sum(int v, int cur_left, int cur_right, int query_left, int query_right) {
        if (query_left > query_right)
            return 0;
        if (query_left == cur_left && query_right == cur_right)
            return tree[v].sum;

        int cur_middle = (cur_left + cur_right) / 2;
        int sum_left = _sum(v * 2, cur_left, cur_middle, query_left, min(query_right, cur_middle));
        int sum_right = _sum(v * 2 + 1, cur_middle + 1, cur_right, max(query_left, cur_middle + 1), query_right);
        return sum_left + sum_right;
    }

    void _update(int v, int cur_left, int cur_right, int ix, int new_val) {
        if (cur_left == cur_right) {
            tree[v].sum = new_val;
        } else {
            int middle = (cur_left + cur_right) / 2;
            if (ix <= middle) {
                _update(v * 2, cur_left, middle, ix, new_val);
            } else {
                _update(v * 2 + 1, middle + 1, cur_right, ix, new_val);
            }
            int child_sum = tree[v * 2].sum + tree[v * 2 + 1].sum;
            tree[v].sum = child_sum;
        }
    }

    void _increment(int v, int cur_left, int cur_right, int ix) {
        if (cur_left == cur_right) {
            tree[v].sum++;
        } else {
            int middle = (cur_left + cur_right) / 2;
            if (ix <= middle) {
                _increment(v * 2, cur_left, middle, ix);
            } else {
                _increment(v * 2 + 1, middle + 1, cur_right, ix);
            }
            tree[v].sum++;
        }
    }

public:
    int get_sum(int query_left, int query_right) {
        return _sum(1, 0, size - 1, query_left, query_right);
    }


    void update(int ix, int new_val) {
        _update(1, 0, size - 1, ix, new_val);
    }

    void increment(int ix) {
        _increment(1, 0, size - 1, ix);
    }
};

struct Element {
    int val;
    int n_larger_to_left;
    int n_lower_to_right;

    Element() : val(0), n_larger_to_left(0), n_lower_to_right(0) {}
};

int main() {
    int n = 0;

    ifstream fin("input.txt");
    ofstream cout("output.txt");

    int max_values = 300'001;
    vector<int> zeros(max_values, 0);

    Tree prefix_tree(zeros);
    Tree postfix_tree(zeros);

    fin >> n;
    vector<Element> perm(n);
    for (int i = 0; i < n; ++i) {
        fin >> perm[i].val;
    }

    for (int i = 0; i < perm.size() - 1; ++i) {
        perm[i].n_larger_to_left = prefix_tree.get_sum(perm[i].val + 1, max_values - 1);
        prefix_tree.increment(perm[i].val);
    }
    for (int i = perm.size() - 1; i >= 1; --i) {
        perm[i].n_lower_to_right = postfix_tree.get_sum(0, perm[i].val - 1);
        postfix_tree.increment(perm[i].val);
    }

    long long ans = 0;
    for (int i = 1; i < perm.size() - 1; ++i) {
        ans += (long long) perm[i].n_lower_to_right * perm[i].n_larger_to_left;
    }

    cout << ans;

    return 0;
}
