#include <iostream>
#include <vector>
#include <queue>
#include <cassert>
#include <unordered_set>

struct Node {
    std::string product_name;
    int color;
    int size;
};

bool operator == (Node const& lhs, Node const& rhs)
{
    return (lhs.product_name == rhs.product_name) && (lhs.color == rhs.color) && (lhs.size == rhs.size);
}

struct Hash {
    size_t operator()(const Node &node) const {
        size_t hash = 0;
        for (char i: node.product_name)
            hash = hash * 271 + i;
        return hash;
    }
};

void run(std::istream &in, std::ostream &out) {
    std::unordered_set<Node, Hash> set;
    std::string operation;
    while(in >> operation){
        Node current;
        in >> current.product_name;
        in >> current.color;
        in >> current.size;
        if(operation == "+"){
            if(set.find(current) != set.end()){
                out<<"FAIL\n";
            } else {
                out << "OK\n";
                set.insert(current);
            }
        } else if(operation == "-"){
            if(set.find(current) != set.end()){
                out<<"OK\n";
                set.erase(current);
            } else {
                out << "FAIL\n";
            }
        } else if(operation == "?"){
            if(set.find(current) != set.end()){
                out<<"OK\n";
            } else {
                out << "FAIL\n";
            }
        }
    }
}

int main() {
    run(std::cin, std::cout);
    return 0;
}