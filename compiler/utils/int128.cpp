#include "common.hpp"

/* UINT64_MAX 18446744073709551615ULL */
#define P10_9 10000000000000000000ULL /* 19 zeroooooes */

std::ostream & operator <<(std::ostream & st, uint128_t const val)
{
    if (val > UINT64_MAX)
    {
        uint128_t const high = val / P10_9;
        uint64_t  const low  = (uint64_t)(val % P10_9);
        
        return st << high << low;
    }
    else
        return st << (uint64_t)val;
}
