#include "common.hpp"
#include <cassert>

using namespace verm;

char const * verm::token_type_to_string(token_types tt)
{
    switch (tt)
    {
        case tk_end_of_file:
            return "end-of-file";
        case tk_identifier:
            return "identifier";
        case tk_integer:
            return "integer";
        case tk_float:
            return "float";
        case tk_string:
            return "string";
        case tk_char:
            return "char";
        case tk_comment:
            return "comment";

        case tk_opr_asgn:
            return "assignment";
        case tk_comma:
            return "comma";
        case tk_member:
            return "member-access";
        case tk_namespace:
            return "namespace-access";
        case tk_end_statement:
            return "end-of-statement";

        case tk_opr_member_maybe:
            return "member-maybe-access";
        case tk_opr_null_coalesce:
            return "null-coalescion";

        case tk_brk_rnd_open:
            return "open-round-bracket";
        case tk_brk_rnd_close:
            return "close-round-bracket";
        case tk_brk_sq_open:
            return "open-square-bracket";
        case tk_brk_sq_close:
            return "close-square-bracket";
        case tk_brk_crl_open:
            return "open-curly-bracket";
        case tk_brk_crl_close:
            return "close-curly-bracket";

        case tk_brk_template_open:
            return "open-template-bracket";

        case tk_opr_add:
            return "add";
        case tk_opr_sub:
            return "subtract";
        case tk_opr_mul:
            return "multiply";
        case tk_opr_div:
            return "divide";
        case tk_opr_mod:
            return "modulo";
        case tk_opr_xor:
            return "xor";

        case tk_opr_bor:
            return "bitwise-or";
        case tk_opr_band:
            return "bitwise-and";
        case tk_opr_bcomp:
            return "bitwise-complement";
        case tk_opr_lor:
            return "logical-or";
        case tk_opr_land:
            return "logical-and";
        case tk_opr_lnot:
            return "logical-negation";

        case tk_opr_bnor:
            return "bitwise-nor";
        case tk_opr_bnand:
            return "bitwise-nand";
        case tk_opr_lnor:
            return "logical-nor";
        case tk_opr_lnand:
            return "logical-nand";

        case tk_opr_shl:
            return "left-shift";
        case tk_opr_shr:
            return "right-shift";
        case tk_opr_rol:
            return "left-rotate";
        case tk_opr_ror:
            return "right-rotate";
        
        case tk_opr_pow:
            return "power";
        case tk_opr_inc:
            return "increment";
        case tk_opr_dec:
            return "decrement";

        case tk_opr_asgn_add:
            return "add-assign";
        case tk_opr_asgn_sub:
            return "subtract-assign";
        case tk_opr_asgn_mul:
            return "multiply-assign";
        case tk_opr_asgn_div:
            return "divide-assign";

        case tk_opr_asgn_mod:
            return "modulo-assign";
        case tk_opr_asgn_xor:
            return "xor-assign";
        case tk_opr_asgn_bor:
            return "bitwise-or-assign";
        case tk_opr_asgn_band:
            return "bitwise-and-assign";

        case tk_opr_asgn_lor:
            return "logical-or-assign";
        case tk_opr_asgn_land:
            return "logical-and-assign";
        case tk_opr_asgn_bnor:
            return "bitwise-nor-assign";
        case tk_opr_asgn_bnand:
            return "bitwise-nand-assign";

        case tk_opr_asgn_lnor:
            return "logical-nor-assign";
        case tk_opr_asgn_lnand:
            return "logical-nand-assign";

        case tk_opr_asgn_shl:
            return "left-shift-assign";
        case tk_opr_asgn_shr:
            return "right-shift-assign";
        case tk_opr_asgn_rol:
            return "left-rotate-assign";
        case tk_opr_asgn_ror:
            return "right-rotate-assign";

        case tk_opr_asgn_pow:
            return "power-assign";

        case tk_opr_lt:
            return "less-than";
        case tk_opr_gt:
            return "greater-than";
        case tk_opr_lte:
            return "less-than-or-equal";
        case tk_opr_gte:
            return "greater-than-or-equal";
        case tk_opr_eq:
            return "equal";
        case tk_opr_neq:
            return "not-equal";
        
        case tk_opr_ref:
            return "reference";
        case tk_opr_deref:
            return "dereference";

        case tk_opr_tern_if:
            return "ternary-if";
        case tk_opr_tern_else:
            return "ternary-else";
        
        case tk_opr_usr_1:
            return "user-operator-1";
        case tk_opr_usr_2:
            return "user-operator-2";
        case tk_opr_asgn_usr_1:
            return "user-operator-1-assign";
        case tk_opr_asgn_usr_2:
            return "user-operator-2-assign";

        default:
            return "UNKNOWN";
    }
}

std::ostream & verm::operator <<(std::ostream & st, token const & tk)
{
    st << "[" << token_type_to_string(tk.get_type());

    switch (tk.get_type())
    {
    case tk_identifier:
    case tk_integer:
    case tk_float:
    case tk_string:
    case tk_char:
    case tk_comment:
        st << " " << tk.get_value();
        //  Fall-through. -ish
    default:
        break;
    }

    if (tk.get_type() == tk_integer)
        st << "; " << tk.get_int_data()->number << "; " << tk.get_int_data()->suffix;
    else if (tk.get_type() == tk_float)
        st << "; " << tk.get_float_data()->number << "; " << tk.get_float_data()->suffix;

    return st << "]";
}

/**********************
    tokenizer class
**********************/

tokenizer::tokenizer(std::string fn, std::string conts)
    : first_token( nullptr), last_token(nullptr)
    , code(new code_unit(fn, conts))
    , last_line(nullptr)
    , contStart(this->code->contents.begin()), contEnd(this->code->contents.end())
    , iter(this->contStart)
    , error(), error_position()
{

}

bool tokenizer::produce_token()
{
    if (last_line == nullptr)
    {
        last_line = new line_of_code(this->code, 0, 1);
    }

retry_char:
    if (iter == contEnd)
    {
        if (last_token == nullptr || last_token->type != tk_end_of_file)
        {
            append_token(tk_end_of_file, 0);

            last_line->end = contEnd - contStart;
        }

        return true;
    }

    switch (char c = *iter++)
    {
    case '\n':
        last_line = new line_of_code(last_line, iter - contStart);
        goto retry_char;

    case ' ':
    case '\t':
    case '\v':
    case '\f':
    case '\r':
        //  Nothing to do with spaces here.
        goto retry_char;

    case '(':
        append_token(tk_brk_rnd_open, 1);
        return true;
    case ')':
        append_token(tk_brk_rnd_close, 1);
        return true;
    case '[':
        append_token(tk_brk_sq_open, 1);
        return true;
    case ']':
        append_token(tk_brk_sq_close, 1);
        return true;
    case '{':
        append_token(tk_brk_crl_open, 1);
        return true;
    case '}':
        append_token(tk_brk_crl_close, 1);
        return true;

    case ',':
        append_token(tk_comma, 1);
        return true;
    case '.':
        append_token(tk_member, 1);
        return true;
    case ';':
        append_token(tk_end_statement, 1);
        return true;

    case '@':
        append_token(tk_opr_ref, 1);
        return true;
    case '#':
        append_token(tk_opr_deref, 1);
        return true;

    case '=':
        //  Can be either assignment or equality check.

        if (iter != contEnd && *iter == '=')
        {
            ++iter;
            //  Advance to the next character.

            append_token(tk_opr_eq, 2);
            //  It's definitely equality check.

            return true;
        }

        append_token(tk_opr_asgn, 1);
        return true;

    case ':':
        //  Can be either ternary else, namespace access, or opening template brackets.

        if (iter != contEnd && *iter == ':')
        {
            ++iter;

            if (iter != contEnd && *iter == '<')
            {
                ++iter;

                append_token(tk_brk_template_open, 3);
                //  It's definitely an opening template bracket.

                return true;
            }

            append_token(tk_namespace, 2);
            //  It's definitely a namespace access.

            return true;
        }

        append_token(tk_opr_tern_else, 1);
        return true;

    case '?':
        //  Can be either ternary if, maybe member access, or null coalescion.

        if (iter != contEnd)
            switch (*iter)
            {
            case '?':
                //  Null coalescion.
                ++iter;
                append_token(tk_opr_null_coalesce, 2);
                return true;

            case '.':
                //  Maybe member access.
                ++iter;
                append_token(tk_opr_member_maybe, 2);
                return true;

            default:
                //  No? Then it's simply a ternary if.
                break;
            }

        append_token(tk_opr_tern_if, 1);
        return true;

    case '!':
        //  Can be either logical not, inequality, or bitwise/logical nor or nand.

        if (iter != contEnd)
            switch ((c = *iter))
            {
            case '=':
                //  Inequality.
                ++iter;
                append_token(tk_opr_neq, 2);
                return true;

            case '&': case '|':
                //  Bitwise or logical, nor or nand.
                ++iter;
                return produce_from_binary_operator(c, true);

            default:
                //  No? Then it's simply a logical not.
                break;
            }

        append_token(tk_opr_lnot, 1);
        return true;

    case '+': case '-': case '*': case '/':
    case '%': case '^': case '&': case '|':
    case '<': case '>':
        return produce_from_binary_operator(c, false);

    case '\'':
        return produce_string(true);
    case '"':
        return produce_string(false);

    case '0': case '1': case '2': case '3':
    case '4': case '5': case '6': case '7':
    case '8': case '9':
        return produce_number(c);

    default:
        //  Everything else is identifiers.
        return produce_identifier();
    }
}

bool tokenizer::produce_identifier()
{
    token * const tk = append_token(tk_identifier, 1);
    //  Token is created now, but modified.

next_identifier_char:
    if (iter != contEnd)
        switch (*iter)
        {
        case ' ': case '\n': case '\t': case '\v': case '\f': case '\r':
            //  Whitespaces.
        case '(': case ')': case '[': case ']': case '{': case '}':
            //  Parentheses.
        case '~': case '!': case '@' : case '#': case '$': case '%' :
        case '^': case '&': case '*' : case '-': case '+': case '=' :
        case ';': case ':': case '\'': case '"': case ',': case '<' :
        case '.': case '>': case '/' : case '?': case '|': case '\\':
            //  Pretty much every symbol except '_'
            break;
            //  Move on; don't increment.

        default:
            //  Everything else is identifiers.

            ++iter;
            goto next_identifier_char;
            //  Advance to the next character.
        }

    tk->value = code->contents.substr(tk->position.index, iter - contStart - tk->position.index);
    return true;
}

bool tokenizer::produce_number(char c)
{
    token * const tk = append_token(tk_integer, 1);
    unsigned int base = 10, digits = 1, digitsBeforeSeparator = 1337;

    if (c == '0')
    {
        if (iter == contEnd)
            goto wrap_up;
        //  It's just 0.

        switch (*iter)
        {
        case 'b': base = 2;  break;
        case 'o': base = 8;  break;
        case 'x': base = 16; break;

        case ' ': case '\n': case '\t': case '\v': case '\f': case '\r':
        case '(': case ')': case '[' : case ']': case '{': case '}' :
        case '~': case '!': case '@' : case '#': case '$': case '%' :
        case '^': case '&': case '*' : case '-': case '+': case '=' :
        case ';': case ':': case '\'': case '"': case ',': case '<' :
        case '.': case '>': case '/' : case '?': case '|': case '\\':
            goto wrap_up;

        default:
            error_position = position_in_code(last_line, iter - contStart);
            error = std::string("Expected 'b', 'o' or 'x' after '0' in integer, not ") + *iter;
            return false;
        }

        ++iter;
        digits = 0;
    }

    //  If it's not 0, then it's another base 10 digit.

    for (/* nothing */; c = *iter, iter < contEnd; ++iter, ++digitsBeforeSeparator)
    {
        //  Love the comma operator!

        if (c == '_')
            continue;
        //  These are skipped.

        switch (base)
        {
        case  2: if (is_digit_bin(c)) goto score_digit; else break;
        case  8: if (is_digit_oct(c)) goto score_digit; else break;
        case 10: if (is_digit_dec(c)) goto score_digit; else break;
        case 16: if (is_digit_hex(c)) goto score_digit; else break;
        }

        //  So, eh... This is not a digit. It may be either a decimal separator or a suffix.

        if (c == '.')
        {
            if (tk->type == tk_integer)
            {
                tk->type = tk_float;
                digitsBeforeSeparator = digits;

                continue;
                //  Next will be the decimals and prefixes.
            }
            else
            {
                //  Means this is not the first decimal separator.

                error_position = position_in_code(last_line, iter - contStart);
                error = "A number may not contain more than one decimal separator";
                return false;
            }
        }

        //  Not a decimal separator? Maybe it's the end of this integer.

        switch (c)
        {
        case ' ': case '\n': case '\t': case '\v': case '\f': case '\r':
        case '(': case ')': case '[' : case ']': case '{': case '}' :
        case '~': case '!': case '@' : case '#': case '$': case '%' :
        case '^': case '&': case '*' : case '-': case '+': case '=' :
        case ';': case ':': case '\'': case '"': case ',': case '<' :
        case '.': case '>': case '/' : case '?': case '|': case '\\':
            goto wrap_up;

        default:
            //  So the digits ended, and this character must be part of the suffix.
            goto find_suffix;
        }

    score_digit:
        ++digits;
        continue;
    }

    if (iter != contEnd)
    {
        //  Check for a proper size and sign suffix.

    find_suffix:
        decltype(iter) const suffixStart = iter;

        do ++iter; while (iter != contEnd && can_be_in_identifier(*iter));

        std::string const suffix = code->contents.substr(suffixStart - contStart, iter - suffixStart);

        if (tk->type == tk_integer)
            tk->int_data.suffix = suffix;
        else
            tk->float_data.suffix = suffix;
    }

wrap_up:
    if (digitsBeforeSeparator == 1)
    {
        //  The number ending in a period means the period is a member access token, not a decimal separator.

        tk->type = tk_integer;
        --iter;
    }

    tk->value = code->contents.substr(tk->position.index, iter - contStart - tk->position.index);

    if (tk->type == tk_integer)
    {
        uint128_t res = tk->int_data.number = 0;

        switch (base)
        {
        case 2:
            //  This skips the `0b`.
            for (auto it = tk->value.begin() + 2; it < tk->value.end() && is_digit_bin(*it); ++it)
                if (*it != '_')
                    res = (res << 1) | (*it - '0');
            break;

        case 8:
            //  This skips the `0o`.
            for (auto it = tk->value.begin() + 2; it < tk->value.end() && is_digit_oct(*it); ++it)
                if (*it != '_')
                    res = (res << 3) | (*it - '0');
            break;

        case 10:
            for (auto it = tk->value.begin(); it < tk->value.end() && is_digit_dec(*it); ++it)
                if (*it != '_')
                    res = res * 10 + (*it - '0');
            break;

        case 16:
            //  This skips the `0x`.
            for (auto it = tk->value.begin() + 2; it < tk->value.end() && is_digit_hex(*it); ++it)
                if (*it <= '9')
                    res = (res << 4) | (*it - '0');
                else if (*it <= 'F')
                    res = (res << 4) | (*it - '7');
                else if (*it != '_')
                    res = (res << 4) | (*it - 'W');
            break;
        }

        tk->int_data.number = res;
    }
    else
        switch (base)
        {
        case 10:
            tk->float_data.number = atof(tk->value.c_str());
            break;

        case 2: case 8: case 16:
            error_position = tk->position;
            error = "Floating point literals may only be decimal";
            return false;
        }

    return true;
}

bool tokenizer::produce_from_binary_operator(char c, bool const hasExclamationMark)
{
    char const firstChar = c;

    if (iter == contEnd)
    {
    return_single_char_operator:
        if (hasExclamationMark)
            switch (firstChar)
            {
            case '&': append_token(tk_opr_bnand, 2); return true;
            case '|': append_token(tk_opr_bnor,  2); return true;

            default: assert(!"This should not happen."); return false;
            }
        else
            switch (firstChar)
            {
            case '+': append_token(tk_opr_add,  1); return true;
            case '-': append_token(tk_opr_sub,  1); return true;
            case '*': append_token(tk_opr_mul,  1); return true;
            case '/': append_token(tk_opr_div,  1); return true;
            case '%': append_token(tk_opr_mod,  1); return true;
            case '^': append_token(tk_opr_xor,  1); return true;
            case '&': append_token(tk_opr_band, 1); return true;
            case '|': append_token(tk_opr_bor,  1); return true;
            case '<': append_token(tk_opr_lt,   1); return true;
            case '>': append_token(tk_opr_gt,   1); return true;

            default: assert(!"This should not happen."); return false;
            }
    }

    c = *iter++;
    //  This increments, but may be decremented later.

    if (c == firstChar)
    {
        //  Doubled symbol! Let's see.

        switch (c)
        {
        case '+': append_token(tk_opr_inc,  2); return true;
        case '-': append_token(tk_opr_dec,  2); return true;
            //  These two are easy.

        case '/': return produce_comment(false);
            //  Oh, this is a comment!

        default: break;
            //  Others can have even more characters.
        }

        if (iter != contEnd)
        {
            c = *iter++;

            if (c == '=')
            {
                //  Two identical symbols followed by '='.

                if (hasExclamationMark)
                    switch (firstChar)
                    {
                    case '&': append_token(tk_opr_asgn_lnand, 4); return true;
                    case '|': append_token(tk_opr_asgn_lnor,  4); return true;

                    default: assert(!"This should not happen."); return false;
                    }
                else
                    switch (firstChar)
                    {
                    case '*': append_token(tk_opr_asgn_pow,   3); return true;
                    case '%': append_token(tk_opr_asgn_usr_1, 3); return true;
                    case '^': append_token(tk_opr_asgn_usr_2, 3); return true;
                    case '&': append_token(tk_opr_asgn_land,  3); return true;
                    case '|': append_token(tk_opr_asgn_lor,   3); return true;
                    case '<': append_token(tk_opr_asgn_shl,   3); return true;
                    case '>': append_token(tk_opr_asgn_shr,   3); return true;

                    default: assert(!"This should not happen."); return false;
                    }
            }
            else if (c == firstChar && (c == '<' or c == '>'))
            {
                //  Three identical symbols, so only 4 possibilities.

                if (iter != contEnd)
                {
                    c = *iter++;

                    if (c == '=')
                        switch (firstChar)
                        {
                        case '<': append_token(tk_opr_asgn_rol, 4); return true;
                        case '>': append_token(tk_opr_asgn_ror, 4); return true;
                        }
                    //  Three identical symbols followed by '='.

                    --iter;
                }

                //  Just three identical symbols.

                switch (firstChar)
                {
                case '<': append_token(tk_opr_rol, 3); return true;
                case '>': append_token(tk_opr_ror, 3); return true;
                }

                //  Other three-symbol sequences are not meaningful, at least yet.
            }
            
            --iter;
            //  Not part of the operator.
        }

        //  So it's exactly two identical symbols.

        if (hasExclamationMark)
            switch (firstChar)
            {
            case '&': append_token(tk_opr_lnand, 3); return true;
            case '|': append_token(tk_opr_lnor,  3); return true;

            default: assert(!"This should not happen."); return false;
            }
        else
            switch (firstChar)
            {
            case '*': append_token(tk_opr_pow,   2); return true;
            case '%': append_token(tk_opr_usr_1, 2); return true;
            case '^': append_token(tk_opr_usr_2, 2); return true;
            case '&': append_token(tk_opr_land,  2); return true;
            case '|': append_token(tk_opr_lor,   2); return true;
            case '<': append_token(tk_opr_shl,   2); return true;
            case '>': append_token(tk_opr_shr,   2); return true;

            default: assert(!"This should not happen."); return false;
            }
    }
    else if (firstChar == '/' && c == '*')
        return produce_comment(true);
    else if (c == '=')
    {
        if (hasExclamationMark)
            switch (firstChar)
            {
            case '&': append_token(tk_opr_asgn_bnand, 3); return true;
            case '|': append_token(tk_opr_asgn_bnor,  3); return true;

            default: assert(!"This should not happen."); return false;
            }
        else
            switch (firstChar)
            {
            case '+': append_token(tk_opr_asgn_add,  2); return true;
            case '-': append_token(tk_opr_asgn_sub,  2); return true;
            case '*': append_token(tk_opr_asgn_mul,  2); return true;
            case '/': append_token(tk_opr_asgn_div,  2); return true;
            case '%': append_token(tk_opr_asgn_mod,  2); return true;
            case '^': append_token(tk_opr_asgn_xor,  2); return true;
            case '&': append_token(tk_opr_asgn_band, 2); return true;
            case '|': append_token(tk_opr_asgn_bor,  2); return true;

            case '<': append_token(tk_opr_lte,       2); return true;
            case '>': append_token(tk_opr_gte,       2); return true;

            default: assert(!"This should not happen."); return false;
            }
    }

    --iter;
    goto return_single_char_operator;
}

bool tokenizer::produce_comment(bool multiline)
{
    token * const tk = append_token(tk_comment, 2);

    //  Note the assignment.
    if ((tk->is_multiline = multiline))
    {
        while (iter != contEnd)
        {
            if (iter[-1] == '*' && iter[0] == '/')
            {
                //  This is the end of the multi-line comment.

                ++iter;
                tk->value = code->contents.substr(tk->position.index, iter - contStart - tk->position.index);
                return true;
            }
            else if (*iter == '\n')
            {
                //  Need to process newlines.

                last_line = new line_of_code(last_line, iter - contStart);
            }

            ++iter;
        }

        //  If this point is reached, the multi-line comment reaches the end of the file!

        error_position = position_in_code(last_line, contEnd - contStart);
        error = "Multi-line comment reaches end of file; lacks ending.";
        return false;
    }
    else
    {
        //  This comment ends at the end of the line, or the end of the file.

        while (iter != contEnd && *iter != '\n') ++iter;

        tk->value = code->contents.substr(tk->position.index, iter - contStart - tk->position.index);
        return true;
    }
}

bool tokenizer::produce_string(bool character)
{
    token * const tk = append_token(character ? tk_char : tk_string, 1);
    //  Token is created now, but modified.

    std::stringstream ss;
    bool inEscape = false;

    while (iter != contEnd)
    {
        char c = *iter++;

        if (inEscape)
        {
            switch (c)
            {
            case '0': ss << '\0'; break;
            case 'a': ss << '\a'; break;
            case 'b': ss << '\b'; break;
            case 't': ss << '\t'; break;
            case 'n': ss << '\n'; break;
            case 'v': ss << '\v'; break;
            case 'f': ss << '\f'; break;
            case 'r': ss << '\r'; break;

            case '\r':
                //  A speshul case.

                if (iter != contEnd && *iter == '\n')
                {
                    //  A newline, possibly following a carriage return.
                    ++iter;

                case '\n':
                    last_line = new line_of_code(last_line, iter - contStart);
                    //  Newline needs to be processed.

                    if (iter != contEnd && *iter == '\r')
                        ++iter;
                    //  If there is a carriage return after the newline, it is
                    //  forgotten as well.

                    ss << '\n';
                    break;
                }

                //  Reaching this means it's just a carriage return.

            default: ss << c; break;
            }

            inEscape = false;
        }
        else
        {
            switch (c)
            {
            case '\\': inEscape = true; break;

            case '\'':
                if (character)
                    goto successful_string;
                else
                {
                    ss << c;
                    break;
                }

            case '"':
                if (character)
                {
                    ss << c;
                    break;
                }
                else
                    goto successful_string;

            case '\r':
                //  This don't look good.

                if (iter != contEnd && *iter == '\n')
                {
                    //  A newline, possibly following a carriage return. Very not good.
                    ++iter;

                case '\n':
                    error_position = position_in_code(last_line, iter - contStart);
                    error = "String and character literals cannot contain unescaped newlines.";
                    return false;
                }

                //  Reaching this means it's just a carriage return.

                error_position = position_in_code(last_line, iter - contStart);
                error = "String and character literals cannot contain unescaped carriage returns.";
                return false;

            default: ss << c; break;
            }
        }
    }

    //  Reaching this point means the end of the file was encountered.

    error_position = position_in_code(last_line, iter - contStart);
    error = character
        ? "Character literal reaches end of file; lacks ending."
        : "String literal reaches end of file; lacks ending.";
    return false;

successful_string:
    tk->value = ss.str();
    return true;
}

token * tokenizer::append_token(token_types tp, long off)
{
    token * const res = new token(tp, last_line, iter - contStart - off);

    if (last_token != nullptr)
        last_token->next = res;

    return last_token = res;
}

/***************************
    token_consumer class
***************************/

token_consumer::token_consumer(tokenizer * src, token * cur)
    : source(src)
    , current(cur)
{

}

token_consumer * token_consumer::fork()
{
    return new token_consumer(this->source, this->current);
}

void token_consumer::join(token_consumer * other)
{
    assert(this->source == other->source);
    //  Same token source.

    assert(other->current->position.line->number >= this->current->position.line->number);
    //  At least the same line.

    assert(other->current->position.index >= this->current->position.index);
    //  At least the same token.

    this->current = other->current;

    delete other;
}

token * token_consumer::fetch_token()
{
    if (this->current == nullptr)
    {
        if (this->source->first_token != nullptr)
            return this->current = this->source->first_token;
        //  Maybe another consumer requested production of tokens already.
        //  If not, proceed...
    }
    else if (this->current->next != nullptr)
    {
        return this->current = this->current->next;
        //  Other tokens were produced.
    }
    else
    {
        if (this->current != this->source->last_token)
        {
            //  If this fails, linkage wasn't maintained. Otherwise, proceed...

            std::cerr << "linkage broken between " << *current << " and "
                << *(source->last_token) << std::endl;

            assert(this->current == this->source->last_token);
        }
    }

    if (this->source->produce_token())
        return this->current = this->source->last_token;
    //  If the first token was successfully produced, return it.

    return nullptr;
    //  This means error. Which is bad.
}
