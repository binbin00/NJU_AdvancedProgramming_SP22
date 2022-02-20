# 关于带中文的 UTF-8 字符串的输出

中英文混合的 UTF-8 字符串在输出控制台，如果想利用 `std::setw` 等进行对齐，会出现错位的现象。这是因为 UTF-8 下中文通常占用 3 个字节，但中文在控制台下占据 2 个字符的位置。这种情况下系统直接把字节数作为宽度格式化就会出现错位的现象。

如果用 GB-18030(GBK) 编码，这种情况不会出现，这源于一个巧合：在 GBK 编码中中文通常占用 2 字符，恰好和控制台下中文占 2 字符的事实对应。然而，依赖这样的巧合是不健壮的，如果包含半角日语字符、西里尔字符、希腊文字符等，这样的巧合就无法成立。

因此，我们需要通用的解决方案，下面先给出使用说明，供不想看细节的同学使用。

## 使用

将 `demo.cpp` 外的所有文件/目录复制到工程中（如果您的项目有专门的文件夹存放第三方 Header-Only 库，可以把 `utf8` 目录和 `utf8.h` 移动进去）。将`mk_width.c`、`utf_width.cpp` 两个文件加入编译目标。

为了使用这些代码，你首先需要满足这些先决条件：

- 使用 C++17 编译
- 确保需要输出的字符串均是 UTF-8 类型，用窄字符类型表示（`char`，`std::string`，而非 `wchar_t`，`std::wstring`）
- 可以获取将要输出的字符串的 `std::string_view` 表示（对于 `std::string`，转换自动完成；对于 C 风格字符串，请自行在网络查询有关资料）

对于需要用到的地方，文件头需要 `#include "utfwidth.h"`，然后在所有使用 `std::setw` 的地方，用 `setw_u8` 代替，且 `setw_u8` 需要传入第二个参数，为欲输出的字符串。

**实际上，你应该参照 `demo.cpp` 以快速上手。**

注意：Windows 下，正确地处理 UTF-8 字符串需要：

- 源代码使用 UTF-8 编译：MSVC 需要打开 `/utf-8` 开关
- 源代码使用 UTF-8 编写：CLion、Visual Studio Code 等工具可以在右下角设置；Visual Studio 需要通过 `.editorconfig` 设置。
- 将控制台代码页改为 65001：可使用 `system("chcp 65001");`，更优雅的做法是用 `SetConsoleOutputCP` Windows API。参照 `demo.cpp` 的做法。这一操作仅需要在程序运行最开始做一次；

## Credits

这组代码用到了两组第三方代码：

- [剑桥大学的 Markus Kuhn](https://www.cl.cam.ac.uk/~mgk25/ucs/wcwidth.c) 编写的 `mk_width.c` 提供了跨平台的 `wcwidth` 函数
- [Nemanja Trifunovic 等编写的 UTF8-CPP 库](https://github.com/nemtrif/utfcpp)提供了方便的 UTF-8 到 UTF-16/32 的方法

## 延伸阅读

https://stackoverflow.com/questions/29188948/cout-setw-doesnt-align-correctly-with-%C3%A5%C3%A4%C3%B6

https://man7.org/linux/man-pages/man3/wcswidth.3.html

https://stackoverflow.com/questions/15114303/determine-whether-a-unicode-character-is-fullwidth-or-halfwidth-in-c

http://www.unicode.org/reports/tr11/