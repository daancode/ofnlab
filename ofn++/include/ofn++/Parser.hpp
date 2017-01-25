#pragma once
#include <map>
#include "Variable.hpp"

namespace ofn
{
    class Parser
    {
    public:
        ofn::Variable parse(std::string expr);
        void addVariable(const std::string& name, ofn::Variable ofn);

    private:
        std::string convert(std::string expr);
        bool isOperator(char element);
        int getPriority(char op);

        std::map<std::string, ofn::Variable> ofn;
    };
}