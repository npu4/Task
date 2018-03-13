#ifndef TSCANER_H
#define TSCANER_H

#include "../include/abstract_scaner.h"
#include "../include/error_count.h"
#include "../include/location.h"
#include <string>"

enum codes : unsigned short {
    None,                      Unknown,             module,                   
    import,                    export,              var,                      
    type,                      array,               long,                     
    short,                     int,                 float,                    
    uint,                      bool,                char,                     
    string,                    void,                byte,                     
    int8_t,                    int16_t,             int32_t,                  
    int64_t,                   int128_t,            uint8_t,                  
    uint16_t,                  uint32_t,            uint64_t,                 
    uint128_t,                 float32_t,           float64_t,                
    float128_t,                bool8_t,             bool16_t,                 
    bool32_t,                  bool64_t,            char8_t,                  
    char16_t,                  char32_t,            string8_t,                
    string16_t,                string32_t,          fn,                       
    val,                       ref,                 const_ref,                
    enum,                      struct,              const,                    
    true,                      false,               if,                       
    then,                      elif,                else,                     
    endif,                     for,                 while,                    
    repeat,                    until,               break,                    
    continue,                  return,              read,                     
    print,                     match,               semicolon,                
    colon,                     equal,               not_equal,                
    open_square_bracket,       close_square_bracket,open_square_bracket_colon,
    close_square_bracket_colon,open_figure_bracket, close_figure_bracket,     
    open_round_bracket,        close_round_bracket, open_round_bracket_colon, 
    close_round_bracket_colon, scope_resolution,    question_mark,            
    logical_not,               logical_or,          not_logical_or,           
    logical_xor,               not_logical_xor,     logical_and,              
    not_logical_and,           less,                greater,                  
    less_or_equal,             greater_or_equal,    not,                      
    or,                        not_or,              xor,                      
    not_xor,                   and,                 not_and,                  
    lshift,                    rshift,              rshift_int,               
    plus,                      minus,               multiplication,           
    division,                  fract_division,      mod,                      
    exponentiation,            fract_exponentiation,sizeof,                   
    index,                     comma,               assignment,               
    structure_dereference
};

struct Lexem_info{
    codes code;
    union{
        size_t    ident_index;
        size_t    string_index;
        char32_t  c;
unsigned __int128 int_val; __float128 float_val;
    };
};

class tscaner : public Abstract_scaner<Lexem_info> {
public:
    tscaner() = default;
    tscaner(Location* location, const Errors_and_tries& et) :
        Abstract_scaner<Lexem_info>(location, et) {};
    tscaner(const tscaner& orig) = default;
    virtual ~tscaner() = default;
    virtual Lexem_info current_lexem();
private:
    enum Automaton_name{
        A_start,     A_unknown, A_idKeyword, 
        A_delimiter, A_number,  A_string
    };
    Automaton_name automaton; /* current automaton */

   typedef bool (tscaner::*Automaton_proc)();
    /* This is the type of pointer to the member function that implements the
     * automaton that processes the lexeme. This function must return true if
     * the lexeme is not yet parsed, and false otherwise. */

    typedef void (tscaner::*Final_proc)();
    /* And this is the type of the pointer to the member function that performs
     * the necessary actions in the event of an unexpected end of the lexeme. */

    static Automaton_proc procs[];
    static Final_proc     finals[];

    /* Lexeme processing functions: */
    bool start_proc();     bool unknown_proc();   
    bool idkeyword_proc(); bool delimiter_proc(); 
    bool number_proc();    bool string_proc();
    /* functions for performing actions in case of an
     * unexpected end of the token */
    void none_proc();            void unknown_final_proc();   
    void idkeyword_final_proc(); void delimiter_final_proc(); 
    void number_final_proc();    void string_final_proc();
};
#endif
