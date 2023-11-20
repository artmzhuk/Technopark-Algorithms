#include <iostream>


template<typename T>
struct DefaultComparator {
    int operator()(const T &l, const T &r) const {
        if (l < r) return -1;
        if (l > r) return 1;
        return 0;
    }
};

template<typename Key, typename Value, typename Comparator=DefaultComparator<Key> >
class AVLTree {
    struct Node {
        Node *left;
        Node *right;

        Key key;
        Value value;

        int64_t weight;
        uint8_t height;

        Node(const Key &key, const Value &value) :
                left(nullptr), right(nullptr),
                key(key), value(value), height(1), weight(1) {
        }

        ~Node() {
            //delete left;
            //delete right;
        }
    };

public:
    AVLTree(Comparator comp = Comparator()) :
            root(nullptr),
            items_count(0),
            comp(comp) {

    }

    ~AVLTree() {
        //delete root;
    }

    size_t size() const { return items_count; }

    Value *find(const Key &key) {
        return find_aux(key, root);
    }

    void insert(const Key &key, const Value &value) {
        root = insert_aux(key, value, root);
    }

    void erase(const Key &key) {
        root = erase_aux(key, root);
    }

    Value kth_element(int64_t index) {
        if (!root) {
            return 0;
        }
        return kth_element_aux(root, index)->value;
    }

private:
    Node *kth_element_aux(Node *node, int64_t index) {
        int64_t delta = index - weight(node->left);
        if (delta == 0) {
            return node;
        } else if (delta < 0) {
            return kth_element_aux(node->left, index);
        } else {
            return kth_element_aux(node->right, delta - 1);
        }
    }

    Value *find_aux(const Key &key, Node *node) {
        if (!node) {
            return nullptr;
        }
        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { //key < node->key
            return find_aux(key, node->left);
        } else if (cmp_res == 1) { //key > node->key
            return find_aux(key, node->right);
        }
        //key == node->key
        return &node->value;
    }

    Node *insert_aux(const Key &key, const Value &value, Node *node) {
        if (!node) {
            items_count++;
            return new Node(key, value);
        }
        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { //key < node->key
            node->left = insert_aux(key, value, node->left);
        } else if (cmp_res == 1) { //key > node->key
            node->right = insert_aux(key, value, node->right);
        }
        //key == node->key
        return balance(node);
    }

    Node *erase_aux(const Key &key, Node *node) {
        if (!node) {
            return nullptr;
        }

        int cmp_res = comp(key, node->key);
        if (cmp_res == -1) { //key < node->key
            node->left = erase_aux(key, node->left);
        } else if (cmp_res == 1) { //key > node->key
            node->right = erase_aux(key, node->right);
        } else { //key == node->key
            Node *left = node->left;
            Node *right = node->right;

            delete node;
            items_count--;

            if (!right) {
                return left;
            }

            // В ДЗ ДЕЛАТЬ ОДНОЙ ФУНКЦИЕЙ find_and_remove_min_node
            Node *min_node = find_min(right);
            right = remove_min_node(right);

            min_node->left = left;
            min_node->right = right;

            return balance(min_node);
        }
        return balance(node);
    }

    Node *remove_min_node(Node *node) {
        if (!node->left) {
            return node->right;
        }

        node->left = remove_min_node(node->left);
        return balance(node);
    }

    Node *find_min(Node *node) {
        if (!node->left) {
            return node;
        }
        return find_min(node->left);
    }

    uint8_t height(Node *node) {
        if (!node) {
            return 0;
        }
        return node->height;
    }

    int64_t weight(Node *node) {
        if (!node) {
            return 0;
        }
        return node->weight;
    }

    void fix_height(Node *node) {
        node->height = std::max(height(node->left), height(node->right)) + 1;
    }

    void fix_weight(Node *node) {
        node->weight = 1 + weight(node->left) + weight(node->right);
    }

    int balance_factor(Node *node) {
        return height(node->right) - height(node->left);
    }

    Node *balance(Node *node) {
        fix_height(node);
        fix_weight(node);

        int bf = balance_factor(node);
        if (bf == 2) {
            if (balance_factor(node->right) < 0) {
                node->right = rotate_right(node->right);
            }
            return rotate_left(node);
        }

        if (bf == -2) {
            if (balance_factor(node->left) > 0) {
                node->left = rotate_left(node->left);
            }
            return rotate_right(node);
        }

        return node;
    }

    Node *rotate_left(Node *a) {
        Node *b = a->right;
        Node *c = b->left;
        a->right = c;
        b->left = a;
        return b;
    }

    Node *rotate_right(Node *a) {
        Node *b = a->left;
        Node *c = b->right;
        a->left = c;
        b->right = a;
        return b;
    }

    Node *root;
    size_t items_count;
    Comparator comp;
};

void run(std::istream &in, std::ostream &out) {
    AVLTree<int, int> tree;
    tree.insert(12, 12);
    tree.insert(8, 8);
    tree.insert(18, 18);
    tree.insert(5, 5);
    tree.insert(11, 11);
    tree.insert(17, 17);
    tree.insert(4, 4);
    for (int i = 0; i < 7; ++i) {
        out << tree.kth_element(i) << " ";
    }
}

int main() {
    run(std::cin, std::cout);
}