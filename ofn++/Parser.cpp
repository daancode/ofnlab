#include "include\ofn++\Parser.hpp"

void ofn::Parser::addVariable(const std::string& name, ofn::Variable ofn)
{
    this->ofn.insert({ name, ofn });
}

ofn::Variable ofn::Parser::parse(std::string expr)
{
    std::string value;
    std::vector<ofn::Variable> stack;

    if (expr.empty()) {
        throw std::string("You must define an expression.");
    }

    expr.erase(std::remove_if(expr.begin(), expr.end(), isspace), expr.end());
    expr = convert(expr);

    for (auto element : expr) {
        if (isalnum(element)) {
            value += element;
            continue;
        }
        else if (!value.empty()) {
            auto object = ofn.find(value);
            if (object != ofn.end()) {
                stack.push_back(ofn.find(value)->second);
            }
            else throw std::string(
                "Variable '" + value + "' is undefined."
            );
            value.clear();
        }
        if (isOperator(element)) {
            auto a = stack.back(); stack.pop_back();
            auto b = stack.back(); stack.pop_back();
            switch (element) {
                case '+': stack.push_back(b + a); break;
                case '-': stack.push_back(b - a); break;
                case '*': stack.push_back(b * a); break;
                case '/': stack.push_back(b / a); break;
            }
        }
    }
    return stack.back();
}

bool ofn::Parser::isOperator(char element) {
    return std::string("+-*/()").find(element) != std::string::npos;
}

int ofn::Parser::getPriority(char op)
{
    switch (op) {
        case '(': return 0;
        case '+': case '-': case ')': return 1;
        case '*': case '/': return 2;
        default: return -1;
    }
}

std::string ofn::Parser::convert(std::string expr)
{
    std::string result, stack;

    auto isLowerPriority = [&](char element) {
        return getPriority(element) <= getPriority(stack.back());
    };

    for (auto element : expr) {
        if (isalnum(element)) {
            result += element;
        }
        else if (element == '(') {
            stack.push_back(element);
        }
        else if (element == ')') {
            result += " ";
            while (stack.back() != '(') {
                result += stack.back();
                stack.pop_back();
            }
            stack.pop_back();
        }
        else if (isOperator(element)) {
            result += " ";
            while (!stack.empty() && isLowerPriority(element)) {
                result += stack.back();
                stack.pop_back();
                result += " ";
            }
            stack.push_back(element);
        }
    }
    result += " ";
    result.insert(result.end(), stack.rbegin(), stack.rend());
    return result;
}