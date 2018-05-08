/*
    File:    symbol_table.h
    Created: 02 April 2018 at 17:42 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#ifndef SYMBOL_TABLE_H
#define SYMBOL_TABLE_H

#include <memory>
#include "../include/ast.h"
#include "../include/scope.h"

namespace symbol_table{  
    enum class Search_status{
        Not_found, Found_in_the_current_scope, Found_in_an_inner_scope
    };
    
    struct Search_result{
        Search_status                    status;
        scope::Id_attribute              attr;
        std::shared_ptr<ast::Definition> definition;
    };

    class Symbol_table{
    public:
        Symbol_table();
        Symbol_table(const Scope&) = default;
        ~Symbol_table();
        
        void create_new_scope();
        void delete_last_scope();
        
        Search_result search(ast::Id ident_idx);
        void insert(ast::Id ident_idx, const scope::Id_info& id_info);
    private:
        struct Impl;
        std::unique_ptr<Impl> impl_;
    };
};
#endif