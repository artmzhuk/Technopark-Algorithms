#include <iostream>
#include <vector>
#include <queue>
#include <cassert>

template<typename T>
struct DefaultComparator {
    bool operator()(const T &l, const T &r) {
        return l < r;
    }
};

template<typename Value, typename Comparator = DefaultComparator<Value>>
class BinaryTree {
public:
    struct Node {
        Node *left;
        Node *right;
        Node *parent;
        Value value;

        Node(Value value) {
            left = nullptr;
            right = nullptr;
            parent = nullptr;
            this->value = value;
        }
    };

    BinaryTree() {
        root = nullptr;
    }

    BinaryTree(Value value) {
        root = new Node(value);
    }

    virtual ~BinaryTree() {
        std::vector<Node *> nodes = this->traverseBFS();
        for (Node *node: nodes) {
            delete node;
        }
    }

    BinaryTree(const BinaryTree &old_tree) {
        if (old_tree.root == nullptr) {
            root = nullptr;
            compare = old_tree.compare;
            return;
        }
        std::queue<Node *> old_queue;
        std::queue<Node *> new_queue;

        root = new Node(old_tree.root->value);
        old_queue.push(old_tree.root);
        new_queue.push(root);
        while (!old_queue.empty()) {
            Node *current_old = old_queue.front();
            Node *current_new = new_queue.front();
            old_queue.pop();
            new_queue.pop();
            if (current_old->left != nullptr) {
                Node *new_left = new Node(current_old->left->value);
                new_left->parent = current_new;
                current_new->left = new_left;

                old_queue.push(current_old->left);
                new_queue.push(current_new->left);
            }
            if (current_old->right != nullptr) {
                Node *new_right = new Node(current_old->right->value);
                new_right->parent = current_new;
                current_new->right = new_right;

                old_queue.push(current_old->right);
                new_queue.push(current_new->right);
            }
        }
    }

    BinaryTree &operator=(const BinaryTree &old_tree) {
        std::vector<Node *> nodes = this->traverseBFS();
        for (Node *node: nodes) {
            delete node;
        }
        if (old_tree.root == nullptr) {
            root = nullptr;
            compare = old_tree.compare;
            return *this;
        }
        std::queue<Node *> old_queue;
        std::queue<Node *> new_queue;

        root = new Node(old_tree.root->value);
        old_queue.push(old_tree.root);
        new_queue.push(root);
        while (!old_queue.empty()) {
            Node *current_old = old_queue.front();
            Node *current_new = new_queue.front();
            old_queue.pop();
            new_queue.pop();
            if (current_old->left != nullptr) {
                Node *new_left = new Node(current_old->left->value);
                new_left->parent = current_new;
                current_new->left = new_left;

                old_queue.push(current_old->left);
                new_queue.push(current_new->left);
            }
            if (current_old->right != nullptr) {
                Node *new_right = new Node(current_old->right->value);
                new_right->parent = current_new;
                current_new->right = new_right;

                old_queue.push(current_old->right);
                new_queue.push(current_new->right);
            }
        }
        return *this;
    }

    void insert(Value value) {
        if (root == nullptr) {
            root = new Node(value);
            return;
        }
        Node *currentNode = root;
        Node *created = new Node(value);
        while (true) {
            if (compare(value, currentNode->value)) {
                if (currentNode->left != nullptr) {
                    currentNode = currentNode->left;
                } else {
                    created->parent = currentNode;
                    currentNode->left = created;
                    break;
                }
            } else {
                if (currentNode->right != nullptr) {
                    currentNode = currentNode->right;
                } else {
                    created->parent = currentNode;
                    currentNode->right = created;
                    break;
                }
            }
        }
    }

    std::vector<Node *> traverseBFS() {
        if (root == nullptr) {
            return std::vector<Node *>();
        }
        std::vector<Node *> vector;
        std::queue<Node *> queue;
        queue.push(root);
        while (!queue.empty()) {
            Node *current = queue.front();
            queue.pop();
            vector.push_back(current);
            if (current->left != nullptr) {
                queue.push(current->left);
            }
            if (current->right != nullptr) {
                queue.push(current->right);
            }
        }
        return vector;
    }

private:
    Node *root;
    Comparator compare;
};


void run(std::istream &in, std::ostream &out) {
    size_t num_of_nodes = 0;
    in >> num_of_nodes;
    BinaryTree<int64_t> tree;
    for (size_t i = 0; i < num_of_nodes; ++i) {
        int64_t value;
        in >> value;
        tree.insert(value);
    }
    std::vector<BinaryTree<int64_t>::Node *> result = tree.traverseBFS();
    for (BinaryTree<int64_t>::Node *i: result) {
        out << i->value << " ";
    }
}

void test(std::istream &in, std::ostream &out) {
    size_t num_of_nodes = 0;
    in >> num_of_nodes;
    BinaryTree<int64_t> tree;
    for (size_t i = 0; i < num_of_nodes; ++i) {
        int64_t value;
        in >> value;
        tree.insert(value);
    }
    std::vector<BinaryTree<int64_t>::Node *> result = tree.traverseBFS();
    for (BinaryTree<int64_t>::Node *i: result) {
        out << i->value << " ";
    }
    BinaryTree<int64_t> newTree(tree);
    std::vector<BinaryTree<int64_t>::Node *> result2 = newTree.traverseBFS();
    for (int i = 0; i < result.size(); ++i) {
        assert(result[i]->value == result2[i]->value);
    }
    BinaryTree<int64_t> newTree2(newTree);
    BinaryTree<int64_t> newTree3(5);
    newTree3 = newTree2;
    std::vector<BinaryTree<int64_t>::Node *> result3 = newTree2.traverseBFS();
    std::vector<BinaryTree<int64_t>::Node *> result4 = newTree3.traverseBFS();
    for (int i = 0; i < result3.size(); ++i) {
        assert(result3[i]->value == result4[i]->value);
    }


}

int main() {
    //test(std::cin, std::cout);
    run(std::cin, std::cout);
    return 0;
}