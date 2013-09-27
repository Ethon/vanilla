//  Copyright (c) <2013> <Florian Erler>
//
//  This software is provided 'as-is', without any express or implied
//  warranty. In no event will the authors be held liable for any damages
//  arising from the use of this software.
//
//  Permission is granted to anyone to use this software for any purpose,
//  including commercial applications, and to alter it and redistribute it
//  freely, subject to the following restrictions:
//
//      1. The origin of this software must not be misrepresented; you must not
//      claim that you wrote the original software. If you use this software
//      in a product, an acknowledgment in the product documentation would be
//      appreciated but is not required.
//
//      2. Altered source versions must be plainly marked as such, and must not be
//      misrepresented as being the original software.
//
//      3. This notice may not be removed or altered from any source
//      distribution.

#ifndef HEADER_UUID_EC0497FD72624F468F78E24A7B75DACE
#define HEADER_UUID_EC0497FD72624F468F78E24A7B75DACE

// C++ Standard Library:
#include <stdexcept>

// Vanilla:
#include <vanilla/str_range.hpp>
#include <vanilla/error.hpp>

namespace vanilla
{
    enum class ttype
    {
        nil,
        eof,
        
        ident,
        int_lit,
        real_lit,
        string_lit,
        
        truelit,
        falselit,
        indeterminate,
        
        element_selection,
        range_op,
        varargs,
        
        plus,
        minus,
        not_,
        mul,
        div,
        concat,
        inc,
        dec,
        pow,
        
        less,
        less_equal,
        greater,
        greater_equal,
        equal,
        not_equal,
        
        assign,
        
        questionmark,
        colon,
        
        lparen,
        rparen,
        lbrace,
        rbrace,
        lbrack,
        rbrack,
        comma,
        endstmnt,
        
        function,
        lambda,
        ret,
        if_,
        else_,
        elseif,
        for_,
        while_,
        native,
        from,
        declared
    };
    std::ostream& operator<<(std::ostream& o, ttype tt);
    
    namespace token_flags
    {
        unsigned const INT_FLAG_BASE10 = 1;
        unsigned const INT_FLAG_BASE16 = 2;
        unsigned const INT_FLAG_BASE8 = 4;
        unsigned const INT_FLAG_BASE2 = 8;
    }
    
    struct token
    {
        token(ttype type = ttype::nil, unsigned line = 0, unsigned pos = 0,
            cstr_range lexeme = cstr_range(), unsigned flags = 0)
            : type(type), lexeme(lexeme), line(line), pos(pos), flags(flags)
        { }
        
        ttype type;
        cstr_range lexeme;
        unsigned line, pos;
        unsigned flags;
    };
    std::ostream& operator<<(std::ostream& o, token const& t);
    
    namespace error
    {
        struct invalid_token_error : scanner_error
        { };
    }
    
    class scanner
    {
    private:
        cstr_range _data;
        char const* _cur;
        unsigned _line, _pos;
        
        char next();
        void skip_spaces();
        
        int read_ident_or_keyword(token&);
        int read_number_lit(token&);
        int read_string_lit(token&);
        int read_lit(token&);
        int read_operator(token&);
        
        int read_special(token&);
        
        int read_token(token&);
        
    public:
        scanner(cstr_range const& data);
        
        token get_token();
        unsigned line() const;
        unsigned pos() const;
    };
    
}

#endif // HEADER_UUID_EC0497FD72624F468F78E24A7B75DACE