//#include "Huffman.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>

#define interface struct
typedef unsigned char byte;

interface IInputStream {
    // Возвращает false, если поток закончился
    virtual bool Read(byte &value) {
        if (current_pos >= buffer.size()) {
            return false;
        } else {
            value = buffer[current_pos];
            current_pos++;
            return true;
        }
    }

    explicit IInputStream(std::string path) {
        std::ifstream infile(path);
        std::vector<char> buffer1((std::istreambuf_iterator<char>(infile)),
                                  std::istreambuf_iterator<char>());
        buffer = buffer1;
        current_pos = 0;
    }

private:
    std::vector<char> buffer;
    size_t current_pos;
};

interface IOutputStream {
    virtual void Write(byte value) {
        buffer.push_back(value);
    }

    std::vector<char> buffer;
    size_t current_pos;
};


static void copyStream(IInputStream &input, IOutputStream &output) {
    byte value;
    while (input.Read(value)) {
        output.Write(value);
    }
}

//======================================
class BitWriter {
public:
    BitWriter(IOutputStream &stream) : out(stream), pos(0), buf(0), totalBits(0) {

    }

    // buf = 0000 0001
    // pos = 1
    void WriteBit(byte value) {
        value = value & 1;
        buf = buf | (value << pos);
        pos++;
        if (pos == 8) {
            flush();
        }
    }

    void WriteByte(byte value) {
        for (int i = 0; i < 8; ++i) {
            byte ith_bit = (value >> i) & 1;
            WriteBit(ith_bit);
        }
    }

    void flushRest(){
        if(pos != 0){
            flush();
        }
    }

    void flush() {
        out.Write(buf);
        buf = 0;
        pos = 0;
        totalBits += 8;
    }

private:
    IOutputStream &out;

    size_t totalBits;
    int pos;
    byte buf;

};

struct Node {
    byte value;
    int freq;
    Node *left;
    Node *right;

    Node(byte value, int freq) : value(value), freq(freq), left(nullptr), right(nullptr) {}
};

struct NodeComparator {
    bool operator()(Node *l, Node *r) {
        return l->freq > r->freq;
    }
};

void getVectorFromInput(IInputStream &original, std::vector<byte> &vector) {
    byte value;
    while (original.Read(value)) {
        vector.push_back(value);
    }
}

byte countAlphabet(std::vector<int> &alphabet, std::vector<byte> &vector) {
    for (auto i: vector) {
        alphabet[i]++;
    }
    byte alphabet_counter = 0;
    for (byte i = 0; i < 255; ++i) {
        if (alphabet[i] != 0) {
            alphabet_counter++;
        }
    }
    return alphabet_counter;
}

Node *construct_tree(std::vector<int> &alphabet) {
    std::priority_queue<Node *, std::vector<Node *>, NodeComparator> priorityQueue;
    for (byte i = 0; i < 255; ++i) {
        if (alphabet[i] != 0) {
            Node *current_node = new Node(i, alphabet[i]);
            priorityQueue.push(current_node);
        }
    }
    while (priorityQueue.size() >= 2) {
        Node *first = priorityQueue.top();
        priorityQueue.pop();
        Node *second = priorityQueue.top();
        priorityQueue.pop();
        Node *current_node = new Node(0, first->freq + second->freq);
        current_node->right = first;
        current_node->left = second;
        priorityQueue.push(current_node);
    }
    Node *root = priorityQueue.top();
    return root;
}

void encode_tree_dfs(BitWriter &bitWriter, Node *root, int *writtenBitsCounter) {
    if (root->left == nullptr && root->right == nullptr) {
        bitWriter.WriteBit(1);
        bitWriter.WriteByte(root->value);
        *writtenBitsCounter += 9;
    } else {
        encode_tree_dfs(bitWriter, root->left, writtenBitsCounter);
        encode_tree_dfs(bitWriter, root->right, writtenBitsCounter);
        bitWriter.WriteBit(0);
        *writtenBitsCounter += 1;
    }
}

void
fillTable(std::unordered_map<byte, std::vector<byte>> &encode_table, Node *root, int depth = 0, byte current_code = 0) {
    if (root->left == nullptr && root->right == nullptr) {
        for (int i = depth - 1; i >= 0; --i) {
            byte ith_bit = (current_code >> i) & 1;
            encode_table[root->value].push_back(ith_bit);
        }
        return;
    } else {
        fillTable(encode_table, root->left, depth + 1, current_code << 1);
        fillTable(encode_table, root->right, depth + 1, (current_code << 1) | 1);
    }
}

byte countFreeBitsInTheEnd(std::vector<int> &alphabet, std::unordered_map<byte, std::vector<byte>> &encode_table) {
    size_t counter_of_bits = 0;
    for (byte i = 0; i < 255; ++i) {
        if (alphabet[i] != 0) {
            counter_of_bits += (encode_table[i].size() * alphabet[i]);
        }
    }
    counter_of_bits %= 8;
    return byte(counter_of_bits);
}

void writeData(BitWriter &bitWriter, std::vector<byte> &input_vector,
               std::unordered_map<byte, std::vector<byte>> &encode_table) {
    for (auto currentByte : input_vector) {
        for(auto currentBit : encode_table[currentByte]){
            bitWriter.WriteBit(currentBit);
        }
    }
    bitWriter.flushRest();
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    std::vector<byte> input_vector;
    getVectorFromInput(original, input_vector);
    std::vector<int> alphabet(256, 0);
    byte alphabet_size = countAlphabet(alphabet, input_vector);
    Node *root = construct_tree(alphabet);
    std::unordered_map<byte, std::vector<byte>> encode_table; // key -- initial; value -- encoded
    fillTable(encode_table, root);

    BitWriter bitWriter(compressed);
    bitWriter.WriteByte(alphabet_size);
    int num_of_bits_encoded_tree = 0;
    encode_tree_dfs(bitWriter, root, &num_of_bits_encoded_tree);

    byte free_bits_end = countFreeBitsInTheEnd(alphabet, encode_table);
    free_bits_end += num_of_bits_encoded_tree;
    free_bits_end %= 8;
    bitWriter.WriteByte(free_bits_end);
    writeData(bitWriter, input_vector, encode_table);

}

void Decode(IInputStream &compressed, IOutputStream &original) {
    copyStream(compressed, original);
}

int main() {
    IInputStream input("/mnt/c/Users/artm/CLionProjects/Technopark-Algorithms/module 2/task5/test.txt");
    IOutputStream output;
    /*byte value;
    while(input.Read(value)){
        std::cout << value;
    }*/
    Encode(input, output);
    return 0;
}