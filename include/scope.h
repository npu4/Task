/*
    File:    scope.h
    Created: 02 April 2018 at 16:41 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#ifndef SCOPE_H
#define SCOPE_H
#include <memory>
#include "../include/ast.h"
namespace scope{
    enum class Id_attribute{
        Used_module,       Unknown_but_exported, Module_name,
        Type_name,         Var_name,             Const_name,
        Func_name,         Label_name,           Exported_type_name,   
        Exported_var_name, Exported_const_name,  Exported_func_name,
        Func_proto,        Struct_component,     Enum_component,       
        Value_param,       Ref_param,            Const_ref_param
    };
    
    enum class Search_status{
        Not_found, Found
    };
    
    struct Search_result{
        Search_status                    status;
        Id_attribute                     attr;
        std::shared_ptr<ast::Definition> definition;
    };
    
    struct Id_info{
        Id_attribute                     attr;
        std::shared_ptr<ast::Definition> definition;
    };
    
    class Scope{
    public:
        Scope();
        Scope(const Scope&) = default;
        ~Scope();
       
        Search_result search(ast::Id ident_idx);
        void insert(ast::Id ident_idx, const Id_info& id_info);
    private:
        struct Impl;
        std::make_shared<Impl> impl_;
    };
};
#endif