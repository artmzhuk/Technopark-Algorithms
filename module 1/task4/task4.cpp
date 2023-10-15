/*В операционной системе Technux есть планировщик процессов.
 *Каждый процесс характеризуется:
 *приоритетом P
 *временем, которое он уже отработал t
 *временем, которое необходимо для завершения работы процесса T
 *Планировщик процессов выбирает процесс с минимальным значением P * (t + 1), выполняет его время P и кладет обратно в
 * очередь процессов.
 *Если выполняется условие t >= T, то процесс считается завершенным и удаляется из очереди.
 *Требуется посчитать кол-во переключений процессора.
 */

#include <iostream>

template<typename T>
struct DefaultComparator {
    bool operator()(const T &l, const T &r) {
        return l < r;
    }
};

template<typename T, typename Comparator = DefaultComparator<T>>
class Heap {
public:
    Heap(Comparator comp_arg = DefaultComparator<T>()) {
        buf = new T[10];
        capacity = 10;
        size = 0;
        compare = comp_arg;
    }

    Heap(T *array, size_t size_arg) {
        size = size_arg;
        capacity = size;
        buf = new T[size];
        for (size_t i = 0; i < size; ++i) {
            buf[i] = array[i];
        }
        Comparator cmp;
        compare = cmp;
        for (long long i = size_arg / 2 - 1; i >= 0; --i) {
            sift_down(i);
        }
    }

    Heap(Heap &h) {
        size = h.size;
        capacity = h.capacity;
        compare = h.get_size();
        buf = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            buf[i] = h.buf[i];
        }
    }

    Heap &operator=(const Heap &h) {
        delete[] buf;
        size = h.size;
        capacity = h.capacity;
        compare = h.compare;
        buf = new T[capacity];
        for (size_t i = 0; i < size; ++i) {
            buf[i] = h.buf[i];
        }
        return *this;
    }

    virtual ~Heap() {
        delete[] buf;
    }

    const T &top() const {
        return buf[0];
    }

    void push(const T &value) {
        if (size == capacity) {
            grow();
        }
        buf[size] = value;
        size++;
        sift_up(size - 1);
    }

    T pop() {
        if (size == 0) {
            exit(1);
        }
        T result = buf[0];
        buf[0] = buf[size - 1];
        size--;
        if (size != 0) {
            sift_down(0);
        }
        return result;
    }

    size_t get_size() {
        return size;
    }

    bool is_empty() {
        return size == 0;
    }

private:
    void sift_down(size_t idx) {
        while (2 * idx + 1 < get_size()) {
            size_t left = 2 * idx + 1;
            size_t right = 2 * idx + 2;
            size_t largest = left;
            if (right < get_size() && compare(buf[left], buf[right])) {
                largest = right;
            }
            if (compare(buf[largest], buf[idx])) {
                break;
            }
            std::swap(buf[largest], buf[idx]);
            idx = largest;
        }
    }


    void sift_up(size_t idx) {
        while (idx > 0) {
            size_t parent = (idx - 1) / 2;
            if (compare(buf[parent], buf[idx])) {
                std::swap(buf[idx], buf[parent]);
                idx = parent;
            } else {
                return;
            }
        }
    }

    void grow() {
        T *new_buf = new T[capacity * 2];
        for (size_t i = 0; i < capacity; ++i) {
            new_buf[i] = buf[i];
        }
        capacity *= 2;
        delete[] buf;
        buf = new_buf;
    }

    Comparator compare;

    T *buf;
    size_t capacity;
    size_t size;
};

struct Process {
    int priority_in_os;
    int priority_in_heap;
    int current_time;
    int target_time;
};

struct ProcessComparator {
    bool operator()(const Process &l, const Process &r) {
        return l.priority_in_heap > r.priority_in_heap;
    }
};

void run(std::istream &in, std::ostream &out) {
    size_t num_of_processes = 0;
    in >> num_of_processes;
    Process *processes = new Process[num_of_processes];
    for (size_t i = 0; i < num_of_processes; ++i) {
        in >> processes[i].priority_in_os >> processes[i].target_time;
        processes[i].current_time = 0;
        processes[i].priority_in_heap = processes[i].priority_in_os;
    }
    Heap<Process, ProcessComparator> heap(processes, num_of_processes);
    int counter = 0;
    while (!heap.is_empty()) {
        Process top = heap.pop();
        top.current_time += top.priority_in_os;
        top.priority_in_heap = top.priority_in_os * (top.current_time + 1);
        if (top.current_time < top.target_time) {
            heap.push(top);
        }
        counter++;
    }
    out << counter;
    delete[] processes;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}