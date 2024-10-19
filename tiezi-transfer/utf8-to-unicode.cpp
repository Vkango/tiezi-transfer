#define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#include <locale>
#include <codecvt>
#include "app.h"

wstring utf8_to_w(string str) {
    std::wstring_convert<std::codecvt_utf8<wchar_t>> converter;
    std::wstring wide_string = converter.from_bytes(str);
    return wide_string;
}
