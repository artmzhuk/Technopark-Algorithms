#include <iostream>
#include <string>
#include <cstdint>

size_t str_hash(const std::string &data) {
    size_t hash = 0;
    for (char i: data)
        hash = hash * 271 + i;
    return hash;
}

template<class Key>
struct Hash;

template<>
struct Hash<std::string> {
    size_t operator()(const std::string &value) const {
        return str_hash(value);
    }
};

template<>
struct Hash<int> {
    size_t operator()(int value) const {
        return value;
    }
};

template<typename Key, typename Value, typename Hasher=Hash<Key>>
class HashTable {
private:
    struct Node {
        Node(const Key &key, const Value &value) : key(key), value(value), status(2) {}

        Node() {
            status = Node::empty;
        }

        enum enum_status{empty, deleted, occupied};
        enum_status status;
        Key key;
        Value value;
    };

    Node *arr;
    size_t capacity;
    size_t count;
    Hasher hasher = Hasher();

    void rehash() {
        Node *old_arr = arr;
        size_t old_capacity = capacity;
        capacity *= 2;
        arr = new Node[capacity];
        for (size_t i = 0; i < old_capacity; ++i) {
            if (old_arr[i].status == Node::occupied) {
                size_t hash = hasher(old_arr[i].key);
                for (size_t j = 0;; ++j) {
                    hash = (hash + j*j) % capacity;
                    if (arr[hash].status == Node::empty) {
                        arr[hash].status = Node::occupied;
                        arr[hash].key = old_arr[i].key;
                        arr[hash].value = old_arr[i].value;
                        break;
                    }
                }
            }
        }
        delete[] old_arr;
    }

public:
    HashTable() {
        arr = new Node[8];
        capacity = 8;
        count = 0;
    }

    virtual ~HashTable() {
        delete[] arr;
    }

    HashTable(HashTable &table) {
        capacity = table.capacity;
        count = table.count;
        hasher = table.hasher;
        arr = new Node[capacity];
        for (size_t i = 0; i < capacity; ++i) {
            arr[i].status = table.arr[i].status;
            if (arr[i].status == Node::occupied) {
                arr[i].key = table.arr[i].key;
                arr[i].value = table.arr[i].value;
            }
        }
    }

    HashTable &operator=(const HashTable &table) {
        if (this != &table) {
            delete[] arr;
            capacity = table.capacity;
            count = table.count;
            hasher = table.hasher;
            arr = new Node[capacity];
            for (size_t i = 0; i < capacity; ++i) {
                arr[i].status = table.arr[i].status;
                if (arr[i].status == Node::occupied) {
                    arr[i].key = table.arr[i].key;
                    arr[i].value = table.arr[i].value;
                }
            }
        }
        return *this;
    }

    bool find(const Key &key) {
        size_t hash = hasher(key);
        for (size_t i = 0;; ++i) {
            hash = (hash + i*i) % capacity;
            if (arr[hash].status == Node::empty) {
                return false;
            } else if (arr[hash].status == Node::occupied && arr[hash].key == key) {
                return true;
            }
        }
    }

    bool insert(const Key &key, const Value &value) {
        if (find(key)) {
            return false;
        }
        if ((count / 3) * 4 >= capacity) {
            rehash();
        }
        size_t hash = hasher(key);
        for (size_t i = 0;; ++i) {
            hash = (hash + i*i) % capacity;
            if (arr[hash].status == Node::empty || arr[hash].status == Node::deleted) {
                arr[hash].status = Node::occupied;
                arr[hash].key = key;
                arr[hash].value = value;
                count++;
                return true;
            }
        }
    }

    bool erase(const Key &key) {
        if (!find(key)) {
            return false;
        }
        size_t hash = hasher(key);
        for (size_t i = 0;; ++i) {
            hash = (hash + i*i) % capacity;
            if (arr[hash].status == Node::occupied && arr[hash].key == key) {
                arr[hash].status = Node::deleted;
                return true;
            }
        }
    }
};

void run(std::istream &in, std::ostream &out) {
    HashTable<std::string, int> ht;
    char command = '\0';
    std::string value;
    while (in >> command >> value) {
        bool result = false;
        switch (command) {
            case '+':
                result = ht.insert(value, 0);
                break;
            case '-':
                result = ht.erase(value);
                break;
            case '?':
                result = ht.find(value);
                break;
            default:
                exit(1);
        }
        if (result) {
            out << "OK" << std::endl;
        } else {
            out << "FAIL" << std::endl;
        }
    }
}

int main() {
    run(std::cin, std::cout);
    return 0;
}