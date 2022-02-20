#pragma once

#include <string_view>
#include <iomanip>

decltype(std::setw(0)) setw_u8(int w, std::string_view u8s);