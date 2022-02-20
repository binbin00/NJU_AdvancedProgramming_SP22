#include <iostream>
#include <string>
#include <string_view>
#include <iomanip>

#include "utfwidth.h"

// 直接定义，避免引入 Windows.h
#ifdef WIN32
extern "C" __declspec(dllimport) int __stdcall SetConsoleOutputCP(unsigned wCodePageID);
#endif

int main()
{
// 在 Windows 平台下
#ifdef WIN32
    SetConsoleOutputCP(65001);
#endif

    std::string str("测试abcｶｷｸｹｺ"); // 注意日语半角假名应该仅占 1 字符宽度
    std::string str2("测试abc再测试");

    // 注意 setw 被换成了 setw_u8
    std::cout << std::right << setw_u8(20, str) << str << std::endl;
    std::cout << std::right << setw_u8(20, str2) << str2 << std::endl;
}