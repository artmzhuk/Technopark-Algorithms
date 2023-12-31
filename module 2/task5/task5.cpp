//#include "Huffman.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <queue>
#include <stack>
#include <cstdint>

#define interface struct
typedef unsigned char byte;

interface IInputStream {
    virtual bool Read(byte &value) {
        if (current_pos >= buffer.size()) {
            return false;
        } else {
            value = buffer[current_pos];
            current_pos++;
            return true;
        }
    }

    explicit IInputStream(const std::string &path) {
        std::ifstream infile(path, std::ios::binary);
        std::vector<byte> buffer1((std::istreambuf_iterator<char>(infile)),
                                  std::istreambuf_iterator<char>());
        buffer = buffer1;
        current_pos = 0;
    }

    explicit IInputStream(std::vector<byte> &input_vector) {
        buffer = input_vector;
        current_pos = 0;
    }

    std::vector<byte> buffer;
private:
    size_t current_pos;
};

interface IOutputStream {
    virtual void Write(byte value) {
        buffer.push_back(value);
    }

    std::vector<byte> buffer;
};


//======================================
class BitWriter {
public:
    explicit BitWriter(IOutputStream &stream) : out(stream), pos(0), buf(0), totalBits(0) {}

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

    void flushRest() {
        if (pos != 0) {
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


class BitReader {
public:
    explicit BitReader(IInputStream &stream) : in(stream), pos(0), buf(0) {}

    bool ReadBit(byte &value) {
        if (pos == 0) {
            bool res = in.Read(buf);
            if (!res) {
                return false;
            }
        }
        value = (buf >> pos) & 1;
        pos++;
        if (pos == 8) {
            pos = 0;
        }
        return true;
    }

    bool ReadByte(byte &value) {
        byte result = 0;
        for (int i = 0; i < 8; i++) {
            byte bit = 0;
            if (!ReadBit(bit)) {
                return false;
            }
            result = result | (bit << i);
        }
        value = result;
        return true;
    }

private:
    IInputStream &in;
    int pos;
    byte buf;
};


struct Node {
    byte value;
    size_t freq;
    Node *left;
    Node *right;

    Node(byte value, size_t freq) : value(value), freq(freq), left(nullptr), right(nullptr) {}

    ~Node() {
        delete left;
        delete right;
    }
};

union singleLetter {
    std::uint64_t num_of_repeats;
    byte ith_byte[8];
};

void printTree(Node *root) {
    if (root->left == nullptr && root->right == nullptr) {
        std::cout << int(root->value) << " ";
        return;
    }
    printTree(root->left);
    printTree(root->right);
}

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

int countAlphabet(std::vector<int> &alphabet, std::vector<byte> &vector) {
    for (auto i: vector) {
        alphabet[i]++;
    }
    int alphabet_counter = 0;
    for (int i = 0; i < 256; ++i) {
        if (alphabet[i] != 0) {
            alphabet_counter++;
        }
    }
    return alphabet_counter;
}

Node *constructTree(std::vector<int> &alphabet) {
    std::priority_queue<Node *, std::vector<Node *>, NodeComparator> priorityQueue;
    for (int i = 0; i < 256; ++i) {
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

void encodeTreeDfs(BitWriter &bitWriter, Node *root, size_t *writtenBitsCounter) {
    if (root->left == nullptr && root->right == nullptr) {
        bitWriter.WriteBit(1);
        bitWriter.WriteByte(root->value);
        *writtenBitsCounter += 9;
    } else {
        encodeTreeDfs(bitWriter, root->left, writtenBitsCounter);
        encodeTreeDfs(bitWriter, root->right, writtenBitsCounter);
        bitWriter.WriteBit(0);
        *writtenBitsCounter += 1;
    }
}

void
fillTable(std::unordered_map<byte, std::vector<byte>> &encode_table, Node *root, int depth = 0, int current_code = 0) {
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
    for (int i = 0; i < 256; ++i) {
        if (alphabet[i] != 0) {
            counter_of_bits += (encode_table[i].size() * alphabet[i]);
        }
    }
    counter_of_bits %= 8;
    return byte(counter_of_bits);
}

void writeData(BitWriter &bitWriter, std::vector<byte> &input_vector,
               std::unordered_map<byte, std::vector<byte>> &encode_table) {
    for (unsigned char i: input_vector) {
        std::vector<byte> this_bits(encode_table[i]);
        for (unsigned char this_bit: this_bits) {
            bitWriter.WriteBit(this_bit);
        }
    }
    bitWriter.flushRest();
}

void encodeOneLetterData(BitWriter &bitWriter, byte value, size_t letter_counter) {
    bitWriter.WriteByte(0);
    bitWriter.WriteByte(value);
    singleLetter letter{};
    letter.num_of_repeats = letter_counter;
    int last_zero = 7;
    for (; last_zero >= 0; --last_zero) {
        if (letter.ith_byte[last_zero] != 0) {
            last_zero++;
            break;
        }
    }
    for (size_t i = 0; i < last_zero; ++i) {
        bitWriter.WriteByte(letter.ith_byte[i]);
    }
}

void fillOffset(BitWriter &bitWriter, int num_of_occupied_end_bits) {
    for (int i = 0; i < (8 - num_of_occupied_end_bits); ++i) {
        bitWriter.WriteBit(0);
    }
    bitWriter.WriteBit(1);
}

void Encode(IInputStream &original, IOutputStream &compressed) {
    std::vector<byte> input_vector;
    getVectorFromInput(original, input_vector);
    BitWriter bitWriter(compressed);
    if (input_vector.empty()) {
        return;
    }
    std::vector<int> alphabet(256, 0);
    int alphabet_size = countAlphabet(alphabet, input_vector);
    Node *root = constructTree(alphabet);
    std::unordered_map<byte, std::vector<byte>> encode_table; // key -- initial; value -- encoded
    if (root->left == nullptr && root->right == nullptr) {
        encode_table[root->value].push_back(1);
        encodeOneLetterData(bitWriter, root->value, alphabet[root->value]);
        return;
    } else {
        fillTable(encode_table, root);
    }
    bitWriter.WriteByte(alphabet_size - 1);
    size_t num_of_bits_encoded_tree = 0;
    encodeTreeDfs(bitWriter, root, &num_of_bits_encoded_tree);

    byte occupied_bits_end = countFreeBitsInTheEnd(alphabet, encode_table);
    num_of_bits_encoded_tree += occupied_bits_end;
    num_of_bits_encoded_tree += 1;
    occupied_bits_end = num_of_bits_encoded_tree % 8;
    fillOffset(bitWriter, occupied_bits_end);
    writeData(bitWriter, input_vector, encode_table);
    delete root;
}

Node *readAndConstructTree(BitReader &bitReader, const int alphabet_size) {
    std::stack<Node *> stack;
    int letters_already_read = 0;
    while (letters_already_read < alphabet_size || stack.size() > 1) {
        byte bit_read = 0;
        bitReader.ReadBit(bit_read);
        if (bit_read == 1) {
            byte letter = 0;
            bitReader.ReadByte(letter);
            letters_already_read++;
            Node *current_node = new Node(letter, -1);
            stack.push(current_node);
        } else {
            Node *first = stack.top();
            stack.pop();
            Node *second = stack.top();
            stack.pop();
            Node *current_node = new Node(0, -1);
            current_node->right = first;
            current_node->left = second;
            stack.push(current_node);
        }
    }
    Node *root = stack.top();
    return root;
}

std::vector<byte> make_bits_vector(BitReader &bitReader) {
    byte read_bit = 0;
    std::vector<byte> data;
    while (bitReader.ReadBit(read_bit)) {
        data.push_back(read_bit);
    }
    return data;
}

void readData(Node *root, std::vector<byte> &bits, size_t &i, std::vector<byte> &data) {
    if (root->left == nullptr && root->right == nullptr) {
        data.push_back(root->value);
    } else {
        if (bits[i] == 0) {
            i++;
            readData(root->left, bits, i, data);
        } else {
            i++;
            readData(root->right, bits, i, data);
        }
    }
}

void decodeSingleLetterData(BitReader &bitReader, IOutputStream &original) {
    byte value = 0;
    bitReader.ReadByte(value);
    singleLetter letter{};
    letter.num_of_repeats = 0;
    for (unsigned char &i: letter.ith_byte) {
        bool is_successful_read = bitReader.ReadByte(i);
        if (!is_successful_read) {
            break;
        }
    }
    for (size_t i = 0; i < letter.num_of_repeats; ++i) {
        original.Write(value);
    }
}

void skipOffset(BitReader &bitReader) {
    byte read = 0;
    while(read != 1){
        read = 0;
        bitReader.ReadBit(read);
    }
}

void Decode(IInputStream &compressed, IOutputStream &original) {
    int alphabet_size = 0;
    BitReader bitReader(compressed);
    if (!bitReader.ReadByte(reinterpret_cast<byte &>(alphabet_size))) {
        return;
    }
    alphabet_size += 1;
    if (alphabet_size == 1) {
        decodeSingleLetterData(bitReader, original);
        return;
    }
    Node *root = readAndConstructTree(bitReader, alphabet_size);
    skipOffset(bitReader);
    std::vector<byte> data_bits(make_bits_vector(bitReader));
    std::vector<byte> result;
    size_t current_bit_tree = 0;
    while (current_bit_tree != data_bits.size()) {
        readData(root, data_bits, current_bit_tree, result);
    }
    for (auto i: result) {
        original.Write(i);
    }
    delete root;
}


int main() {
    //IInputStream input("/mnt/c/Users/artm/CLionProjects/Technopark-Algorithms/module 2/task5/171548.jpg");
    IInputStream input("/mnt/c/Users/artm/CLionProjects/Technopark-Algorithms/module 2/task5/test.txt");
    //IInputStream input("/mnt/c/Users/artm/CLionProjects/Technopark-Algorithms/module 2/task5/artem.pdf");
    //IInputStream input("/mnt/c/Users/artm/CLionProjects/Technopark-Algorithms/module 2/task5/MSCEditor.exe");

    IOutputStream output;
    Encode(input, output);

    IInputStream input1(output.buffer);
    IOutputStream output1;
    Decode(input1, output1);
    for (size_t i = 0; i < input.buffer.size(); i++) {
        if (output1.buffer[i] != input.buffer[i]) {
            std::cout << i << "\n";
            return 1;
        }
    }
    size_t input_size = input.buffer.size();
    size_t compressed_size = output.buffer.size();
    std::cout << "Original size:" << input_size << "\n";
    std::cout << "Compressed size:" << compressed_size << "\n";
    std::cout << "Ratio:" << float(compressed_size) / float(input_size) << "\n";
    return 0;
}