#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <stack>
#include <cctype>
#include <stdexcept>
#include"Big_Integer.hpp"

template<typename T>
class Calculator {
public:
    T evaluate(const std::string& expression);

private:
    std::string removeSpaces(const std::string& expr);
    T parseExpression(const std::string& expr);
    void processTop(std::stack<T>& values, std::stack<char>& ops);
    bool isOperator(char op);
    int precedence(char op);
    T applyOperation(T a, T b, char op);
};

template<typename T>
T Calculator<T>::evaluate(const std::string& expression) {
    std::string expr = removeSpaces(expression);
    return parseExpression(expr);
}

template<typename T>
std::string Calculator<T>::removeSpaces(const std::string& expr) {
    std::string result;
    for (char ch : expr) {
        if (!isspace(ch)) result += ch;
    }
    return result;
}

template<typename T>
T Calculator<T>::parseExpression(const std::string& expr) {
    std::stack<T> values;
    std::stack<char> ops;
    for (size_t i = 0; i < expr.length(); ++i) {
        if (isdigit(expr[i])) {
            T val = 0;
            while (i < expr.length() && isdigit(expr[i])) {
                Big_Integer ten = 10, num_now = expr[i] - '0';
                val = (val * ten) + num_now;
                ++i;
            }
            values.push(val);
            --i;
        } else if (expr[i] == '(') {
            ops.push(expr[i]);
        } else if (expr[i] == ')') {
            while (!ops.empty() && ops.top() != '(') {
                processTop(values, ops);
            }
            ops.pop(); // Remove '('
        } else if (isOperator(expr[i])) {
            while (!ops.empty() && precedence(ops.top()) >= precedence(expr[i])) {
                processTop(values, ops);
            }
            ops.push(expr[i]);
        } else {
            throw std::invalid_argument("Invalid character in expression");
        }
    }

    while (!ops.empty()) {
        processTop(values, ops);
    }

    return values.top();
}

template<typename T>
void Calculator<T>::processTop(std::stack<T>& values, std::stack<char>& ops) {
    T b = values.top(); values.pop();
    T a = values.top(); values.pop();
    char op = ops.top(); ops.pop();
    values.push(applyOperation(a, b, op));
}

template<typename T>
bool Calculator<T>::isOperator(char op) {
    return op == '+' || op == '-' || op == '*' || op == '/';
}

template<typename T>
int Calculator<T>::precedence(char op) {
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    return 0;
}

template<typename T>
T Calculator<T>::applyOperation(T a, T b, char op) {
    switch (op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/':
            if (b == 0) throw std::invalid_argument("Division by zero");
            return a / b;
        default: throw std::invalid_argument("Invalid operator");
    }
}


#endif // CALCULATOR_HPP
