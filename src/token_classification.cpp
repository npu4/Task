/*
    File:    token_classification.cpp
    Created: 25 March 2018 at 16:56 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#include "../include/token_classification.h"
static const Lexem_category lexeme_categories[] = {
    Lexem_category::None,                 Lexem_category::Unknown,
    Lexem_category::Literal,              Lexem_category::Literal,
    Lexem_category::Literal,              Lexem_category::Literal,
    Lexem_category::Literal,              Lexem_category::Literal,
    Lexem_category::Literal,              Lexem_category::Literal,
    Lexem_category::Literal,              Lexem_category::Literal,
    Lexem_category::Literal,              Lexem_category::Non_modified_type,
    Lexem_category::Modified_type,        Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Size_changer,         Lexem_category::Size_changer,
    Lexem_category::Kw_read,              Lexem_category::Modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Kw_return,            Lexem_category::Kw_endif,
    Lexem_category::Kw_print,             
    Lexem_category::Kw_exit,              Lexem_category::Kw_for,
    Lexem_category::Kw_if,                
    Lexem_category::Kw_value,             
    Lexem_category::Kw_else,              Lexem_category::Kw_elif,
    Lexem_category::Kw_uses,              Lexem_category::Literal,
    Lexem_category::Kw_const,             Lexem_category::Modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Literal,              Lexem_category::Kw_array,
    Lexem_category::Kw_module,            Lexem_category::Non_modified_type,
    Lexem_category::Kw_var,
    Lexem_category::Kw_enum,              Lexem_category::Kw_repeat,
    Lexem_category::Kw_while,             Lexem_category::Kw_as_long_as,
    Lexem_category::Kw_export,            Lexem_category::Kw_case,
    Lexem_category::Kw_continue,          Lexem_category::Kw_match,
    Lexem_category::Modified_type,        Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Kw_ref,               Lexem_category::Modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Kw_struct,
    Lexem_category::Kw_type,              Lexem_category::Kw_then,
    Lexem_category::Kw_function,
    Lexem_category::Modified_type,        Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Non_modified_type,    Lexem_category::Non_modified_type,
    Lexem_category::Comma,                Lexem_category::Semicolon,
    Lexem_category::Colon,                Lexem_category::Dot,
    Lexem_category::Open_round_bracket,   Lexem_category::Closed_round_bracket,
    Lexem_category::Open_square_bracket,  Lexem_category::Closed_square_bracket,
    Lexem_category::Equal_sign,           Lexem_category::At_sign,
    Lexem_category::Variant_delimiter,    Lexem_category::Conditional_op,
    Lexem_category::Lor_op,               Lexem_category::Lor_op,
    Lexem_category::Lor_op,               Lexem_category::Lor_op,
    Lexem_category::Land_op,              Lexem_category::Land_op,
    Lexem_category::Lnot,                 Lexem_category::Rel_op,
    Lexem_category::Rel_op,               Lexem_category::Rel_op,
    Lexem_category::Rel_op,               Lexem_category::Rel_op,
    Lexem_category::Bor_op,               Lexem_category::Bor_op,
    Lexem_category::Bor_op,               Lexem_category::Bor_op,
    Lexem_category::Open_curly_bracket,   Lexem_category::Closed_curly_bracket,
    Lexem_category::Band_op,              Lexem_category::Band_op,
    Lexem_category::Band_op,              Lexem_category::Band_op,
    Lexem_category::Band_op,              Lexem_category::Bnot,
    Lexem_category::Add_op,               Lexem_category::Add_op,
    Lexem_category::Mul_op,               Lexem_category::Mul_op,
    Lexem_category::Mul_op,               Lexem_category::Mul_op,
    Lexem_category::Pow_op,               Lexem_category::Pow_op,
    Lexem_category::Size,                 Lexem_category::Dim_size,
    Lexem_category::Allocated_array_open, Lexem_category::Allocated_array_close,
    Lexem_category::Module_name_prefix,   Lexem_category::Scope_resolution,
    Lexem_category::Assign_to_field,      Lexem_category::Label_prefix,
    Lexem_category::Label_suffix,         Lexem_category::Assign_op,
    Lexem_category::Pattern_is,
    Lexem_category::Br_dots,              Lexem_category::Ident
};

Lexem_category get_lexem_category(const Main_lexem_info& li)
{
    return lexeme_categories[static_cast<unsigned>(li.code)];
}