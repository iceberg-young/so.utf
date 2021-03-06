/**
 * @copyright 2015 Iceberg YOUNG
 * @license GNU Lesser General Public License version 3
 */

#include "utf.hpp"
#include "cast.hpp"

namespace so {
    namespace {
        char body(char32_t code) {
            return (code & 0b00111111) | 0b10000000;
        }
    }

    std::string& operator+=(std::string& utf8, char32_t code) {
        if (code <= 0x007F) {
            utf8 += char(code);
        }
        else if (code <= 0x07FF) {
            utf8 += char(((code >>  6) & 0b00011111) | 0b11000000);
            utf8 += body(code);
        }
        else if (code <= 0xFFFF) {
            utf8 += char(((code >> 12) & 0b00001111) | 0b11100000);
            utf8 += body(code >> 6);
            utf8 += body(code);
        }
        else if (code <= 0x10FFFF) {
            utf8 += char(((code >> 18) & 0b00000111) | 0b11110000);
            utf8 += body(code >> 12);
            utf8 += body(code >> 6);
            utf8 += body(code);
        }
        else {
            throw unicode_cast_error{
              std::to_string(code) + " is not a valid code point."
            };
        }
        return utf8;
    }

    std::string utf8(const std::u16string& utf16) {
        return utf32_indirect<std::string, std::u16string>(utf16);
    }

    std::string utf8(const std::u32string& utf32) {
        return utf32_direct<std::string>(utf32);
    }

    std::string utf8(char32_t utf32) {
        return utf32_single<std::string>(utf32);
    }
}
