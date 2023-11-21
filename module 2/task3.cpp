#include <iostream>
#include <cstdint>
#include <queue>


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
            delete left;
            delete right;
        }
    };

public:
    AVLTree(Comparator comp = Comparator()) :
            root(nullptr),
            items_count(0),
            comp(comp) {

    }

    ~AVLTree() {
        delete root;
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

    Value kth_element(size_t index) {
        if (!root) {
            return 0;
        }
        Node *result = kth_element_aux(root, index);
        if (result) {
            return result->value;
        } else {
            return 0;
        }
    }

    AVLTree(const AVLTree &old_tree) {
        if (old_tree.root == nullptr) {
            root = nullptr;
            comp = old_tree.comp;
            return;
        }
        std::queue<Node *> old_queue;
        std::queue<Node *> new_queue;

        root = new Node(old_tree.root->key, old_tree.root->value);
        old_queue.push(old_tree.root);
        new_queue.push(root);
        while (!old_queue.empty()) {
            Node *current_old = old_queue.front();
            Node *current_new = new_queue.front();
            current_new->weight = current_old->weight;
            current_new->height = current_old->height;

            old_queue.pop();
            new_queue.pop();
            if (current_old->left != nullptr) {
                Node *new_left = new Node(current_old->left->key, current_old->left->value);
                current_new->left = new_left;

                old_queue.push(current_old->left);
                new_queue.push(current_new->left);
            }
            if (current_old->right != nullptr) {
                Node *new_right = new Node(current_old->right->key, current_old->right->value);
                current_new->right = new_right;

                old_queue.push(current_old->right);
                new_queue.push(current_new->right);
            }
        }
        comp = old_tree.comp;
    }

    AVLTree &operator=(const AVLTree &old_tree) {
        delete this->root;
        if (old_tree.root == nullptr) {
            root = nullptr;
            comp = old_tree.comp;
            return *this;
        }
        std::queue<Node *> old_queue;
        std::queue<Node *> new_queue;

        root = new Node(old_tree.root->key, old_tree.root->value);
        old_queue.push(old_tree.root);
        new_queue.push(root);
        while (!old_queue.empty()) {
            Node *current_old = old_queue.front();
            Node *current_new = new_queue.front();
            current_new->weight = current_old->weight;
            current_new->height = current_old->height;

            old_queue.pop();
            new_queue.pop();
            if (current_old->left != nullptr) {
                Node *new_left = new Node(current_old->left->key, current_old->left->value);
                current_new->left = new_left;

                old_queue.push(current_old->left);
                new_queue.push(current_new->left);
            }
            if (current_old->right != nullptr) {
                Node *new_right = new Node(current_old->left->key, current_old->right->value);
                current_new->right = new_right;

                old_queue.push(current_old->right);
                new_queue.push(current_new->right);
            }
        }
        this->comp = old_tree.comp;
        return *this;
    }

private:
    Node *kth_element_aux(Node *node, int64_t index) {
        if (!node) {
            return nullptr;
        }
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

            node->left = nullptr;
            node->right = nullptr;
            delete node;
            items_count--;

            if (!right) {
                return left;
            }

            Node *min_node = nullptr;
            Node **min_node_ptr = &min_node;
            right = find_and_remove_min_node(right, min_node_ptr);

            min_node->left = left;
            min_node->right = right;

            return balance(min_node);
        }
        return balance(node);
    }

    Node *find_and_remove_min_node(Node *node, Node **min_node_ptr) {
        if (!node->left) {
            *min_node_ptr = node;
            return node->right;
        }
        node->left = find_and_remove_min_node(node->left, min_node_ptr);
        return balance(node);
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

    void fix_height_and_weight(Node *a, Node *b) {
        fix_height(a);
        fix_height(b);
        fix_weight(a);
        fix_weight(b);
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

    Node *rotate_left(Node *a) { //nodes are marked as per lectures
        Node *b = a->right;
        Node *c = b->left;
        a->right = c;
        b->left = a;
        fix_height_and_weight(a, b);
        return b;
    }

    Node *rotate_right(Node *a) { //nodes are marked as per lectures
        Node *b = a->left;
        Node *c = b->right;
        a->left = c;
        b->right = a;
        fix_height_and_weight(a, b);
        return b;
    }

    Node *root;
    size_t items_count;
    Comparator comp;
};

void run(std::istream &in, std::ostream &out) {
    AVLTree<int64_t, int64_t> tree;
    size_t N;
    in >> N;
    for (size_t i = 0; i < N; ++i) {
        int64_t number = 0;
        size_t kth_index = 0;
        in >> number >> kth_index;
        if (number > 0) {
            tree.insert(number, number);
        } else {
            tree.erase(std::llabs(number));
        }
        out << tree.kth_element(kth_index) << " ";
    }
}

void test(std::istream &in, std::ostream &out) {
    AVLTree<int, int> tree;
//    tree.insert(12, 12);
//    tree.insert(8, 8);
//    tree.insert(18, 18);
//    tree.insert(5, 5);
//    tree.insert(11, 11);
//    tree.insert(17, 17);
//    tree.insert(4, 4);
//    for (int i = 0; i < tree.size(); ++i) {
//        out << tree.kth_element(i) << " ";
//    }
    tree.insert(10, 1);
    out << tree.kth_element(0) << " ";
    tree.insert(20, 2);
    out << tree.kth_element(0) << " ";
    tree.insert(30, 3);
    out << tree.kth_element(3);
    AVLTree<int, int> tree1(tree);
    AVLTree<int, int> tree2;
    tree2.insert(5, 7);
    tree2 = tree1;
}

int main() {
    test(std::cin, std::cout);
    //run(std::cin, std::cout);
}