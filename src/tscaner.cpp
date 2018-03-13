#include "../include/tscaner.h"
#include "../include/get_init_state.h"
#include "../include/search_char.h"
#include "../include/belongs.h"
#include <set>
#include <string>
#include <vector>
#include "../include/operations_with_sets.h"

#include <quadmath> 
size_t digit2int(char32_t ch)
{
size_t v =  ch - U'0';
return (v<=9)? v:(v&0b1101'1111) - 7;
};
__float128 myfirstscaner::build_float()
{
__float128 num;                                                                               
num = int_value + fract_part*powq(10, num_of_digits);
num = num*powq(10, sign_of_degree*exponent);
return num;}
tscaner::Automaton_proc tscaner::procs[] = {
    &tscaner::start_proc(),     &tscaner::unknown_proc(),   
    &tscaner::idkeyword_proc(), &tscaner::delimiter_proc(), 
    &tscaner::number_proc(),    &tscaner::string_proc()
};

tscaner::Final_proc tscaner::finals[] = {
    &tscaner::none_proc(),            &tscaner::unknown_final_proc(),   
    &tscaner::idkeyword_final_proc(), &tscaner::delimiter_final_proc(), 
    &tscaner::number_final_proc(),    &tscaner::string_final_proc()
};

enum Category {
    SPACES,          DELIMITER_BEGIN, 
    STRING0,         STRING_BEGIN,    
    STRING1,         STRING2,         
    STRING10,        STRING3,         
    STRING4,         STRING5,         
    STRING6,         STRING7,         
    STRING8,         STRING9,         
    STRING11,        NUMBER0,         
    NUMBER1,         NUMBER2,         
    IDKEYWORD_BEGIN, IDKEYWORD0,      
    IDKEYWORD1,      IDKEYWORD2,      
    IDKEYWORD3,      IDKEYWORD4,      
    IDKEYWORD5,      IDKEYWORD6,      
    IDKEYWORD7,      IDKEYWORD8,      
    IDKEYWORD9,      IDKEYWORD10,     
    IDKEYWORD11,     IDKEYWORD12,     
    IDKEYWORD13,     IDKEYWORD14,     
    IDKEYWORD15,     IDKEYWORD16,     
    IDKEYWORD17,     Other
};

static const std::map<char32_t, uint64_t> categories_table = {
    {'\0', 1},           {'\X01', 1},         {'\X02', 1},         {'\X03', 1},         
    {'\X04', 1},         {'\X05', 1},         {'\X06', 1},         {'\a', 1},           
    {'\b', 1},           {'\t', 1},           {'\n', 1},           {'\v', 1},           
    {'\f', 1},           {'\r', 1},           {'\X0e', 1},         {'\X0f', 1},         
    {'\X10', 1},         {'\X11', 1},         {'\X12', 1},         {'\X13', 1},         
    {'\X14', 1},         {'\X15', 1},         {'\X16', 1},         {'\X17', 1},         
    {'\X18', 1},         {'\X19', 1},         {'\X1a', 1},         {'\X1b', 1},         
    {'\X1c', 1},         {'\X1d', 1},         {'\X1e', 1},         {'\X1f', 1},         
    {' ', 4294967297},   {'!', 2},            {", 12},             {'#', 2},            
    {'$', 24},           {'%', 2},            {'&', 2},            {', 32},             
    {'(', 2},            {')', 2},            {'*', 2},            {'+', 2},            
    {',', 2},            {'-', 65538},        {'.', 131072},       {'/', 2},            
    {'0', 11093943232},  {'1', 11098153856},  {'2', 11098153728},  {'3', 11098153728},  
    {'4', 11098153728},  {'5', 11093959424},  {'6', 11098153728},  {'7', 11093959424},  
    {'8', 11098153472},  {'9', 11093959168},  {':', 2},            {';', 2},            
    {'<', 2},            {'=', 2},            {'>', 2},            {'?', 2},            
    {'A', 11094728704},  {'B', 11094729728},  {'C', 11094728704},  {'D', 11094761472},  
    {'E', 11094761472},  {'F', 11094728704},  {'G', 11094720512},  {'H', 11094720512},  
    {'I', 11094720512},  {'J', 11094720512},  {'K', 11094720512},  {'L', 11094720512},  
    {'M', 11094720512},  {'N', 11094720512},  {'O', 11094720512},  {'P', 11094720512},  
    {'Q', 11094753280},  {'R', 11094720512},  {'S', 11094753280},  {'T', 11094720512},  
    {'U', 11094720512},  {'V', 11094720512},  {'W', 11094720512},  {'X', 11094722560},  
    {'Y', 11094720512},  {'Z', 11094720512},  {'[', 2},            {']', 2},            
    {'^', 2},            {'_', 11098914816},  {'a', 11099447296},  {'b', 11095254016},  
    {'c', 11100495872},  {'d', 11098955776},  {'e', 79818956800},  {'f', 45459185664},  
    {'g', 11098914816},  {'h', 11115692032},  {'i', 11099439104},  {'j', 11094720512},  
    {'k', 11098914816},  {'l', 11099439104},  {'m', 11099439104},  {'n', 11166023680},  
    {'o', 11082141696},  {'p', 11099439104},  {'q', 11094753280},  {'r', 28279308288},  
    {'s', 11367907328},  {'t', 9757261824},   {'u', 11099439104},  {'v', 11095244800},  
    {'w', 11095244800},  {'x', 11098916864},  {'y', 11098914816},  {'z', 11094720512},  
    {'{', 2},            {'|', 2},            {'}', 2},            {'~', 2},            
    {'Ё', 11094720512}, {'А', 11094720512}, {'Б', 11094720512}, {'В', 11094720512}, 
    {'Г', 11094720512}, {'Д', 11094720512}, {'Е', 11094720512}, {'Ж', 11094720512}, 
    {'З', 11094720512}, {'И', 11094720512}, {'Й', 11094720512}, {'К', 11094720512}, 
    {'Л', 11094720512}, {'М', 11094720512}, {'Н', 11094720512}, {'О', 11094720512}, 
    {'П', 11094720512}, {'Р', 11094720512}, {'С', 11094720512}, {'Т', 11094720512}, 
    {'У', 11094720512}, {'Ф', 11094720512}, {'Х', 11094720512}, {'Ц', 11094720512}, 
    {'Ч', 11094720512}, {'Ш', 11094720512}, {'Щ', 11094720512}, {'Ъ', 11094720512}, 
    {'Ы', 11094720512}, {'Ь', 11094720512}, {'Э', 11094720512}, {'Ю', 11094720512}, 
    {'Я', 11094720512}, {'а', 11094720512}, {'б', 11094720512}, {'в', 11094720512}, 
    {'г', 11094720512}, {'д', 11094720512}, {'е', 11094720512}, {'ж', 11094720512}, 
    {'з', 11094720512}, {'и', 11094720512}, {'й', 11094720512}, {'к', 11094720512}, 
    {'л', 11094720512}, {'м', 11094720512}, {'н', 11094720512}, {'о', 11094720512}, 
    {'п', 11094720512}, {'р', 11094720512}, {'с', 11094720512}, {'т', 11094720512}, 
    {'у', 11094720512}, {'ф', 11094720512}, {'х', 11094720512}, {'ц', 11094720512}, 
    {'ч', 11094720512}, {'ш', 11094720512}, {'щ', 11094720512}, {'ъ', 11094720512}, 
    {'ы', 11094720512}, {'ь', 11094720512}, {'э', 11094720512}, {'ю', 11094720512}, 
    {'я', 11094720512}, {'ё', 11094720512}
};


uint64_t get_categories_set(char32_t c){
    auto it = categories_table.find(c);
    if(it != categories_table.end()){
        return it->second;
    }else{
        return 1ULL << Other;
    }
}
bool tscaner::start_proc(){
    bool t = true;
    state = -1;
    /* For an automaton that processes a token, the state with the number (-1) is
     * the state in which this automaton is initialized. */
    if(belongs(SPACES, char_categories)){
        loc->current_line += U'\n' == ch;
        return t;
    }
    lexem_begin_line = loc->current_line;
    if(belongs(DELIMITER_BEGIN, char_categories)){
        (loc->pcurrent_char)--; automaton = A_delimiter;
        state = -1;
        return t;
    }

    if(belongs(STRING_BEGIN, char_categories)){
        (loc->pcurrent_char)--; automaton = A_string;
        state = 0;
        buffer.clear();
        return t;
    }

    if(belongs(STRING5, char_categories)){
        (loc->pcurrent_char)--; automaton = A_number;
        state = 0;
        int_val = 0;
          float_val = 0;
          is_float = false;
          integer_part = 0;
          fractional_part = 0;
          exponent = 1;
          exp_sign = 1;
          frac_part_num_digits = 0;
          token.code = Integer;
        return t;
    }

    if(belongs(IDKEYWORD_BEGIN, char_categories)){
        (loc->pcurrent_char)--; automaton = A_idKeyword;
        state = 0;
        return t;
    }

    automaton = A_unknown;
    return t;
}

bool tscaner::unknown_proc(){
    return belongs(Other, char_categories);
}

struct Keyword_list_elem{
    std::u32string keyword;
    codes kw_code;
};

static const Keyword_list_elem kwlist[] = {
    {U"array", array},           {U"bool", bool},             
    {U"bool16_t", bool16_t},     {U"bool32_t", bool32_t},     
    {U"bool64_t", bool64_t},     {U"bool8_t", bool8_t},       
    {U"break", break},           {U"byte", byte},             
    {U"char", char},             {U"char16_t", char16_t},     
    {U"char32_t", char32_t},     {U"char8_t", char8_t},       
    {U"const", const},           {U"const ref", const_ref},   
    {U"continue", continue},     {U"elif", elif},             
    {U"else", else},             {U"endif", endif},           
    {U"enum", enum},             {U"export", export},         
    {U"false", false},           {U"float", float},           
    {U"float128_t", float128_t}, {U"float32_t", float32_t},   
    {U"float64_t", float64_t},   {U"fn", fn},                 
    {U"for", for},               {U"if", if},                 
    {U"import", import},         {U"int", int},               
    {U"int128_t", int128_t},     {U"int16_t", int16_t},       
    {U"int32_t", int32_t},       {U"int64_t", int64_t},       
    {U"int8_t", int8_t},         {U"long", long},             
    {U"match", match},           {U"module", module},         
    {U"print", print},           {U"read", read},             
    {U"ref", ref},               {U"repeat", repeat},         
    {U"return", return},         {U"short", short},           
    {U"string", string},         {U"string16_t", string16_t}, 
    {U"string32_t", string32_t}, {U"string8_t", string8_t},   
    {U"struct", struct},         {U"then", then},             
    {U"true", true},             {U"type", type},             
    {U"uint", uint},             {U"uint128_t", uint128_t},   
    {U"uint16_t", uint16_t},     {U"uint32_t", uint32_t},     
    {U"uint64_t", uint64_t},     {U"uint8_t", uint8_t},       
    {U"until", until},           {U"val", val},               
    {U"var", var},               {U"void", void},             
    {U"while", while}
};

#define NUM_OF_KEYWORDS 63

#define THERE_IS_NO_KEYWORD (-1)

static int search_keyword(const std::u32string& finded_keyword){
    int result      = THERE_IS_NO_KEYWORD;
    int low_bound   = 0;
    int upper_bound = NUM_OF_KEYWORDS - 1;
    int middle;
    while(low_bound <= upper_bound){
        middle             = (low_bound + upper_bound) / 2;
        auto& curr_kw      = kwlist[middle].keyword;
        int compare_result = finded_keyword.compare(curr_kw);
        if(0 == compare_result){
            return middle;
        }
        if(compare_result < 0){
            upper_bound = middle - 1;
        }else{
            low_bound   = middle + 1;
        }
    }
    return result;
}

static const std::set<size_t> final_states_for_idkeywords = {
    1, 2, 3, 4, 5, 6, 7
};

bool tscaner::idkeyword_proc(){
    bool t             = true;
    bool there_is_jump = false;
    switch(state){
        case 0:
            if(belongs(IDKEYWORD0, char_categories)){
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD1, char_categories)){
                buffer += ch;
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD2, char_categories)){
                buffer += ch;
                state = 3;
                there_is_jump = true;
            }

            break;
        case 2:
            if(belongs(IDKEYWORD3, char_categories)){
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD4, char_categories)){
                buffer += ch;
                state = 2;
                there_is_jump = true;
            }

            break;
        case 3:
            if(belongs(IDKEYWORD5, char_categories)){
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD6, char_categories)){
                buffer += ch;
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(STRING8, char_categories)){
                buffer += ch;
                state = 4;
                there_is_jump = true;
            }

            break;
        case 4:
            if(belongs(IDKEYWORD7, char_categories)){
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD8, char_categories)){
                buffer += ch;
                state = 5;
                there_is_jump = true;
            }

            break;
        case 5:
            if(belongs(IDKEYWORD9, char_categories)){
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD10, char_categories)){
                buffer += ch;
                state = 6;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD11, char_categories)){
                buffer += ch;
                state = 2;
                there_is_jump = true;
            }

            break;
        case 6:
            if(belongs(IDKEYWORD12, char_categories)){
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD11, char_categories)){
                buffer += ch;
                state = 7;
                there_is_jump = true;
            }

            break;
        case 7:
            if(belongs(IDKEYWORD13, char_categories)){
                buffer += ch;
                state = 8;
                there_is_jump = true;
            }
             else if(belongs(IDKEYWORD14, char_categories)){
                state = 2;
                there_is_jump = true;
            }

            break;
        case 8:
            if(belongs(IDKEYWORD15, char_categories)){
                buffer += ch;
                state = 10;
                there_is_jump = true;
            }

            break;
        case 9:
            if(belongs(IDKEYWORD16, char_categories)){
                buffer += ch;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 10:
            if(belongs(IDKEYWORD17, char_categories)){
                buffer += ch;
                state = 9;
                there_is_jump = true;
            }

            break;
        default:
            ;
    }

    if(!there_is_jump){
        t = false;
        if(!is_elem(state, final_states_for_idkeywords)){
            printf("At line %zu unexpectedly ended identifier or keyword.", loc->current_line);
            en->increment_number_of_errors();
        }
        
        int search_result = search_keyword(buffer);
        if(search_result != THERE_IS_NO_KEYWORD) {
            token.code = kwlist[search_result].kw_code;
        }
    }

    return t;
}

static const State_for_char init_table_for_delimiters[] ={
    {7, U'!'},  {50, U'#'}, {49, U'%'}, {28, U'&'}, {20, U'('}, 
    {22, U')'}, {44, U'*'}, {41, U'+'}, {52, U','}, {42, U'-'}, 
    {47, U'/'}, {1, U':'},  {0, U';'},  {30, U'<'}, {6, U'='},  
    {33, U'>'}, {23, U'?'}, {15, U'['}, {17, U']'}, {26, U'^'}, 
    {18, U'{'}, {24, U'|'}, {19, U'}'}, {37, U'~'}
};

struct Elem {
    /** A pointer to a string of characters that can be crossed. */
    char32_t*       symbols;
    /** A lexeme code. */
    codes code;
    /** If the current character matches symbols[0], then the transition to the state
     *  first_state;
     *  if the current character matches symbols[1], then the transition to the state
     *  first_state + 1;
     *  if the current character matches symbols[2], then the transition to the state
     *  first_state + 2, and so on. */
    uint16_t        first_state;
};

static const Elem delim_jump_table[] = {
    {const_cast<char32_t*>(U""), semicolon, 0},                  
    {const_cast<char32_t*>(U"]):="), colon, 2},                  
    {const_cast<char32_t*>(U""), close_square_bracket_colon, 0}, 
    {const_cast<char32_t*>(U""), close_round_bracket_colon, 0},  
    {const_cast<char32_t*>(U""), scope_resolution, 0},           
    {const_cast<char32_t*>(U""), assignment, 0},                 
    {const_cast<char32_t*>(U""), equal, 0},                      
    {const_cast<char32_t*>(U"=|^&"), logical_not, 8},            
    {const_cast<char32_t*>(U""), not_equal, 0},                  
    {const_cast<char32_t*>(U"|"), Unknown, 10},                  
    {const_cast<char32_t*>(U"^"), Unknown, 11},                  
    {const_cast<char32_t*>(U"&"), Unknown, 12},                  
    {const_cast<char32_t*>(U""), not_logical_or, 0},             
    {const_cast<char32_t*>(U""), not_logical_xor, 0},            
    {const_cast<char32_t*>(U""), not_logical_and, 0},            
    {const_cast<char32_t*>(U":"), open_square_bracket, 16},      
    {const_cast<char32_t*>(U""), open_square_bracket_colon, 0},  
    {const_cast<char32_t*>(U""), close_square_bracket, 0},       
    {const_cast<char32_t*>(U""), open_figure_bracket, 0},        
    {const_cast<char32_t*>(U""), close_figure_bracket, 0},       
    {const_cast<char32_t*>(U":"), open_round_bracket, 21},       
    {const_cast<char32_t*>(U""), open_round_bracket_colon, 0},   
    {const_cast<char32_t*>(U""), close_round_bracket, 0},        
    {const_cast<char32_t*>(U""), question_mark, 0},              
    {const_cast<char32_t*>(U"|"), or, 25},                       
    {const_cast<char32_t*>(U""), logical_or, 0},                 
    {const_cast<char32_t*>(U"^"), xor, 27},                      
    {const_cast<char32_t*>(U""), logical_xor, 0},                
    {const_cast<char32_t*>(U"&"), and, 29},                      
    {const_cast<char32_t*>(U""), logical_and, 0},                
    {const_cast<char32_t*>(U"=<"), less, 31},                    
    {const_cast<char32_t*>(U""), less_or_equal, 0},              
    {const_cast<char32_t*>(U""), lshift, 0},                     
    {const_cast<char32_t*>(U"=>"), greater, 34},                 
    {const_cast<char32_t*>(U""), greater_or_equal, 0},           
    {const_cast<char32_t*>(U">"), rshift, 36},                   
    {const_cast<char32_t*>(U""), rshift_int, 0},                 
    {const_cast<char32_t*>(U"|^&"), not, 38},                    
    {const_cast<char32_t*>(U""), not_or, 0},                     
    {const_cast<char32_t*>(U""), not_xor, 0},                    
    {const_cast<char32_t*>(U""), not_and, 0},                    
    {const_cast<char32_t*>(U""), plus, 0},                       
    {const_cast<char32_t*>(U">"), minus, 43},                    
    {const_cast<char32_t*>(U""), structure_dereference, 0},      
    {const_cast<char32_t*>(U"*"), multiplication, 45},           
    {const_cast<char32_t*>(U"."), exponentiation, 46},           
    {const_cast<char32_t*>(U""), fract_exponentiation, 0},       
    {const_cast<char32_t*>(U"."), division, 48},                 
    {const_cast<char32_t*>(U""), fract_division, 0},             
    {const_cast<char32_t*>(U""), mod, 0},                        
    {const_cast<char32_t*>(U"#"), sizeof, 51},                   
    {const_cast<char32_t*>(U""), index, 0},                      
    {const_cast<char32_t*>(U""), comma, 0}
};

bool tscaner::delimiter_proc(){
    bool t = false;
    if(-1 == state){
        state = get_init_state(ch, init_table_for_delimiters,
                               sizeof(init_table_for_delimiters)/sizeof(State_for_char));
        token.code = delim_jump_table[state].code;
        t = true;
        return t;
    }
    Elem elem = delim_jump_table[state];
    token.code = delim_jump_table[state].code;
    int y = search_char(ch, elem.symbols);
    if(y != THERE_IS_NO_CHAR){
        state = elem.first_state + y; t = true;
    }
    return t;
}

static const std::set<size_t> final_states_for_numbers = {
    1, 2,  3, 4, 5, 6, 7, 8, 
    9, 10
};

bool tscaner::number_proc(){
    bool t             = true;
    bool there_is_jump = false;
    switch(state){
        case 0:
            if(belongs(STRING10, char_categories)){
                integer_part = integer_part * 10 + digit2int(ch);
                state = 10;
                there_is_jump = true;
            }
             else if(belongs(STRING11, char_categories)){
                integer_part = integer_part * 10 + digit2int(ch);
                state = 9;
                there_is_jump = true;
            }

            break;
        case 1:
            if(belongs(NUMBER0, char_categories)){
                precision = ch; is_float = true;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 2:
            if(belongs(NUMBER1, char_categories)){
                exp_sign = setexp(ch);
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                fractional_part = fractional_part / 10 + digit2int(ch); frac_part_num_digits += 1;
                state = 8;
                there_is_jump = true;
            }
             else if(belongs(NUMBER0, char_categories)){
                precision = ch; is_float = true;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 3:
            if(belongs(STRING2, char_categories)){
                state = 2;
                there_is_jump = true;
            }
             else if(belongs(NUMBER1, char_categories)){
                exp_sign = setexp(ch);
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                fractional_part = fractional_part / 10 + digit2int(ch); frac_part_num_digits += 1;
                state = 8;
                there_is_jump = true;
            }
             else if(belongs(NUMBER0, char_categories)){
                precision = ch; is_float = true;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 4:
            if(belongs(STRING2, char_categories)){
                state = 11;
                there_is_jump = true;
            }
             else if(belongs(STRING3, char_categories)){
                integer_part = (integer_part << 1) + digit2int(ch);
                state = 4;
                there_is_jump = true;
            }

            break;
        case 5:
            if(belongs(STRING2, char_categories)){
                state = 12;
                there_is_jump = true;
            }
             else if(belongs(STRING4, char_categories)){
                integer_part = (integer_part << 3) + digit2int(ch);
                state = 5;
                there_is_jump = true;
            }

            break;
        case 6:
            if(belongs(STRING2, char_categories)){
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                exponent = exponent * 10 + digit2int(ch);
                state = 6;
                there_is_jump = true;
            }

            break;
        case 7:
            if(belongs(STRING2, char_categories)){
                state = 16;
                there_is_jump = true;
            }
             else if(belongs(STRING9, char_categories)){
                integer_part = (integer_part << 4) + digit2int(ch);
                state = 7;
                there_is_jump = true;
            }

            break;
        case 8:
            if(belongs(STRING2, char_categories)){
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(NUMBER1, char_categories)){
                exp_sign = setexp(ch);
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                fractional_part = fractional_part / 10 + digit2int(ch); frac_part_num_digits += 1;
                state = 8;
                there_is_jump = true;
            }
             else if(belongs(NUMBER0, char_categories)){
                precision = ch; is_float = true;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 9:
            if(belongs(STRING2, char_categories)){
                state = 15;
                there_is_jump = true;
            }
             else if(belongs(NUMBER1, char_categories)){
                exp_sign = setexp(ch);
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(NUMBER2, char_categories)){
                is_float = true;
                state = 13;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                integer_part = integer_part * 10 + digit2int(ch);
                state = 9;
                there_is_jump = true;
            }
             else if(belongs(NUMBER0, char_categories)){
                precision = ch; is_float = true;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 10:
            if(belongs(STRING2, char_categories)){
                state = 15;
                there_is_jump = true;
            }
             else if(belongs(NUMBER1, char_categories)){
                exp_sign = setexp(ch);
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(NUMBER2, char_categories)){
                is_float = true;
                state = 13;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                integer_part = integer_part * 10 + digit2int(ch);
                state = 9;
                there_is_jump = true;
            }
             else if(belongs(STRING6, char_categories)){
                state = 11;
                there_is_jump = true;
            }
             else if(belongs(NUMBER0, char_categories)){
                precision = ch; is_float = true;
                state = 1;
                there_is_jump = true;
            }
             else if(belongs(STRING7, char_categories)){
                state = 16;
                there_is_jump = true;
            }
             else if(belongs(STRING8, char_categories)){
                state = 12;
                there_is_jump = true;
            }

            break;
        case 11:
            if(belongs(STRING3, char_categories)){
                integer_part = (integer_part << 1) + digit2int(ch);
                state = 4;
                there_is_jump = true;
            }

            break;
        case 12:
            if(belongs(STRING4, char_categories)){
                integer_part = (integer_part << 3) + digit2int(ch);
                state = 5;
                there_is_jump = true;
            }

            break;
        case 13:
            if(belongs(STRING5, char_categories)){
                fractional_part = fractional_part / 10 + digit2int(ch); frac_part_num_digits += 1;
                state = 3;
                there_is_jump = true;
            }

            break;
        case 14:
            if(belongs(STRING5, char_categories)){
                exponent = exponent * 10 + digit2int(ch);
                state = 6;
                there_is_jump = true;
            }

            break;
        case 15:
            if(belongs(STRING5, char_categories)){
                integer_part = integer_part * 10 + digit2int(ch);
                state = 9;
                there_is_jump = true;
            }

            break;
        case 16:
            if(belongs(STRING9, char_categories)){
                integer_part = (integer_part << 4) + digit2int(ch);
                state = 7;
                there_is_jump = true;
            }

            break;
        default:
            ;
    }

    if(!there_is_jump){
        t = false;
        if(!is_elem(state, final_states_for_numbers)){
            printf("At line %zu unexpectedly ended the number.", loc->current_line);
            en->increment_number_of_errors();
        }
        
          if(is_float){
            token.float_val=build_float();
            token.code = precision2code(precision);
          } else {
            token.int_val=integer_part;
            token.code = Integer;
          }
    }

    return t;
}

static const std::set<size_t> final_states_for_strings = {
    0, 1, 2,  3, 4, 5, 6, 7, 
    8, 9, 10
};

bool tscaner::string_proc(){
    bool t             = true;
    bool there_is_jump = false;
    switch(state){
        case 0:
            if(belongs(STRING0, char_categories)){
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING1, char_categories)){
                state = 16;
                there_is_jump = true;
            }

            break;
        case 1:
            if(belongs(STRING1, char_categories)){
                state = 16;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 11;
                there_is_jump = true;
            }
             else if(belongs(STRING3, char_categories)){
                buffer += char_code;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 2:
            if(belongs(STRING1, char_categories)){
                state = 16;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 13;
                there_is_jump = true;
            }
             else if(belongs(STRING4, char_categories)){
                buffer += char_code;
                state = 2;
                there_is_jump = true;
            }

            break;
        case 3:
            if(belongs(STRING1, char_categories)){
                state = 16;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 15;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                buffer += char_code;
                state = 3;
                there_is_jump = true;
            }

            break;
        case 4:
            if(belongs(STRING1, char_categories)){
                state = 16;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 15;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                buffer += char_code;
                state = 3;
                there_is_jump = true;
            }
             else if(belongs(STRING6, char_categories)){
                buffer += char_code;
                state = 11;
                there_is_jump = true;
            }
             else if(belongs(STRING7, char_categories)){
                buffer += char_code;
                state = 19;
                there_is_jump = true;
            }
             else if(belongs(STRING8, char_categories)){
                buffer += char_code;
                state = 13;
                there_is_jump = true;
            }

            break;
        case 5:
            if(belongs(STRING1, char_categories)){
                state = 16;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 19;
                there_is_jump = true;
            }
             else if(belongs(STRING9, char_categories)){
                buffer += char_code;
                state = 5;
                there_is_jump = true;
            }

            break;
        case 6:
            if(belongs(STRING0, char_categories)){
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING1, char_categories)){
                state = 18;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 12;
                there_is_jump = true;
            }
             else if(belongs(STRING3, char_categories)){
                buffer += char_code;
                state = 6;
                there_is_jump = true;
            }

            break;
        case 7:
            if(belongs(STRING0, char_categories)){
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING1, char_categories)){
                state = 18;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 14;
                there_is_jump = true;
            }
             else if(belongs(STRING4, char_categories)){
                buffer += char_code;
                state = 7;
                there_is_jump = true;
            }

            break;
        case 8:
            if(belongs(STRING0, char_categories)){
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING1, char_categories)){
                state = 18;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 17;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                buffer += char_code;
                state = 8;
                there_is_jump = true;
            }

            break;
        case 9:
            if(belongs(STRING0, char_categories)){
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING1, char_categories)){
                state = 18;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 17;
                there_is_jump = true;
            }
             else if(belongs(STRING5, char_categories)){
                buffer += char_code;
                state = 8;
                there_is_jump = true;
            }
             else if(belongs(STRING6, char_categories)){
                buffer += char_code;
                state = 12;
                there_is_jump = true;
            }
             else if(belongs(STRING7, char_categories)){
                buffer += char_code;
                state = 20;
                there_is_jump = true;
            }
             else if(belongs(STRING8, char_categories)){
                buffer += char_code;
                state = 14;
                there_is_jump = true;
            }

            break;
        case 10:
            if(belongs(STRING0, char_categories)){
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING1, char_categories)){
                state = 18;
                there_is_jump = true;
            }
             else if(belongs(STRING2, char_categories)){
                buffer += char_code;
                state = 20;
                there_is_jump = true;
            }
             else if(belongs(STRING9, char_categories)){
                buffer += char_code;
                state = 10;
                there_is_jump = true;
            }

            break;
        case 11:
            if(belongs(STRING3, char_categories)){
                buffer += char_code;
                state = 1;
                there_is_jump = true;
            }

            break;
        case 12:
            if(belongs(STRING3, char_categories)){
                buffer += char_code;
                state = 6;
                there_is_jump = true;
            }

            break;
        case 13:
            if(belongs(STRING4, char_categories)){
                buffer += char_code;
                state = 2;
                there_is_jump = true;
            }

            break;
        case 14:
            if(belongs(STRING4, char_categories)){
                buffer += char_code;
                state = 7;
                there_is_jump = true;
            }

            break;
        case 15:
            if(belongs(STRING5, char_categories)){
                buffer += char_code;
                state = 3;
                there_is_jump = true;
            }

            break;
        case 16:
            if(belongs(STRING10, char_categories)){
                buffer += char_code;
                state = 4;
                there_is_jump = true;
            }
             else if(belongs(STRING11, char_categories)){
                buffer += char_code;
                state = 3;
                there_is_jump = true;
            }

            break;
        case 17:
            if(belongs(STRING5, char_categories)){
                buffer += char_code;
                state = 8;
                there_is_jump = true;
            }

            break;
        case 18:
            if(belongs(STRING10, char_categories)){
                buffer += char_code;
                state = 9;
                there_is_jump = true;
            }
             else if(belongs(STRING11, char_categories)){
                buffer += char_code;
                state = 8;
                there_is_jump = true;
            }

            break;
        case 19:
            if(belongs(STRING9, char_categories)){
                buffer += char_code;
                state = 5;
                there_is_jump = true;
            }

            break;
        case 20:
            if(belongs(STRING9, char_categories)){
                buffer += char_code;
                state = 10;
                there_is_jump = true;
            }

            break;
        case 21:
            if(ch != U'\"'){
                buffer += ch;
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING0, char_categories)){
                state = 22;
                there_is_jump = true;
            }

            break;
        case 22:
            if(belongs(STRING0, char_categories)){
                buffer += ch;
                state = 21;
                there_is_jump = true;
            }
             else if(belongs(STRING1, char_categories)){
                state = 18;
                there_is_jump = true;
            }

            break;
        default:
            ;
    }

    if(!there_is_jump){
        t = false;
        if(!is_elem(state, final_states_for_strings)){
            printf("At line %zu unexpectedly ended a string literal.", loc->current_line);
            en->increment_number_of_errors();
        }
        token.code=(buffer.length()==1)?Char:String;
            token.string_index = strs -> insert(buffer);
    }

    return t;
}

void tscaner::none_proc(){
    /* This subroutine will be called if, after reading the input text, it turned
     * out to be in the A_start automaton. Then you do not need to do anything. */
}

void tscaner::unknown_final_proc(){
    /* This subroutine will be called if, after reading the input text, it turned
     * out to be in the A_unknown automaton. Then you do not need to do anything. */
}

void tscaner::idkeyword_final_proc(){
    if(!is_elem(state, final_states_for_idkeywords)){
        printf("At line %zu unexpectedly ended identifier or keyword.", loc->current_line);
        en->increment_number_of_errors();
    }

    int search_result = search_keyword(buffer);
    if(search_result != THERE_IS_NO_KEYWORD) {
        token.code = kwlist[search_result].kw_code;
    }

}

void tscaner::delimiter_final_proc(){
        
    token.code = delim_jump_table[state].code;
    
}

void tscaner::number_final_proc(){
    if(!is_elem(state, final_states_for_numbers)){
        printf("At line %zu unexpectedly ended the number.", loc->current_line);
        en->increment_number_of_errors();
    }
    
          if(is_float){
            token.float_val=build_float();
            token.code = precision2code(precision);
          } else {
            token.int_val=integer_part;
            token.code = Integer;
          }
}

void tscaner::string_final_proc(){
    if(!is_elem(state, final_states_for_strings)){
        printf("At line %zu unexpectedly ended a string literal.", loc->current_line);
        en->increment_number_of_errors();
    }
    token.code=(buffer.length()==1)?Char:String;
    token.string_index = strs -> insert(buffer);
}

Lexem_info tscaner::current_lexem(){
    automaton = A_start; token.code = None;
    lexem_begin = loc->pcurrent_char;
    bool t = true;
    while((ch = *(loc->pcurrent_char)++)){
        char_categories = get_categories_set(ch); //categories_table[ch];
        t = (this->*procs[automaton])();
        if(!t){
            /* We get here only if the lexeme has already been read. At the same time,
             * the current automaton reads the character immediately after the end of
             * the token read, based on this symbol, it is decided that the token has
             * been read and the transition to the next character has been made.
             * Therefore, in order to not miss the first character of the next lexeme,
             * we need to decrease the pcurrent_char pointer by one. */
            (loc->pcurrent_char)--;
            return token;
        }
    }
    /* Here we can be, only if we have already read all the processed text. In this
     * case, the pointer to the current symbol indicates a byte, which is immediately
     * after the zero character, which is a sign of the end of the text. To avoid
     * entering subsequent calls outside the text, we need to go back to the null
     * character. */
    (loc->pcurrent_char)--;
    /* Further, since we are here, the end of the current token (perhaps unexpected)
     * has not yet been processed. It is necessary to perform this processing, and,
     * probably, to display any diagnostics. */
    (this->*finals[automaton])();
    return token;
}


