#include <iostream>
#include <array>
#include <cassert>
#include <unordered_map>
#include <queue>
#include <algorithm>
#include <cstring>
#include <valarray>

const char FieldSize = 16;
const std::array<char, FieldSize> finishField = {1, 2, 3, 4,
                                                 5, 6, 7, 8,
                                                 9, 10, 11, 12,
                                                 13, 14, 15, 0};

class GameState {
public:
    GameState(const std::array<char, FieldSize> &field)
            : field(field) {
        emptyPos = -1;
        evristic = -1;
        for (int i = 0; i < FieldSize; i++) {
            if (field[i] == 0) {
                emptyPos = i;
            }
        }
        assert(emptyPos != -1);
        CalcEvr();
    }

    bool IsComplete() const {
        return field == finishField;
    }

    bool IsSolvable() const {
        size_t zero_line = (emptyPos / 4) + 1;

        return (getInvCount() + zero_line) % 2 == 0;
    }

    bool CanMoveLeft() const {
        return emptyPos % 4 != 3;
    }

    bool CanMoveRight() const {
        return emptyPos % 4 != 0;
    }

    bool CanMoveUp() const {
        return emptyPos < 12;
    }

    bool CanMoveDown() const {
        return emptyPos > 3;
    }

    int CalcEvr() {

        int res = 0;
        for (int i = 0; i < 16; ++i) {
            int elem = field[i];
            int x_target = (elem - 1) % 4;
            int x_current = i % 4;
            int y_target = (elem - 1) / 4;
            int y_current = i / 4;
            int hv = sqrt(((x_current - x_target) * (x_current - x_target)) +
                          (y_current - y_target) * (y_current - y_target));

            res += hv;
        }
        evristic = res;
        return res;
    }

    GameState MoveLeft() {
        assert(CanMoveLeft());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 1]);
        newState.emptyPos++;
        CalcEvr();
        return newState;
    }

    GameState MoveRight() {
        assert(CanMoveRight());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 1]);
        newState.emptyPos--;
        CalcEvr();
        return newState;
    }

    GameState MoveUp() {
        assert(CanMoveUp());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos + 4]);
        newState.emptyPos += 4;
        CalcEvr();
        return newState;
    }

    GameState MoveDown() {
        assert(CanMoveDown());

        GameState newState(*this);
        std::swap(newState.field[emptyPos], newState.field[emptyPos - 4]);
        newState.emptyPos -= 4;
        CalcEvr();
        return newState;
    }

    bool operator==(const GameState &other) const {
        return field == other.field;
    }

    const int getEvr() const {
        return evristic;
    }

    int evristic;
private:

    size_t getInvCount() const {
        size_t inv_count = 0;
        for (int i = 0; i < FieldSize - 1; i++) {
            for (int j = i + 1; j < FieldSize; j++) {
                if (field[i] > field[j] && field[i] && field[j])
                    inv_count++;
            }
        }
        return inv_count;
    }

    std::array<char, FieldSize> field;
    char emptyPos;
    friend struct GameStateHasher;

    friend std::ostream &operator<<(std::ostream &out, const GameState &state);
};

struct GameStateHasher {
public:
    size_t operator()(const GameState &state) const {
        size_t hash = 0;
        std::memcpy(&hash, state.field.data(), sizeof(hash));
        return hash;
    }
};

std::ostream &operator<<(std::ostream &out, const GameState &state) {
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            out << static_cast<int>(state.field[i * 4 + j]) << ' ';
        }
        out << std::endl;
    }
    return out;
}

struct StateComp {
    constexpr bool operator()(
            GameState const &a,
            GameState const &b)
    const noexcept {
        return a.evristic > b.evristic;
    }
};

std::pair<int, std::string> GetSolution(const std::array<char, FieldSize> &field) {
    GameState startState(field);
    if (!startState.IsSolvable())
        return std::make_pair(-1, "");

    std::unordered_map<GameState, char, GameStateHasher> visited;
    visited[startState] = 'S';

    std::priority_queue<GameState, std::vector<GameState>, StateComp> queue;

    queue.push(startState);

    while (!queue.empty()) {
        GameState state = queue.top();
        queue.pop();

        if (state.IsComplete())
            break;

        if (state.CanMoveLeft()) {
            GameState newState = state.MoveLeft();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'L';
                queue.push(newState);
            }
        }
        if (state.CanMoveRight()) {
            GameState newState = state.MoveRight();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'R';
                queue.push(newState);
            }
        }
        if (state.CanMoveUp()) {
            GameState newState = state.MoveUp();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'U';
                queue.push(newState);
            }
        }
        if (state.CanMoveDown()) {
            GameState newState = state.MoveDown();
            if (visited.find(newState) == visited.end()) {
                visited[newState] = 'D';
                queue.push(newState);
            }
        }
    }

    std::string path;
    GameState state(finishField);
    int counter = 0;
    while (visited[state] != 'S') {
        char move = visited[state];
        counter++;
        switch (move) {
            case 'L': {
                state = state.MoveRight();
                path += 'L';
                break;
            }
            case 'R': {
                state = state.MoveLeft();
                path += 'R';
                break;
            }
            case 'D': {
                state = state.MoveUp();
                path += 'D';
                break;
            }
            case 'U': {
                state = state.MoveDown();
                path += 'U';
                break;
            }
        }
    }
    std::reverse(path.begin(), path.end());

    std::pair<int, std::string> pair;
    pair.first = counter;
    pair.second = path;
    return pair;
}

void run(std::istream &in, std::ostream &out) {
    std::array<char, FieldSize> field;
    for (int i = 0; i < 16; ++i) {
        int buf = 0;
        in >> buf;
        field[i] = buf;
    }
    std::pair<int, std::string> solution = GetSolution(field);
    out << solution.first << "\n" << solution.second;
}

int main() {
    run(std::cin, std::cout);
    return 0;
}