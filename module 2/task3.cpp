/* Постройте B-дерево минимального порядка t и выведите его по слоям.
 * В качестве ключа используются числа, лежащие в диапазоне 0..232 -1
 * Требования:
 * B-дерево должно быть реализовано в виде шаблонного класса.
 * Решение должно поддерживать передачу функции сравнения снаружи
*/

#include <vector>
#include <cassert>
#include <iostream>
#include <queue>


template<typename T>
struct DefaultComparator {
    int operator()(const T &l, const T &r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template<typename Key, typename Comparator = DefaultComparator<Key>>
class BTree {
public:
    struct Node {
        explicit Node(bool leaf) : leaf(leaf) {}

        ~Node() {
            for (Node *child: children) {
                delete child;
            }
        }

        bool leaf;
        std::vector<Key> keys; //t-1, 2t-1, в корне от 1 до 2t-1
        std::vector<Node *> children; // 2t
    };

    BTree(size_t min_degree, Comparator comparator = Comparator()) : root(nullptr), t(min_degree) {
        compare = comparator;
        assert(min_degree >= 2);
    }

    ~BTree() {
        if (root)
            delete root;
    }

    void insert(const Key &key) {
        if (!root) {
            root = new Node(true);
        }
        if (is_node_full(root)) {
            Node *newRoot = new Node(false);
            newRoot->children.push_back(root);
            root = newRoot;
            split(root, 0);
        }

        insert_non_full(root, key);
    }

    bool find(const Key &key) {
        return find_aux(root, key);
    }

    std::vector<std::vector<Key>> getKeysByLayer() {
        std::queue<Node *> nextLayer;
        std::vector<std::vector<Key>> result;
        nextLayer.push(root);
        while (!nextLayer.empty()) {
            std::queue<Node *> thisLayer;
            thisLayer = nextLayer;
            nextLayer = std::queue<Node *>(); // nextLayer is now empty
            std::vector<Key> thisLayerVector;
            while (!thisLayer.empty()) {
                Node *popped = thisLayer.front();
                thisLayer.pop();
                for (auto i: popped->keys)
                    thisLayerVector.push_back(i);
                if (popped->children.size() != 0) {
                    for (auto i: popped->children) {
                        nextLayer.push(i);
                    }
                }
            }
            result.push_back(thisLayerVector);
        }
        return result;
    }

private:
    bool is_node_full(Node *node) {
        return node->keys.size() == 2 * t - 1;
    }

    bool find_aux(Node *node, const Key &key) {
        int i = 0;
        while (i < node->keys.size() && compare(key, node->keys[i]) >= 1)
            i++;
        if (i < node->keys.size() && compare(key, node->keys[i]) == 0)
            return true;
        else if (node->leaf)
            return false;
        else
            return find_aux(node->children[i], key);
    }

    void split(Node *x, size_t index) {
        Node *y = x->children[index];
        Node *z = new Node(y->leaf);
        z->keys.resize(t - 1);
        for (size_t j = 0; j < t - 1; j++) {
            z->keys[j] = y->keys[j + t];
        }

        if (!y->leaf) {
            z->children.resize(t);
            for (size_t j = 0; j < t; j++) {
                z->children[j] = y->children[j + t];
            }
        }
        x->children.resize(x->children.size() + 1);
        for (long j = x->children.size() - 2; j >= (long) index + 1; j--) {
            x->children[j + 1] = x->children[j];
        }
        x->children[index + 1] = z;
        x->keys.resize(x->keys.size() + 1);
        for (long j = x->keys.size() - 2; j >= (long) index; j--) {
            x->keys[j + 1] = x->keys[j];
        }
        x->keys[index] = y->keys[t - 1];
        y->keys.resize(t - 1);
        if (!y->leaf)
            y->children.resize(t);
    }


    void insert_non_full(Node *node, const Key &key) {
        int pos = node->keys.size() - 1;
        if (node->leaf) {
            node->keys.resize(node->keys.size() + 1);
            while (pos >= 0 && compare(key, node->keys[pos]) <= -1) {
                node->keys[pos + 1] = node->keys[pos];
                pos--;
            }
            node->keys[pos + 1] = key;
        } else {
            while (pos >= 0 && compare(key, node->keys[pos]) <= -1) {
                pos--;
            }

            if (is_node_full(node->children[pos + 1])) {
                split(node, pos + 1);
                if (compare(key, node->keys[pos + 1]) >= 1) {
                    pos++;
                }
            }
            insert_non_full(node->children[pos + 1], key);
        }
    }

    Comparator compare;
    Node *root;
    size_t t; // minimum degree

};

void run(std::istream &in, std::ostream &out) {
    size_t t;
    in >> t;
    BTree<long> tree(t);
    long value;
    while (in >> value) {
        tree.insert(value);
    }
    std::vector<std::vector<long>> result;
    result = tree.getKeysByLayer();
    for (auto i: result) {
        for (auto j: i) {
            out << j << " ";
        }
        out << "\n";
    }
}

int main() {
    run(std::cin, std::cout);
}