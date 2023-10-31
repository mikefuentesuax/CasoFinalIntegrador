#include <iostream>
#include <sstream>
#include <stack>
#include <vector>
#include <map>
#include <functional>
#include <string>
#include <cmath>
#include <algorithm>

typedef std::vector<std::string> Tokens;

using CustomFunction = std::function<double(std::vector<double>)>;

std::map<std::string, double> symbolTable;
std::map<std::string, CustomFunction> functionTable;

Tokens tokenize(const std::string& input) {
    Tokens tokens;
    std::string token;
    std::istringstream stream(input);

    while (stream >> token) {
        tokens.push_back(token);
    }

    return tokens;
}

double evaluate(Tokens& tokens) {
    std::stack<double> stack;

    for (const std::string& token : tokens) {
        if (isdigit(token[0]) || (token[0] == '-' && token.size() > 1)) {
            stack.push(std::stod(token));
        } else if (token == "+") {
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(a + b);
        } else if (token == "-") {
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(a - b);
        } else if (token == "*") {
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(a * b);
        } else if (token == "/") {
            double b = stack.top();
            stack.pop();
            double a = stack.top();
            stack.pop();
            stack.push(a / b);
        } else if (token == "sin") {
            if (stack.empty()) {
                std::cerr << "Error: La funcion 'sin' requiere al menos un argumento" << std::endl;
                exit(1);
            }
            double angle = stack.top();
            stack.pop();
            double result = sin(angle);
            stack.push(result);
        } else if (token == "cos") {
            if (stack.empty()) {
                std::cerr << "Error: La funcion 'cos' requiere al menos un argumento" << std::endl;
                exit(1);
            }
            double angle = stack.top();
            stack.pop();
            double result = cos(angle);
            stack.push(result);
        } else if (token == "log") {
            if (stack.empty()) {
                std::cerr << "Error: La funcion 'log' requiere al menos un argumento" << std::endl;
                exit(1);
            }
            double x = stack.top();
            stack.pop();
            double result = log(x);
            stack.push(result);
        } else if (token == "max") {
            if (stack.size() < 2) {
                std::cerr << "Error: La funcion 'max' requiere al menos dos argumentos" << std::endl;
                exit(1);
            }
            double result = stack.top();
            stack.pop();
            while (!stack.empty()) {
                result = std::max(result, stack.top());
                stack.pop();
            }
            stack.push(result);
        } else if (token == "min") {
            if (stack.size() < 2) {
                std::cerr << "Error: La funcion 'min' requiere al menos dos argumentos" << std::endl;
                exit(1);
            }
            double result = stack.top();
            stack.pop();
            while (!stack.empty()) {
                result = std::min(result, stack.top());
                stack.pop();
            }
            stack.push(result);
        } else if (isdigit(token[0]) || token[0] == '-') {
            stack.push(std::stod(token));
        } else if (symbolTable.count(token)) {
            stack.push(symbolTable[token]);
        } else if (functionTable.count(token)) {
            CustomFunction func = functionTable[token];
            std::vector<double> args;
            while (!stack.empty()) {
                args.push_back(stack.top());
                stack.pop();
            }
            std::reverse(args.begin(), args.end());
            double result = func(args);
            stack.push(result);
        } else {
            std::cerr << "Error: Token desconocido: " << token << std::endl;
            exit(1);
        }
    }

    if (stack.size() == 1) {
        return stack.top();
    } else {
        std::cerr << "Error: Expresion no válida" << std::endl;
        exit(1);
    }
}

void defineVariable(const std::string& name, double value) {
    symbolTable[name] = value;
}

void defineFunction(const std::string& name, CustomFunction func) {
    functionTable[name] = func;
}

int main() {
    while (true) {
        std::cout << "Ingrese una expresion (o 'exit' para salir): ";
        std::string input;
        std::getline(std::cin, input);

        if (input == "exit") {
            break;
        }

        if (input.empty()) {
            continue;
        }

        Tokens tokens = tokenize(input);

        if (tokens.size() >= 3 && tokens[1] == "=") {
            if (tokens.size() != 3 || !isdigit(tokens[2][0]) && tokens[2][0] != '-') {
                std::cerr << "Error: Formato de asignacion de variable incorrecto" << std::endl;
                continue;
            }
            defineVariable(tokens[0], std::stod(tokens[2]));
            std::cout << "Variable '" << tokens[0] << "' definida como " << std::stod(tokens[2]) << std::endl;
        } else {
            double result = evaluate(tokens);
            std::cout << "Resultado: " << result << std::endl;
        }
    }

    return 0;
}
