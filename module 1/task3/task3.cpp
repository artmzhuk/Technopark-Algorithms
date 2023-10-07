#include <iostream>
#include <cassert>

/*
 * 3_1. Реализовать очередь с динамическим зацикленным буфером (на основе динамического массива).
 * Требования: Очередь должна быть реализована в виде класса.
 */

template<typename T>
class Queue {
public:
    Queue(int cap) : cap(cap) {
        data = new T[cap];
        count = 0;
        head = 0;
        tail = 0;
    }

    Queue() {
        data = new T[8];
        cap = 4;
        count = 0;
        head = 0;
        tail = 0;
    }

    Queue(Queue &q) {
        data = new T[q.cap];
        for (int i = 0; i < q.cap; ++i) {
            data[i] = q.data[i];
        }
        cap = q.cap;
        count = q.count;
        head = q.head;
        tail = q.tail;
    }

    Queue &operator=(const Queue &q) {
        delete[] data;
        data = new T[q.cap];
        for (int i = 0; i < q.cap; ++i) {
            data[i] = q.data[i];
        }
        cap = q.cap;
        count = q.count;
        head = q.head;
        tail = q.tail;
        return *this;
    }

    ~Queue() {
        delete data;
    }

    bool IsEmpty() {
        return count == 0;
    }

    void Push(int x) {
        if (count == cap) {
            IncreaseBuffer();
        }
        data[tail] = x;
        tail++;
        if (tail == cap) {
            tail = 0;
        }
        count++;
    }

    int Pop() {
        if (IsEmpty()) {
            return -1;
        }
        int x = data[head];
        head++;
        if (head == cap) {
            head = 0;
        }
        count--;
        return x;
    }

    int Size() {
        return count;
    }

private:
    void IncreaseBuffer() {
        T *new_data = new T[cap * 2];
        for (int i = 0; i < cap - head; ++i) {
            new_data[i] = data[head + i];
        }
        for (int i = 0; i < tail; ++i) {
            new_data[cap - head + i] = data[i];
        }
        head = 0;
        tail = cap;
        cap *= 2;
        delete[] data;
        data = new_data;
    }

    int cap;
    T *data;
    int count;
    int head;
    int tail;
};

void TestQueue() {
    Queue<int> q;
    q.Push(1);
    q.Push(2);
    q.Push(3);
    q.Push(4);
    assert(q.Pop() == 1);
    assert(q.Pop() == 2);
    q.Push(5);
    q.Push(6);
    q.Push(7);
    Queue<int> a = q;
    assert(q.Size() == 5);
    q.Pop();
    q.Pop();
    q.Push(144);
    a = q;
}

void run(std::istream &in, std::ostream &out){
    Queue<int> q;
    int num_of_ops = 0;
    in >> num_of_ops;
    bool is_still_correct = true;
    for (int i = 0; i < num_of_ops && is_still_correct; ++i) {
        int a = 0;
        int b = 0;
        in >> a >> b;
        switch (a) {
            case 2:
                if (b != q.Pop()) {
                    is_still_correct = false;
                }
                break;
            case 3:
                q.Push(b);
                break;
            default:
                is_still_correct = false;
                break;
        }
    }
    if (is_still_correct) {
        out << "YES" << std::endl;
    } else {
        out << "NO" << std::endl;
    }
}

int main() {
    run(std::cin, std::cout);
    return 0;
}