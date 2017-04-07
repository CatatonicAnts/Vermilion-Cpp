#pragma once

#include <iostream>

struct uint128_t
{
    inline uint128_t(__uint128_t const v) : val( v) { }

#define OP(op) \
    inline uint128_t operator op(uint128_t const other) const { return this->val op other.val; }

    OP(+)  OP(-)  OP(*) OP(/) OP(%)
    OP(>>) OP(<<) OP(|) OP(&) OP(^)

#undef OP

#define OP(op) \
    inline bool operator op(uint128_t const other) const { return this->val op other.val; }

    OP(>) OP(<) OP(==) OP(!=)

#undef OP

#define OP(op) \
    inline explicit operator op() const { return (op)this->val; }

    OP(uint8_t) OP(uint16_t) OP(uint32_t) OP(uint64_t)
    OP( int8_t) OP( int16_t) OP( int32_t) OP( int64_t)

#undef OP

private:
    __uint128_t val;
};

std::ostream & operator <<(std::ostream & st, uint128_t val);
