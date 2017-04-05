#include <string>

namespace verm
{
    class code_unit;
    class line_of_code;
    class token;
    class tokenizer;
    class token_consumer;
    //  Forward declarations.

    enum token_types
    {
        tk_end_of_file,
        //  This one is speshul.
        tk_identifier,
        //  Includes keywords (known as reserved identifiers)
        tk_integer, tk_float,
        //  Any formats.
        tk_string, tk_char,
        //  Includes fancy types.
        tk_comment,
        //  Ignored by the parser.

        tk_opr_asgn, tk_comma, tk_member, tk_namespace, tk_end_statement,
        //  '='          ','       '.'        '::'          ';'
        tk_opr_member_maybe, tk_opr_null_coalesce,
        //  '?.'                 '??'

        tk_brk_rnd_open,        //  Open round bracket '('
        tk_brk_rnd_close,       //  Close round bracket ')'
        tk_brk_sq_open,         //  Open square bracket '['
        tk_brk_sq_close,        //  Close square bracket ']'
        tk_brk_crl_open,        //  Open curly bracket '{'
        tk_brk_crl_close,       //  Close curly bracket '}'

        tk_brk_template_open,   //  Open template bracket '::<'

        tk_opr_add, tk_opr_sub, tk_opr_mul, tk_opr_div, tk_opr_mod, tk_opr_xor,
        //  '+'         '-'         '*'         '/'         '%'         '^'
        tk_opr_bor, tk_opr_band, tk_opr_bcomp, tk_opr_lor, tk_opr_land, tk_opr_lnot,
        //  '|'         '&'          '~'           '||'        '&&'         '!'
        tk_opr_bnor, tk_opr_bnand, tk_opr_lnor, tk_opr_lnand,
        //  '!|'         '!&'          '!||'        '!&&'
        tk_opr_shl, tk_opr_shr, tk_opr_rol, tk_opr_ror,
        //  '<<'        '>>'        '<<<'       '>>>'

        tk_opr_pow, tk_opr_inc, tk_opr_dec,
        //  '**'        '++'        '--'

        tk_opr_asgn_add, tk_opr_asgn_sub, tk_opr_asgn_mul, tk_opr_asgn_div,
        //  '+='             '-='             '*='             '/='
        tk_opr_asgn_mod, tk_opr_asgn_xor, tk_opr_asgn_bor, tk_opr_asgn_band,
        //  '%='             '^='             '|='             '&='
        tk_opr_asgn_lor, tk_opr_asgn_land, tk_opr_asgn_bnor, tk_opr_asgn_bnand,
        //  '||='            '&&='             '!|='             '!&='
        tk_opr_asgn_lnor, tk_opr_asgn_lnand,
        //  '!||='            '!&&='
        tk_opr_asgn_shl, tk_opr_asgn_shr, tk_opr_asgn_rol, tk_opr_asgn_ror,
        //  '<<='            '>>='            '<<<='           '>>>='
        tk_opr_asgn_pow,
        //  '**='

        tk_opr_lt, tk_opr_gt, tk_opr_lte, tk_opr_gte, tk_opr_eq, tk_opr_neq,
        //  '<'        '>'        '<='        '>='        '=='       '!='

        tk_opr_ref, tk_opr_deref,
        //  '@'         '#'

        tk_opr_tern_if, tk_opr_tern_else,
        //  '?'             ':'

        //  These are binay operators for the programmers to define for their
        //  own needs.
        tk_opr_usr_1, tk_opr_usr_2, tk_opr_asgn_usr_1, tk_opr_asgn_usr_2,
        //  '%%'          '^^'          '%%='              '^^='
    };

    char const * token_type_to_string(token_types tt);

    enum integer_sizes
    {
        ints_8, ints_16, ints_32, ints_64, ints_128,
    };

    struct tok_int_data
    {
        integer_sizes size;
        bool is_signed;
        __uint128_t integer;
    };

    enum float_formats
    {
        floatf_single, floatf_double,
    };

    struct tok_float_data
    {
        float_formats format;

        __extension__ union
        {
            float valueF;
            double valueD;
        };
    };

    class code_unit
    {
        friend class tokenizer;

        std::string const filename;
        std::string const contents;

        line_of_code * first_line;

        inline code_unit(std::string fn, std::string conts)
            : filename( fn)
            , contents(conts)
            , first_line(nullptr)
        {

        }
    };

    class line_of_code
    {
        friend class tokenizer;
        friend class token_consumer;

        code_unit * const code;
        long start, end, number;

        token * first_token;
        line_of_code * next;

        inline line_of_code(code_unit * cd, long st, long nu)
            : code( cd), start(st), end(-1), number(nu)
            , first_token(nullptr), next(nullptr)
        {
            
        }

        inline line_of_code(line_of_code * prev, long st)
            : code( prev->code), start(st), end(-1), number(prev->number + 1)
            , first_token(nullptr), next(nullptr)
        {
            prev->end = st;
            prev->next = prev;
        }

    public:
        inline long get_number() const { return this->number; }
    };

    struct position_in_code
    {
        line_of_code * line;
        long index;

        inline position_in_code() : line( nullptr), index(-1) { }

        inline position_in_code(line_of_code * ln, long ind)
            : line( ln)
            , index(ind)
        {

        }
    };

    class token
    {
        friend class tokenizer;
        friend class token_consumer;

        token_types type;
        std::string value;

        __extension__ union
        {
            bool is_reserved;   //  For identifiers.
            bool is_multiline;  //  For comments.
            tok_int_data int_data;
            tok_float_data float_data;
        };

        position_in_code position;
        token * next;

        inline token(token_types tp, position_in_code pos)
            : type( tp), value(), is_reserved(false), position(pos), next(nullptr)
        {

        }

        inline token(token_types tp, line_of_code * ln, long ind)
            : type( tp), value(), is_reserved(false), position(ln, ind), next(nullptr)
        {

        }

    public:
        inline token_types              get_type()     const { return this->type;     }
        inline std::string              get_value()    const { return this->value;    }
        inline position_in_code const & get_position() const { return this->position; }
    };

    std::ostream & operator <<(std::ostream & st, token const & tk);

    class tokenizer
    {
        friend class token_consumer;

        token * first_token, * last_token;
        code_unit * code;
        line_of_code * last_line;

        std::string::const_iterator contStart, contEnd, iter;

        bool produce_token();
        bool produce_identifier();
        bool produce_number(char c);
        bool produce_from_binary_operator(char c, bool hasExclamationMark);
        bool produce_comment(bool multiline);
        bool produce_string(bool character);
        token * append_token(token_types tp, long off);

    public:
        std::string error;
        position_in_code error_position;

        tokenizer(std::string fn, std::string conts);
    };

    class token_consumer
    {
        tokenizer * source;
        token * current;

    public:
        token_consumer(tokenizer * src, token * cur);
        inline token_consumer(tokenizer * src) : token_consumer(src, src->first_token) { }

        token_consumer * fork();
        void join(token_consumer * other);

        inline tokenizer * get_source() const { return this->source; }
        inline token * get_token() const { return this->current; }

        token * fetch_token();
    };
}
