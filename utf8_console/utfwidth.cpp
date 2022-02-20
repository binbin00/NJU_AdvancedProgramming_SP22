#include <exception>
#include <iostream>
#include <string_view>
#include <iomanip>
#include "utf8.h"

extern "C" int mk_wcwidth(wchar_t ucs);
extern "C" int mk_wcswidth(const wchar_t *pwcs, size_t n);

class wcswidth_iterator
{
private:
    size_t _result = 0;

public:
    size_t result() const { return _result; }
    void reset() { _result = 0; }

    wcswidth_iterator& operator=(wchar_t value)
    {
        auto len = mk_wcwidth(value);
        if (len < 0)
            throw std::runtime_error("Invalid UTF-8 value");
        _result += len;
        return *this;
    }

    wcswidth_iterator& operator*() { return *this; }
    wcswidth_iterator& operator++() { return *this; }
    wcswidth_iterator& operator++(int) { return *this; }
};

size_t utf8_cswidth(std::string_view str)
{
    if constexpr (sizeof(wchar_t) == 4)
    {
        return utf8::utf8to32(str.begin(), str.end(), wcswidth_iterator()).result();
    }
    else
    {
        return utf8::utf8to16(str.begin(), str.end(), wcswidth_iterator()).result();
    }
}

decltype(std::setw(0)) setw_u8(int w, std::string_view u8s)
{
    try
    {
        auto delta = utf8_cswidth(u8s) - u8s.length();
        return std::setw(w - delta);
    } catch (std::runtime_error&)
    {
        return std::setw(w);
    }
}