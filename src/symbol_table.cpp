/*
    File:    symbol_table.cpp
    Created: 03 April 2018 at 06:21 Moscow time
    Author:  Гаврилов Владимир Сергеевич
    E-mails: vladimir.s.gavrilov@gmail.com
             gavrilov.vladimir.s@mail.ru
             gavvs1977@yandex.ru
*/

#include "../include/symbol_table.h"
#include <cstddef>
#include <forward_list>

namespace symbol_table{
    struct Symbol_table::Impl{
    private:
        std::forward_list<scope::Scope> table_;
    public:    
        void create_new_scope();
        void delete_last_scope();        
        Search_result search(ast::Id ident_idx);
        void insert(ast::Id ident_idx, const scope::Id_info& id_info);
    };
    
    Symbol_table::Symbol_table() : impl_(std::make_unique<Impl>()) {}
    
    Symbol_table::~Symbol_table() = default;  
    
    void Symbol_table::Impl::delete_last_scope()
    {
        if(!table_.empty()){
            table_.pop_front();
        };
    }    
    
    void Symbol_table::Impl::create_new_scope()
    {
        table_.push_front(scope::Scope());
    } 
    
    Search_result Symbol_table::Impl::search(ast::Id ident_idx)
    {
        Search_result result;
        result.status         = Search_result::Not_found;
        size_t        counter = 0;
        for(const auto& s : table_){
            auto it = s.search(ident_idx);
            if(it != s.end()){
                result.status     = (counter == 0) ? 
                                    Search_result::Found_in_the_current_scope :
                                    Search_result::Found_in_an_inner_scope;
                auto info         = it->second;
                result.attr       = info.attr;
                result.definition = info.definition;
                break;
            }
            ++counter;
        }
        return result;
    }
    
    void Symbol_table::Impl::insert(ast::Id ident_idx, const scope::Id_info& id_info)
    {
        if(!table_.empty()){
            table_.push_front(scope::Scope());
        };
        auto& current_scope = table_.front();
        current_scope.insert(ident_idx, id_info);
    }

    Search_result Symbol_table::search(ast::Id ident_idx)
    {
        auto result = impl_->search(ident_idx);
        return result;
    }
    
    void Symbol_table::insert(ast::Id ident_idx, const scope::Id_info& id_info)
    {
        impl_->insert(ident_idx, id_info);
    }
    
    void Symbol_table::delete_last_scope()
    {
        impl_->delete_last_scope();
    }    
    
    void Symbol_table::create_new_scope()
    {
        impl_->create_new_scope();
    }    
};