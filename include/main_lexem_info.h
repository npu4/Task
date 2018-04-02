/*
    File:    main_lexem_info.h
    Created: 23 March 2018 at 08:08 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#ifndef MAIN_LEXEM_INFO_H
#define MAIN_LEXEM_INFO_H
enum class Main_lexem_code : unsigned short{
    None,                 Unknown,              U8String,
    U16String,            U32String,            Char8,
    Char16,               Char32,               Integer,
    Float32,              Float64,              Float80,
    Float128,             Kw_byte,              Kw_unsigned,
    Kw_uint8,             Kw_uint16,            Kw_uint32,
    Kw_uint64,            Kw_uint128,           Kw_long,
    Kw_small,             Kw_read,              Kw_float,
    Kw_float32,           Kw_float64,           Kw_float80,
    Kw_float128,          Kw_return,            Kw_endif,
    Kw_print,             Kw_exit,
    Kw_for,               Kw_if,                
    Kw_value,             Kw_else,
    Kw_elif,              Kw_uses,              Kw_true,
    Kw_const,             Kw_bool,              Kw_bool8,
    Kw_bool16,            Kw_bool32,            Kw_bool64,
    Kw_false,             Kw_array,             Kw_module,
    Kw_void,              Kw_var,
    Kw_enum,              Kw_repeat,            Kw_while,
    Kw_as_long_as,        Kw_export,            Kw_case,
    Kw_continue,          Kw_match,             Kw_char,
    Kw_char8,             Kw_char16,            Kw_char32,
    Kw_ref,               Kw_string,            Kw_string8,
    Kw_string16,          Kw_string32,          Kw_struct,
    Kw_type,              Kw_then,              
    Kw_function,          Kw_int,               Kw_int8,
    Kw_int16,             Kw_int32,             Kw_int64,
    Kw_int128,            Comma,                Semicolon,
    Colon,                Dot,                  Open_round_bracket,
    Closed_round_bracket, Open_square_bracket,  Closed_square_bracket,
    Equal_sign,           At_sign,              
	Variant_delimiter,    Conditional_op,
    Logical_or,           Logical_or_not,       Logical_xor,
    Logical_xor_not,      Logical_and,          Logical_and_not,
    Logical_not,          Less_than,            Greater_than,
    Lower_or_equals,      Greater_or_equals,    Not_equals,
    Bitwise_xor,          Bitwise_or,           Bitwise_xor_not,
    Bitwise_or_not,       Open_curly_bracket,   Closed_curly_bracket,
    Bitwise_and,          Bitwise_and_not,      Left_shift,
    Right_shift,          Signed_right_shift,   Bitwise_not,
    Plus,                 Minus,                Mul,
    Div,                  Fdiv,                 Mod,
    Power,                Fpower,               Sizeof,
    Dimension_size,       Allocated_array_open, Allocated_array_close,
    Module_name_prefix,   Scope_resolution,     Assign_to_field,
    Label_prefix,         Label_suffix,         Assign,
    Pattern_is,
    Br_dots,              Ident
};

struct Main_lexem_info{
    Main_lexem_code code;
    union{
        size_t            ident_index;
        size_t            string_index;
        char32_t          c;
        unsigned __int128 int_val;
        __float128        float_val;
    };
};
#endif