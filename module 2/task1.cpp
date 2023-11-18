#include <iostream>
#include <string>

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
    struct Node {
        Node(const Key &key, const Value &value, int status) : key(key), value(value), status(status) {}

        Node() {
            status = 0;
        }

        int status; //0-empty 1-deleted 2--occupied
        Key key;
        Value value;
    };

public:
    Node *arr;
    size_t capacity;
    size_t count;
    Hasher hasher = Hasher();

    HashTable() {
        arr = new Node[8];
        capacity = 8;
        count = 0;
    }

    bool find(const Key &key) {
        for (size_t i = 0;;++i){
            size_t hash = (hasher(key) + i) % capacity;
            if (arr[hash].status == 0) {
                return false;
            } else if (arr[hash].status == 2 && arr[hash].key == key) {
                return true;
            }
        }
    }

    bool insert(const Key& key, const Value& value){
        if(find(key)){
            return false;
        }
        if(count / 3 * 4 >= capacity){
            rehash();
        }

        for (size_t i = 0;;++i){
            size_t hash = (hasher(key) + i) % capacity;
            if (arr[hash].status == 0 || arr[hash].status == 1) {
                arr[hash].status = 2;
                arr[hash].key = key;
                arr[hash].value = value;
                count++;
                return true;
            }
        }

    }

    bool erase(const Key& key){
        if(!find(key)){
            return false;
        }
        for (size_t i = 0;;++i){
            size_t hash = (hasher(key) + i ) % capacity;
            if (arr[hash].status == 2 && arr[hash].key == key) {
                arr[hash].status = 1;
                count--;
                return true;
            }
        }
    }

    void rehash(){
        Node *newarr = new Node[capacity*2];
        for(size_t i = 0; i < capacity; ++i){
            if(arr[i].status == 2){
                size_t hash = hasher(arr[i].key)%(capacity*2);
                newarr[hash].key = arr[i].key;
                newarr[hash].value = arr[i].value;
                newarr[hash].status = 2;
            }

        }
        delete[] arr;
        arr = newarr;
        capacity *= 2;
    }
};

void run(std::istream &in, std::ostream &out) {
    HashTable<std::string, int> table;

    for(int i = 0; i < 25; ++i){
        std::string a = "d" + std::to_string(i);
        table.insert(a, i);
    }

    HashTable<std::string, int> ht;
    char command = '\0';
    std::string value;
    while(std::cin >> command >> value) {
        bool result = false;
        switch(command) {
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
            std::cout << "OK" << std::endl;
        } else {
            std::cout << "FAIL" << std::endl;
        }
    }

}

int main() {
    run(std::cin, std::cout);
    return 0;
}