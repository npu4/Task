/*
    File:    scope.cpp
    Created: 02 April 2018 at 16:41 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#include <map>
#include "../include/scope.h"
#include "../include/ast.h"

namespace scope{
    struct Scope::Impl{
        std::map<ast::Id, Id_info> scope_;
        
        Search_result search(ast::Id ident_idx);
        void insert(ast::Id ident_idx, const Id_info& id_info);
    };
    
    Scope::Scope() : impl_(std::make_shared<Impl>()) {}
    
    Scope::~Scope() = default;  

    Search_result Scope::Impl::search(ast::Id ident_idx)
    {
        Search_result result;
        auto it = scope_.find(ident_idx);
        if(it != scope_.end()){
            result.status     = Search_result::Found;
            auto info         = it->second;
            result.attr       = info.attr;
            result.definition = info.definition;
        }else{
            result.status     = Search_result::Not_found;
            result.definition = nullptr;
        }
        return result;
    }
    
    void Scope::Impl::insert(ast::Id ident_idx, const Id_info& id_info)
    {
        scope_.insert(ident_idx, id_info);
    }

    Search_result Scope::search(ast::Id ident_idx)
    {
        auto result = impl_->search(ident_idx);
        return result;
    }
    
    void Scope::insert(ast::Id ident_idx, const Id_info& id_info)
    {
        impl_->insert(ident_idx, id_info);
    }
};