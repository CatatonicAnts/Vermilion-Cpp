#pragma once

namespace verm
{
    bool is_digit_bin(char c);
    bool is_digit_oct(char c);
    bool is_digit_dec(char c);
    bool is_digit_hex(char c);
    bool can_start_identifier(char c);
    bool can_be_in_identifier(char c);
}
