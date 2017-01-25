#include <iostream>
#include <cassert>
#include "include\ofn++\Variable.hpp"

ofn::Discretization ofn::Variable::discretization;

void ofn::Discretization::initialize(int ratio)
{
    yAxis.clear();
    this->ratio = ratio;
    for (int i = 0; i <= ratio; ++i) {
        yAxis.insert(yAxis.begin() + yAxis.size() / 2, (1.f / ratio) * i);
        yAxis.insert(yAxis.begin() + yAxis.size() / 2, (1.f / ratio) * i);
    }
    size = yAxis.size();
}

ofn::Variable::Variable() = default;

ofn::Variable::Variable(float a, float b, float c, float d)
{
    auto computeArm = [&](float ax, float ay, float bx, float by) {
        if (ax != bx) {
            auto a = (ay - by) / (ax - bx);
            auto b = ay - ax * a;
            for (int i = 0; i <= discretization.ratio; ++i) {
                m_values.push_back(((1.f / discretization.ratio) * i - b) / a);
            }
        }
        else for (int i = 0; i <= discretization.ratio; ++i) {
            m_values.push_back(ax);
        }
    };
    computeArm(a, 0.f, b, 1.f);
    computeArm(c, 0.f, d, 1.f);
}

void ofn::Variable::changeOrder()
{
    std::reverse(m_values.begin(), m_values.end());
}

// Operators.

float& ofn::Variable::operator[](int index)
{
    assert(index >= 0 && index < m_values.size());
    return m_values[index];
}

const float& ofn::Variable::operator[](int index) const
{
    assert(index >= 0 && index < m_values.size());
    return m_values[index];
}

ofn::Variable ofn::operator+(const Variable& a, const Variable& b)
{
    auto temp = ofn::Variable();
    for (int i = 0; i < Variable::discretization.size; ++i) {
        temp.m_values.push_back(a[i] + b[i]);
    }
    return temp;
}

ofn::Variable ofn::operator-(const Variable& a, const Variable& b)
{
    auto temp = ofn::Variable();
    for (int i = 0; i < Variable::discretization.size; ++i) {
        temp.m_values.push_back(a[i] - b[i]);
    }
    return temp;
}

ofn::Variable ofn::operator*(const Variable& a, const Variable& b)
{
    auto temp = ofn::Variable();
    for (int i = 0; i < Variable::discretization.size; ++i) {
        temp.m_values.push_back(a[i] * b[i]);
    }
    return temp;
}

ofn::Variable ofn::operator/(const Variable& a, const Variable& b)
{
    return a * (1.f / b);
}

ofn::Variable ofn::operator/(float scalar, const Variable& a)
{
    Variable temp;
    for (int i = 0; i < Variable::discretization.size; ++i) {
        temp.m_values.push_back(scalar / a[i]);
    }
    return temp;
}