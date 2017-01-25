#pragma once
#include "Utility.hpp"

namespace ofn
{
    struct Discretization
    {
        int ratio;
        std::vector<float> yAxis;
        int size;

        void initialize(int ratio = 10);
    };

    class Variable
    {
    public:
        Variable();
        Variable(float a, float b, float c, float d);
        void changeOrder();
  
        float& operator[](int index);
        const float& operator[](int index) const;

        static Discretization discretization;

    private:
        std::vector<float> m_values;

    // Friends.
        friend Variable operator+(const Variable& a, const Variable& b);
        friend Variable operator-(const Variable& a, const Variable& b);
        friend Variable operator*(const Variable& a, const Variable& b);
        friend Variable operator/(float scalar, const Variable& a);
    };

    Variable operator+(const Variable& a, const Variable& b);
    Variable operator-(const Variable& a, const Variable& b);
    Variable operator*(const Variable& a, const Variable& b);
    Variable operator/(const Variable& a, const Variable& b);
    Variable operator/(float scalar, const Variable& a);
}
