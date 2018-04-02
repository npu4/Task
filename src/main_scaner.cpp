/*
    File:    main_scaner.cpp
    Created: 23 March 2018 at 08:14 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#include <utility>
#include "../include/main_scaner.h"

Main_lexem_info Main_scaner::convert_lexeme(const Lexem_info& lexeme)
{
    Main_lexem_info result;
    Lexem_code      code   = lexeme.code;
    int             offset = static_cast<int>(Main_lexem_code::Integer) - Integer;
    switch(code){
        case None: case Unknown:
            result.code         = static_cast<Main_lexem_code>(code);
            break;
        case Kw_byte ... Br_dots:
            result.code         = static_cast<Main_lexem_code>(code + offset);
            break;
        case Integer:
            result.code         = Main_lexem_code::Integer;
            result.int_val      = lexeme.int_val;
        case Float32: case Float64: case Float80: case Float128:
            result.code         = static_cast<Main_lexem_code>(code + offset);
            result.float_val    = lexeme.float_val;
            break;
        case Ident:
            result.code         = Main_lexem_code::Ident;
            result.ident_index  = lexeme.ident_index;
            break;
        case Char:
            result.code         = Main_lexem_code::Char32;
            result.c            = lexeme.c;
            break;
        case String:
            result.code         = Main_lexem_code::U32String;
            result.string_index = lexeme.string_index;
            break;
        default:
            ;
    }
    return result;
}

static const std::u32string unicode_prefix[] = {
    U"u8", U"u16", U"u32"
};

static constexpr int NOT_FOUND = -1;

static int search_prefix(const std::u32string& str)
{
     int result = NOT_FOUND;
     for(int i = 0; i < 3; ++i){
        if(str == unicode_prefix[i]){
            return i;
        }
     }
     return result;
}

static const std::pair<Main_lexem_code, Main_lexem_code> str_char_codes[] = {
    {Main_lexem_code::Char8,  Main_lexem_code::U8String},
    {Main_lexem_code::Char16, Main_lexem_code::U16String},
    {Main_lexem_code::Char32, Main_lexem_code::U32String}
};

Main_lexem_info Main_scaner::current_lexem()
{
    aelic_               = (aeli_ = aux_scaner_->current_lexem()).code;
    lexem_begin_line_    = aux_scaner->lexem_begin_line_number();;
    lexem_begin_         = aux_scaner->lexem_begin_ptr();
    if(aelic_ != Ident){
        return convert_lexeme(aeli_);
    }
    auto ident_as_string = et_.ids_trie->get_string(aeli_.ident_index);
    int  idx             = search_prefix(ident_as_string);
    if(idx == NOT_FOUND){
        return convert_lexeme(aeli_); 
    }
    char32_t* pch = loc_->pcurrent_char;
    if(*pch != U"\""){
        return convert_lexeme(aeli_);         
    }

    aelic_               = (aeli_ = aux_scaner_->current_lexem()).code;
    Main_lexem_info result;
    if(aelic_ == Char){
        result.code            = str_char_codes[idx].first;
        result.c               = aeli_.c;
    }else{
        result.code            = str_char_codes[idx].second;
        result.string_index    = aeli_.string_index;
      
    }
    return result;
}

size_t Main_scaner::lexem_begin_line_number() const
{
    return lexem_begin_line_;
}

void Main_scaner::back()
{
    loc_->pcurrent_char = lexem_begin_;
    loc_->current_line  = lexem_begin_line_;
}