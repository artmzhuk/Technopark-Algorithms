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
        Value value;

        Node(Value value) {
            left = nullptr;
            right = nullptr;
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
                current_new->left = new_left;

                old_queue.push(current_old->left);
                new_queue.push(current_new->left);
            }
            if (current_old->right != nullptr) {
                Node *new_right = new Node(current_old->right->value);
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
                current_new->left = new_left;

                old_queue.push(current_old->left);
                new_queue.push(current_new->left);
            }
            if (current_old->right != nullptr) {
                Node *new_right = new Node(current_old->right->value);
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
                    currentNode->left = created;
                    break;
                }
            } else {
                if (currentNode->right != nullptr) {
                    currentNode = currentNode->right;
                } else {
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

    bool traverseBFS_checkEqual() {
        if (root == nullptr) {
            return false;
        }
        Value rootValue = root->value;
        std::queue<Node *> queue;
        queue.push(root);
        while (!queue.empty()) {
            Node *current = queue.front();
            queue.pop();
            if(current->value != rootValue){
                return false;
            }
            if (current->left != nullptr) {
                queue.push(current->left);
            }
            if (current->right != nullptr) {
                queue.push(current->right);
            }
        }
        return true;
    }

private:
    Node *root;
    Comparator compare;
};


void run(std::istream &in, std::ostream &out) {
    BinaryTree<int64_t> tree;
    int64_t value;
    while(in){
        in >> value;
        tree.insert(value);
    }

    bool result = tree.traverseBFS_checkEqual();
    if(result){
        out << "1";
    } else {
        out << "0";
    }
}

int main() {

    run(std::cin, std::cout);
    return 0;
}