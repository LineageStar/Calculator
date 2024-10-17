#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include <iostream>
#include <string>
#include <stack>
#include <stdexcept>
#include "Big_Integer.hpp"

template <typename T>
class Calculator {
public:
    T evaluate(const std::string& expression) {
        std::string expr = removeSpaces(expression);
        return parseExpression(expr);
    }

private:
    std::string removeSpaces(const std::string& expr) {
        std::string result;
        for (char ch : expr) {
            if (!isspace(ch)) result += ch;
        }
        return result;
    }

    T parseExpression(const std::string& expr) {
        std::stack<T> values;
        std::stack<char> ops;
        for (size_t i = 0; i < expr.length(); ++i) {
            if (isdigit(expr[i])) {
                T val = parseNumber(expr, i);
                values.push(val);
            } else if (expr[i] == '(') {
                ops.push(expr[i]);
            } else if (expr[i] == ')') {
                while (!ops.empty() && ops.top() != '(') {
                    processTop(values, ops);
                }
                ops.pop();
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

    T parseNumber(const std::string& expr, size_t& i) {
        std::string num_str;
        while (i < expr.length() && (isdigit(expr[i]) || expr[i] == '0')) {
            num_str += expr[i++];
        }
        --i; // Adjust for the increment in the loop
        return T(num_str);
    }

    void processTop(std::stack<T>& values, std::stack<char>& ops) {
        T b = values.top(); values.pop();
        T a = values.top(); values.pop();
        char op = ops.top(); ops.pop();
        values.push(applyOperation(a, b, op));
    }

    bool isOperator(char op) {
        return op == '+' || op == '-' || op == '*' || op == '/';
    }

    int precedence(char op) {
        if (op == '+' || op == '-') return 1;
        if (op == '*' || op == '/') return 2;
        return 0;
    }

    T applyOperation(const T& a, const T& b, char op) {
        switch (op) {
            case '+': return a + b;
            case '-': return a - b;
            case '*': return a * b;
            case '/':
                if (b == T("0")) throw std::invalid_argument("Division by zero");
                return a / b;
            default: throw std::invalid_argument("Invalid operator");
        }
    }
};

int main() {
    Calculator<Big_Integer> calc;
    try {
        std::string expression = "12345678901234567890 + (23456789012345678901 * 2)";
        std::cout << "Result: " << calc.evaluate(expression) << std::endl;
    } catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }
    return 0;
}


#endif // _CALCULATOR_H_