#include <iostream>
#include <array>
#include <vector>
#include <map>
#include <queue>
#include <unordered_map>
#include <stack>
#include <cstdint>

static const size_t LINE_SIZE = 4;
static const size_t FIELD_SIZE = LINE_SIZE * LINE_SIZE;

using FieldArray = std::array<uint8_t, FIELD_SIZE>;

const FieldArray EMPTY_STATE = {
        0, 0, 0, 0,//2+1 % 3 == 0
        0, 0, 0, 0,//5+1 % 3 == 0
        0, 0, 0, 0,//8
        0, 0, 0, 0//8

};

const FieldArray GOAL_STATE = {
        1, 2, 3,//0
        4, 5, 6,//3
        7, 8, 9, 10, 11, 12, 13, 14, 15, 0//6 % 3 == 0
};


class FieldState {
public:
    FieldState(const FieldArray &arr) : state(arr) {
        for (uint8_t i = 0; i < state.size(); ++i) {
            if (state[i] == 0) {
                zero_pos = i;
                break;
            }
        }
    }

    // FieldState(const FieldState& fs) : state(fs.state), zero_pos(fs.zero_pos) {
    // }
    FieldState(const FieldState &fs) = default;

    bool operator==(const FieldState &r) const {
        return state == r.state;
    }

    bool operator!=(const FieldState &r) const {
        return state != r.state;
    }

    bool operator<(const FieldState &r) const {
        return state < r.state;
    }

    std::vector<std::pair<FieldState, char>> GetNextVertices() const {
        std::vector<std::pair<FieldState, char>> result;
        result.reserve(4);

        if (zero_pos >= LINE_SIZE) { //up
            result.emplace_back(moveUp(), 'D');
        }
        if (zero_pos < (FIELD_SIZE - LINE_SIZE)) { //down
            result.emplace_back(moveDown(), 'U');
        }
        if ((zero_pos + 1) % LINE_SIZE != 0) { //right
            result.emplace_back(moveRight(), 'L');
        }
        if (zero_pos % LINE_SIZE != 0) { //left
            result.emplace_back(moveLeft(), 'R');
        }
        return result;
    }

    void print() const {
        for (int i = 0; i < state.size(); ++i) {
            std::cout << (int) state[i] << " ";
            if ((i + 1) % LINE_SIZE == 0) {
                std::cout << std::endl;
            }
        }
    }

    bool validate() const {
        int inversionCounter = 0;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            if (state[i] == 0) {
                continue;
            }

            int inversionCounterElem = 0;
            for (int j = 0; j < i; ++j) {
                if (state[j] > state[i]) {
                    inversionCounterElem++;
                }
            }
            inversionCounter += inversionCounterElem;
        }
        size_t zero_line = (zero_pos / LINE_SIZE) + 1;
        //std::cout << "zero line: " << zero_line << "\n";
        //std::cout << "inversions: " << inversionCounter << "\n";
        zero_line += inversionCounter;
        return zero_line % 2 == 0;
    }

    int CalcEvr(const FieldState &start) {
        int res = 0;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            int elem = state[i];
            if (elem == 0) {
                continue;
            }
            int x_target = (elem - 1) % LINE_SIZE;
            int x_current = i % LINE_SIZE;
            int y_target = (elem - 1) / LINE_SIZE;
            int y_current = i / LINE_SIZE;
            int hv = abs((x_current - x_target)) + abs(y_current - y_target);

            /*int startPos = -1;
            for (int j = 0; j < FIELD_SIZE; j++) {
                if (start.state[i] == elem) {
                    startPos = j;
                    break;
                }
            }

            int x_start = startPos % SIZE_WIDTH;
            int y_start = startPos / SIZE_WIDTH;

            int gv = abs(x_current - x_start) + abs(y_current - y_start);*/
            res += hv;
        }
        return res;
    }

    FieldArray state;

private:
    FieldState moveUp() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos - LINE_SIZE;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;
        return fs;
    }

    FieldState moveDown() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos + LINE_SIZE;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;

        return fs;
    }

    FieldState moveRight() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos + 1;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;

        return fs;
    }

    FieldState moveLeft() const {
        FieldState fs(*this);
        uint8_t new_zero_pos = fs.zero_pos - 1;
        std::swap(fs.state[fs.zero_pos], fs.state[new_zero_pos]);
        fs.zero_pos = new_zero_pos;

        return fs;
    }

    uint8_t zero_pos;

};

struct PairComp {
    constexpr bool operator()(
            std::pair<FieldState, int> const &a,
            std::pair<FieldState, int> const &b)
    const noexcept {
        return a.second > b.second;
    }
};

struct state_hash {
    std::size_t operator()(FieldState const &v) const {
        std::size_t res = 0;
        for (int i = 0; i < FIELD_SIZE; ++i) {
            size_t val = v.state[i];
            res |= i << (val * 4);
        }
        return res;
    }
};

int getDist(const FieldState &cur, const std::unordered_map<FieldState, int, state_hash> &dist) {
    if (dist.find(cur) == dist.end()) {
        return INT32_MAX;
    } else {
        return dist.find(cur)->second;
    }
}

void AStar(const FieldState &start) {
    const FieldState goal(GOAL_STATE);
    const FieldState empty(EMPTY_STATE);

    std::unordered_map<FieldState, std::pair<FieldState, char>, state_hash> parents;
    //std::map<FieldState, char> parent_transfer;
    std::unordered_map<FieldState, int, state_hash> dist;
    std::priority_queue<std::pair<FieldState, int>,
            std::vector<std::pair<FieldState, int>>, PairComp> q;
    parents.insert(std::make_pair(start, std::make_pair(empty, 'N')));
    //parent_transfer.insert(std::make_pair(start, 'N'));
    dist[start] = 0;
    q.emplace(start, 0);

    FieldState cur(EMPTY_STATE);
    while (!q.empty()) {
        cur = q.top().first;
        int cur_priority = q.top().second;
        q.pop();

        if (cur == goal) {
            break;
        }

        if (cur_priority >= getDist(cur, dist)) {
            std::vector<std::pair<FieldState, char>> children = cur.GetNextVertices();
            for (std::pair<FieldState, char> &st: children) {
                if (getDist(st.first, dist) == INT32_MAX) {
                    q.emplace(st.first, st.first.CalcEvr(start));
                }
                if (getDist(st.first, dist) > getDist(cur, dist) + 1) {
                    dist[st.first] = getDist(cur, dist) + 1;
                    parents.insert(std::make_pair(st.first, std::make_pair(cur, st.second)));
                    //parent_transfer.insert(std::make_pair(st.first, st.second));
                    q.emplace(st.first, st.first.CalcEvr(start) + dist[st.first]);
                }

/*            if (parents.find(st) == parents.end()) {
                parents.insert(std::make_pair(st, cur));
                q.push(st);
            }*/
            }
        }

    }
    int steps_count = 0;
    std::stack<char> stack;
    if (cur == goal) {
        while (cur != empty) {
            steps_count++;
            //cur.print();
            auto it = parents.find(cur);
            stack.push(it->second.second);
            cur = it->second.first;
            //std::cout << "-------------------" << std::endl;
        }
        //std::cout << "Solution found, steps_count=" << steps_count << std::endl;
        std::cout << steps_count - 1 << "\n";
        while (!stack.empty()) {
            if (stack.top() != 'N')
                std::cout << stack.top();
            stack.pop();
        }
    } else {
        std::cout << "-1\n";
        //std::cout << "Solution NOT found, vertices count=" << parents.size() << std::endl;
    }
}

int main() {
/*    FieldState start({
                             1, 2, 3, 4,
                             0, 5, 6, 8,
                             9, 10, 7, 11,
                             13, 14, 15, 12
                     });*/
    FieldArray arr;
    for (int i = 0; i < FIELD_SIZE; ++i) {
        int buf = 0;
        std::cin >> buf;
        arr[i] = buf;
    }
    FieldState start(arr);

    if (start.validate()) {
        AStar(start);
    } else {
        std::cout << "-1\n";
        return 0;
    }


    //bfs(start);

}