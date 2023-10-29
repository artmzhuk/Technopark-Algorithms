#include <iostream>

template<typename T>
class Stack{
public:
    Stack(){
        data = new T[800000];
        top = 0;
        cap = 800000;
    }

    virtual ~Stack() {
        delete data;
    }

    void push(T element){
        if (top == cap){
            exit(1);
        }
        data[top] = element;
        top++;
    }
    T pop(){
        if(top == 0){
            return 0;
        }
        top--;
        return data[top];
    }

    int size(){
        return top;
    }

private:
    T *data;
    int top;
    int cap;
};

int main() {
    Stack<char> stack;
    char element;
    while(std::cin >> element){
        char popped = 1;
        if(element == '('){
            stack.push(element);
        } else {
            popped = stack.pop();
        }
        if (popped == 0){
            std::cout << "NO"; //case when trying to pop from empty stack
            return 0;
        }
    }
    if(stack.size() == 0){
        std::cout << "YES";
    } else {
        std::cout << "NO";
    }
    return 0;
}