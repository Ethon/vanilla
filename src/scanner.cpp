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

// C++ Standard Library:
#include <algorithm>

// Vanilla:
#include <vanilla/scanner.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// UTILITY
///////////////////////////////////////////////////////////////////////////

unsigned const SCANNER_ERROR = -1;
unsigned const SCANNER_NOMATCH = 0;
unsigned const SCANNER_SUCCESS = 1;

namespace
{
    bool isoctdigit(char c)
    {
        return c >= '0' && c < '8';
    }
    
    bool isbindigit(char c)
    {
        return c == '0' || c == '1';
    }
}
    
#define TTYPE_ENUM_CASE(x) case x: return o << #x;
    
std::ostream& vanilla::operator<<(std::ostream& o, ttype tt)
{
    switch(tt)
    {
        TTYPE_ENUM_CASE(ttype::nil);
        TTYPE_ENUM_CASE(ttype::eof);
        
        TTYPE_ENUM_CASE(ttype::ident);
        TTYPE_ENUM_CASE(ttype::int_lit);
        TTYPE_ENUM_CASE(ttype::real_lit);
        TTYPE_ENUM_CASE(ttype::string_lit);
        
        TTYPE_ENUM_CASE(ttype::truelit);
        TTYPE_ENUM_CASE(ttype::falselit);
        TTYPE_ENUM_CASE(ttype::indeterminate);
        
        TTYPE_ENUM_CASE(ttype::element_selection);
        TTYPE_ENUM_CASE(ttype::range_op);
        TTYPE_ENUM_CASE(ttype::varargs);
        
        TTYPE_ENUM_CASE(ttype::plus);
        TTYPE_ENUM_CASE(ttype::minus);
        TTYPE_ENUM_CASE(ttype::not_);
        TTYPE_ENUM_CASE(ttype::mul);
        TTYPE_ENUM_CASE(ttype::div);
        TTYPE_ENUM_CASE(ttype::concat);
        TTYPE_ENUM_CASE(ttype::inc);
        TTYPE_ENUM_CASE(ttype::dec);
        TTYPE_ENUM_CASE(ttype::pow);
        
        TTYPE_ENUM_CASE(ttype::less);
        TTYPE_ENUM_CASE(ttype::less_equal);
        TTYPE_ENUM_CASE(ttype::greater);
        TTYPE_ENUM_CASE(ttype::greater_equal);
        TTYPE_ENUM_CASE(ttype::equal);
        TTYPE_ENUM_CASE(ttype::not_equal);
        
        TTYPE_ENUM_CASE(ttype::assign);
        
        TTYPE_ENUM_CASE(ttype::questionmark);
        TTYPE_ENUM_CASE(ttype::colon);        
        
        TTYPE_ENUM_CASE(ttype::lparen);
        TTYPE_ENUM_CASE(ttype::rparen);
        TTYPE_ENUM_CASE(ttype::lbrace);
        TTYPE_ENUM_CASE(ttype::rbrace);
        TTYPE_ENUM_CASE(ttype::lbrack);
        TTYPE_ENUM_CASE(ttype::rbrack);
        TTYPE_ENUM_CASE(ttype::comma);
        TTYPE_ENUM_CASE(ttype::endstmnt);
        
        TTYPE_ENUM_CASE(ttype::function);
        TTYPE_ENUM_CASE(ttype::lambda);
        TTYPE_ENUM_CASE(ttype::ret);
        TTYPE_ENUM_CASE(ttype::if_);
        TTYPE_ENUM_CASE(ttype::else_);
        TTYPE_ENUM_CASE(ttype::elseif);
        TTYPE_ENUM_CASE(ttype::for_);
        TTYPE_ENUM_CASE(ttype::while_);
        
        TTYPE_ENUM_CASE(ttype::native);
        TTYPE_ENUM_CASE(ttype::from);
        TTYPE_ENUM_CASE(ttype::declared);
    }
    
    assert(false);
}

#undef TTYPE_ENUM_CASE

std::ostream& vanilla::operator<<(std::ostream& o, token const& t)
{
    return o << "{ Pos:(" << t.line << ", " << t.pos << ") Type:" << t.type << " Lexeme:'"
        << t.lexeme << "' Flags:" << t.flags << " }";
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::scanner
///////////////////////////////////////////////////////////////////////////

char vanilla::scanner::next()
{
    if(_cur == _data.end() || ++_cur == _data.end())
        return 0;
    
    if(*_cur == '\n')
    {
        _pos = 0;
        ++_line;
    }
    else
    {
        ++_pos;
    }
    
    return *_cur;
}

void vanilla::scanner::skip_spaces()
{
    while(std::isspace(*_cur))
        next();
}

#define TRY_READ(x, t) do { int r = x(t); if(r != SCANNER_NOMATCH) return r; } while(0)
int vanilla::scanner::read_ident_or_keyword(token& t)
{
   if(!std::isalpha(*_cur) && *_cur != '_')
        return SCANNER_NOMATCH;
   
    char const* begin = _cur;
    while(std::isalnum(*_cur) || *_cur == '_')
        next();
    char const* end = _cur;
   
    t.lexeme = cstr_range(begin, end);
    t.flags = 0;
    if(t.lexeme.compare("function") == 0)
        t.type = ttype::function;
    else if(t.lexeme.compare("lambda") == 0)
        t.type = ttype::lambda;
    else if(t.lexeme.compare("return") == 0)
        t.type = ttype::ret;
    else if(t.lexeme.compare("true") == 0)
        t.type = ttype::truelit;
    else if(t.lexeme.compare("false") == 0)
        t.type = ttype::falselit;
    else if(t.lexeme.compare("indeterminate") == 0)
        t.type = ttype::indeterminate;
    else if(t.lexeme.compare("if") == 0)
        t.type = ttype::if_;
    else if(t.lexeme.compare("else") == 0)
        t.type = ttype::else_;
    else if(t.lexeme.compare("elseif") == 0)
        t.type = ttype::elseif;
    else if(t.lexeme.compare("for") == 0)
        t.type = ttype::for_;
    else if(t.lexeme.compare("while") == 0)
        t.type = ttype::while_;
    else if(t.lexeme.compare("native") == 0)
        t.type = ttype::native;
    else if(t.lexeme.compare("from") == 0)
        t.type = ttype::from;
    else if(t.lexeme.compare("declared") == 0)
        t.type = ttype::declared;
    else
        t.type = ttype::ident;
    
    return SCANNER_SUCCESS;  
}
        
int vanilla::scanner::read_number_lit(token& t)
{
    if(*_cur == '0')
    {
        next();
        
        // Base 16.
        if(*_cur == 'x' || *_cur == 'X')
        {
            next();
            char const* begin = _cur;
            while(std::isxdigit(*_cur))
                next();
            char const* end = _cur;
            
            if(begin == end)
                return SCANNER_ERROR;
            
            t.type = ttype::int_lit;
            t.lexeme = cstr_range(begin, end);
            t.flags = token_flags::INT_FLAG_BASE16;
            return SCANNER_SUCCESS;
        }
        
        // Base8.
        else if(isoctdigit(*_cur))
        {
            char const* begin = _cur;
            do
            {
                next();
            } while(isoctdigit(*_cur));
            char const* end = _cur;
            
            t.type = ttype::int_lit;
            t.lexeme = cstr_range(begin, end);
            t.flags = token_flags::INT_FLAG_BASE8;
            return SCANNER_SUCCESS;
        }
        
        // Base2.
        else if(*_cur == 'b' || *_cur == 'B')
        {
            next();
            char const* begin = _cur;
            while(isbindigit(*_cur))
                next();
            char const* end = _cur;
            
            if(begin == end)
                return SCANNER_ERROR;
            
            t.type = ttype::int_lit;
            t.lexeme = cstr_range(begin, end);
            t.flags = token_flags::INT_FLAG_BASE2;
            return SCANNER_SUCCESS;
        }
        
        // A real of the format 0.*****
        else if(*_cur == '.')
        {
            char const* begin = _cur - 1;
            next();
            while(std::isdigit(*_cur))
                next();
            char const* end = _cur;
            
            t.type = ttype::real_lit;
            t.lexeme = cstr_range(begin, end);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        // The 0 literal.
        else
        {
            t.type  = ttype::int_lit;
            t.lexeme = cstr_range(_cur - 1, _cur);
            t.flags = token_flags::INT_FLAG_BASE10;
            return SCANNER_SUCCESS;
        }
    }
    
    // Base 10.
    else
    {
        char const* begin = _cur;
        while(std::isdigit(*_cur))
            next();
        char const* end = _cur;
        
        if(begin == end)
            return SCANNER_NOMATCH;
        
        if(*_cur == '.')
        {
            ++_cur;
            while(std::isdigit(*_cur))
                next();
            end = _cur;
            
            t.type = ttype::real_lit;
            t.lexeme = cstr_range(begin, end);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        t.type = ttype::int_lit;
        t.lexeme = cstr_range(begin, end);
        t.flags = token_flags::INT_FLAG_BASE10;
        return SCANNER_SUCCESS;
    }
}

int vanilla::scanner::read_string_lit(token& t)
{
    if(*_cur != '"')
        return SCANNER_NOMATCH;
    
    char const* begin = _cur;
    char const* end = std::find(_cur + 1, _data.end(), '"');
    while(end != _data.end() && (end[-1] == '\\' && end[-2] != '\\'))
        end = std::find(end + 1, _data.end(), '"'); // Skip escaped 
        
    if(end == _data.end())
        return SCANNER_ERROR;
    
    while(_cur != end)
        next();
    next();
    
    t.type = ttype::string_lit;
    t.lexeme = cstr_range(begin + 1, end);
    t.flags = 0;
    return SCANNER_SUCCESS;
}

int vanilla::scanner::read_lit(token& t)
{
    TRY_READ(read_number_lit, t);
    TRY_READ(read_string_lit, t);
    return SCANNER_NOMATCH;
}
        
int vanilla::scanner::read_operator(token& t)
{
    switch(*_cur)
    {
        case '.':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '.')
            {
                next();
                if(*_cur == '.')
                {
                    next();
                    t.type = ttype::varargs;
                    t.lexeme = cstr_range(begin, _cur);
                    t.flags = 0;
                    return SCANNER_SUCCESS;
                }
                
                t.type = ttype::range_op;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::element_selection;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        case '+':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '+')
            {
                next();
                t.type = ttype::inc;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::plus;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        case '-':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '-')
            {
                next();
                t.type = ttype::dec;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::minus;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        case '/':
        {
            t.type = ttype::div;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case '~':
        {
            t.type = ttype::concat;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case '*':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '*')
            {
                next();
                t.type = ttype::pow;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::mul;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        case '<':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '=')
            {
                next();
                t.type = ttype::less_equal;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::less;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        case '>':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '=')
            {
                next();
                t.type = ttype::greater_equal;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::greater;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        case '=':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '=')
            {
                next();
                t.type = ttype::equal;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::assign;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        case '!':
        {
            char const* begin = _cur;
            next();
            if(*_cur == '=')
            {
                next();
                t.type = ttype::not_equal;
                t.lexeme = cstr_range(begin, _cur);
                t.flags = 0;
                return SCANNER_SUCCESS;
            }
            
            t.type = ttype::not_;
            t.lexeme = cstr_range(begin, _cur);
            t.flags = 0;
            return SCANNER_SUCCESS;
        }
        
        default:
            return SCANNER_NOMATCH;
    }
}

int vanilla::scanner::read_special(token& t)
{
    switch(*_cur)
    {
        case '(':
        {
            t.type = ttype::lparen;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case ')':
        {
            t.type = ttype::rparen;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case '{':
        {
            t.type = ttype::lbrace;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case '}':
        {
            t.type = ttype::rbrace;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case '[':
        {
            t.type = ttype::lbrack;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case ']':
        {
            t.type = ttype::rbrack;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case ',':
        {
            t.type = ttype::comma;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case ';':
        {
            t.type = ttype::endstmnt;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case '?':
        {
            t.type = ttype::questionmark;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        case ':':
        {
            t.type = ttype::colon;
            t.lexeme = cstr_range(_cur, _cur + 1);
            t.flags = 0;
            next();
            return SCANNER_SUCCESS;
        }
        
        default:
            return SCANNER_NOMATCH;
    }
}

int vanilla::scanner::read_token(token& t)
{
    TRY_READ(read_ident_or_keyword, t);
    TRY_READ(read_lit, t);
    TRY_READ(read_operator, t);
    TRY_READ(read_special, t);
    return SCANNER_NOMATCH;
}

vanilla::scanner::scanner(cstr_range const& data)
    : _data(data), _cur(_data.begin()), _line(1), _pos(1)
{ }

vanilla::token vanilla::scanner::get_token()
{
    skip_spaces();
    if(!*_cur)
        return token(ttype::eof, line(), pos());
    
    token out(ttype::nil, line(), pos());
    int r = read_token(out);
    if(r == SCANNER_SUCCESS)
        return out;
    if(r == SCANNER_NOMATCH)
        BOOST_THROW_EXCEPTION(error::invalid_token_error()
            << error::line_info(line()) << error::pos_info(pos()));
        
    assert(false);
}

unsigned vanilla::scanner::line() const
{
    return _line;
}

unsigned vanilla::scanner::pos() const
{
    return _pos;
}