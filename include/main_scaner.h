/*
    File:    main_scaner.h
    Created: 23 March 2018 at 07:36 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#ifndef MAIN_SCANER_H
#define MAIN_SCANER_H

#include <string>
#include <memory>
#include "../include/location.h"
#include "../include/error_count.h"
#include "../include/trie.h"
#include "../include/errors_and_tries.h"
#include "../include/aux_scaner.h"
#include "../include/main_lexem_info.h"

class Main_scaner{
public:
    Main_scaner()                        = default;
    Main_scaner(const Location_ptr& location, const Errors_and_tries& et) :
        aux_scaner_(std::make_unique<Aux_scaner>(location, et)),
        et_(et), loc_(location)
        {}
    Main_scaner(const Main_scaner&)      = default;
    ~Main_scaner()                       = default;

    Main_lexem_info current_lexem();
    size_t          lexem_begin_line_number() const;
    void            back();
private:
    std::unique_ptr<Aux_scaner> aux_scaner_;
    Errors_and_tries            et_;
    Location_ptr                loc_;

    size_t                      lexem_begin_line_;
    char32_t*                   lexem_begin_;
    Lexem_info                  aeli_;
    Lexem_code                  aelic_;
    
    Main_lexem_info convert_lexeme(const Lexem_info&);
};
#endif