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

#ifndef HEADER_UUID_67CAAE82DE464A2F8E679107C7725185
#define HEADER_UUID_67CAAE82DE464A2F8E679107C7725185

// C++ Standard Library:
#include <exception>

// Vanilla:
#include <vanilla/expression_ast.hpp>
#include <vanilla/statement_ast.hpp>
#include <vanilla/scanner.hpp>
#include <vanilla/error.hpp>

namespace vanilla
{
    namespace error
    {
        struct unexpected_token_error : parser_error
        { };
        
        struct expected_primary_expression_error : parser_error
        { };
        
        struct invalid_escape_sequence : parser_error
        { };
        
        VANILLA_MAKE_ERRINFO(ttype, expected_type);
        VANILLA_MAKE_ERRINFO(ttype, received_type);
        VANILLA_MAKE_ERRINFO(std::string, escape_sequence);
    }
    
    expression_node::ptr parse_expr(char const* expr);
    
    statement_node::ptr parse_string(char const* str);
    
    statement_node::ptr parse_file(char const* filename);
}

#endif // HEADER_UUID_67CAAE82DE464A2F8E679107C7725185