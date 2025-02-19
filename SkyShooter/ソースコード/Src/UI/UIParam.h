#pragma once

template<class T> 
struct UIParam
{
    UIParam(void)
        :
        val(T()),
        valMax(T())
    {};

    UIParam(const T& v, const T& vMax)
        :
        val(v),
        valMax(vMax)
    {};

    T val;
    T valMax;
};
