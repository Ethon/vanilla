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
#include <vector>
#include <string>
#include <fstream>
#include <iterator>

// Boost:
#include <boost/optional.hpp>

// Vanilla:
#include <vanilla/parsing.hpp>
#include <vanilla/scanner.hpp>
#include <vanilla/statement_ast.hpp>
#include <vanilla/float_object.hpp>

namespace
{    
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
    
    class token_buffer
    {
    private:
        std::vector<vanilla::token> _buffer;
        std::size_t _cur;
        
    public:
        token_buffer(vanilla::scanner& scan)
            : _buffer(), _cur(0)
        {
            for(vanilla::token token = scan.get_token(); ; token = scan.get_token())
            {
                _buffer.push_back(token);
                if(token.type == vanilla::ttype::eof)
                    break;
            }
        }
        
        bool exhausted()
        {
            return _cur == _buffer.size();
        }
        
        vanilla::token* cur()
        {
            if(exhausted())
                return nullptr;
            
            return &_buffer[_cur];
        }
        
        vanilla::token* accept(vanilla::ttype t)
        {
            assert(!exhausted());
            
            if(_buffer[_cur].type != t)
                return nullptr;
            
            vanilla::token* result = &_buffer[_cur];
            ++_cur;
            return result;
        }
        
        vanilla::token* expect(vanilla::ttype type)
        {
            assert(!exhausted());
                
            vanilla::token* t = &_buffer[_cur];
            if(t->type != type)
            {
                BOOST_THROW_EXCEPTION(vanilla::error::unexpected_token_error()
                    << vanilla::error::line_info(t->line) << vanilla::error::pos_info(t->pos)
                    << vanilla::error::expected_type(type) << vanilla::error::received_type(t->type)
                );
            }
            
            ++_cur;
            return t;
        }
        
        void require(vanilla::ttype type)
        {
            assert(!exhausted());
                
            vanilla::token& t = _buffer[_cur];
            if(t.type != type)
            {
                BOOST_THROW_EXCEPTION(vanilla::error::unexpected_token_error()
                    << vanilla::error::line_info(t.line) << vanilla::error::pos_info(t.pos)
                    << vanilla::error::expected_type(type) << vanilla::error::received_type(t.type)
                );
            }
            
        }
    };
    
    
    vanilla::int_object::int_type string_to_int(vanilla::cstr_range str, unsigned base)
    {
        std::unique_ptr<char[]> temp(new char[str.length() + 1]);
        std::memcpy(temp.get(), str.begin(), str.length());
        temp[str.length()] = 0;
        
        return vanilla::int_object::int_type(temp.get(), base);
    }
    
    vanilla::float_object::float_type string_to_float(vanilla::cstr_range str)
    {
        std::unique_ptr<char[]> temp(new char[str.length() + 1]);
        std::memcpy(temp.get(), str.begin(), str.length());
        temp[str.length()] = 0;
        
        return vanilla::float_object::float_type(temp.get(), 10);
    }
    
    std::string process_escape_sequences(vanilla::cstr_range str)
    {
        std::string result;
        for(char const* it = str.begin(); it != str.end(); ++it)
        {
            if(*it != '\\')
            {
                result.push_back(*it);
                continue;
            }
            
            ++it;
            switch(*it)
            {
                case 'n':
                {
                    result.push_back('\n');
                    break;
                }
                
                case 't':
                {
                    result.push_back('\t');
                    break;
                }
                
                case '\\':
                {
                    result.push_back('\\');
                    break;
                }
                
                case '\"':
                {
                    result.push_back('\"');
                    break;
                }
                
                default:
                    BOOST_THROW_EXCEPTION(vanilla::error::invalid_escape_sequence()
                        << vanilla::error::escape_sequence(std::string(it - 1, it + 1)));
            }
        }
        
        return result;
    }
    
    std::string string_lit_to_string(vanilla::token* t)
    {
        assert(t->type == vanilla::ttype::string_lit);
        
        try
        {
            return process_escape_sequences(t->lexeme);
        }
        catch(vanilla::error::invalid_escape_sequence& e)
        {
            BOOST_THROW_EXCEPTION(e << vanilla::error::line_info(t->line)
                << vanilla::error::pos_info(t->pos));
        }
    }
    
    vanilla::expression_node::ptr parse_int_lit(token_buffer& buffer)
    {
        vanilla::token* t;
        if( (t = buffer.accept(vanilla::ttype::int_lit)) )
        {
            if(t->flags & vanilla::token_flags::INT_FLAG_BASE10)
            {
                return make_unique<vanilla::int_expression_node>(
                    t->line, t->pos, string_to_int(t->lexeme, 10));
            }
            
            if(t->flags & vanilla::token_flags::INT_FLAG_BASE16)
            {
                return make_unique<vanilla::int_expression_node>(
                    t->line, t->pos, string_to_int(t->lexeme, 16));
            }
            
            if(t->flags & vanilla::token_flags::INT_FLAG_BASE2)
            {
                return make_unique<vanilla::int_expression_node>(
                    t->line, t->pos, string_to_int(t->lexeme, 2));
            }
            
            if(t->flags & vanilla::token_flags::INT_FLAG_BASE8)
            {
                return make_unique<vanilla::int_expression_node>(
                    t->line, t->pos, string_to_int(t->lexeme, 8));
            }
            
            assert(false); // Invalid integer base.
        }
        
        return vanilla::expression_node::ptr();
    }
    
    vanilla::expression_node::ptr parse_real_lit(token_buffer& buffer)
    {
        vanilla::token* t;
        if( (t = buffer.accept(vanilla::ttype::real_lit)) )
        {
            return make_unique<vanilla::float_expression_node>(
                t->line, t->pos, string_to_float(t->lexeme));
        }
        
        return vanilla::expression_node::ptr();
    }
    
    vanilla::expression_node::ptr parse_string_lit(token_buffer& buffer)
    {
        vanilla::token* t;
        if( (t = buffer.accept(vanilla::ttype::string_lit)) )
        {
            return make_unique<vanilla::string_expression_node>(
                t->line, t->pos, string_lit_to_string(t));
        }
        
        return vanilla::expression_node::ptr();
    }
    
    vanilla::expression_node::ptr parse_bool_lit(token_buffer& buffer)
    {
        vanilla::token* t;
        if( (t = buffer.accept(vanilla::ttype::truelit)) )
        {
            return make_unique<vanilla::bool_expression_node>(
                t->line, t->pos, true);
        }
        else if( (t = buffer.accept(vanilla::ttype::falselit)) )
        {
            return make_unique<vanilla::bool_expression_node>(
                t->line, t->pos, false);
        }
        else if( (t = buffer.accept(vanilla::ttype::indeterminate)) )
        {
            return make_unique<vanilla::bool_expression_node>(
                t->line, t->pos, boost::logic::indeterminate);
        }
        
        return vanilla::expression_node::ptr();
    }
    
    vanilla::expression_node::ptr parse_constant(token_buffer& buffer)
    {
        vanilla::token* t;
        vanilla::expression_node::ptr n;
        if( (n = parse_int_lit(buffer)) ) { }
        else if( (n = parse_real_lit(buffer)) ) { }
        else if( (n = parse_string_lit(buffer)) ) { }
        else if( (n = parse_bool_lit(buffer)) ) { }
        
        return n;
    }
    
    vanilla::expression_node::ptr parse_variable_expression(token_buffer& buffer)
    {
        vanilla::token* t;
        if( (t = buffer.accept(vanilla::ttype::ident)) )
            return make_unique<vanilla::variable_expression_node>(
                t->line, t->pos, std::string(t->lexeme.begin(), t->lexeme.end()));
        
        return vanilla::expression_node::ptr();
    }
    
    vanilla::expression_node::ptr parse_expression(token_buffer& buffer);
    vanilla::expression_node::ptr parse_paren_expression(token_buffer& buffer)
    {
        if(!buffer.accept(vanilla::ttype::lparen))
            return vanilla::expression_node::ptr();
        
        vanilla::expression_node::ptr result = parse_expression(buffer);
        buffer.expect(vanilla::ttype::rparen);
        return result;
    }
    
    std::vector<std::pair<std::string, vanilla::expression_node::ptr>>
        parse_function_arguments(token_buffer& buffer)
    {
        buffer.expect(vanilla::ttype::lparen);
        std::vector<std::pair<std::string, vanilla::expression_node::ptr>> result;
        while(!buffer.accept(vanilla::ttype::rparen))
        {
            vanilla::token* curname = buffer.expect(vanilla::ttype::ident);
            auto name = std::string(curname->lexeme.begin(), curname->lexeme.end());
            vanilla::expression_node::ptr default_value;
            if(buffer.accept(vanilla::ttype::assign))
                default_value = parse_expression(buffer);
            result.push_back(std::make_pair(std::move(name), std::move(default_value)));
            
            if(!buffer.accept(vanilla::ttype::comma))
            {
                buffer.expect(vanilla::ttype::rparen);
                break;
            }
        }
        
        return result;
    }
    
    vanilla::statement_node::ptr parse_statement(token_buffer& buffer);
    vanilla::expression_node::ptr parse_function_definition_expression(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::function)) )
            return vanilla::expression_node::ptr();
        
        vanilla::token* name_token = buffer.accept(vanilla::ttype::ident);
        std::string name;
        if(name_token)
            name.assign(name_token->lexeme.begin(), name_token->lexeme.end());
        else
            name = "!anonymous function";

        // Parse argument list.
        buffer.expect(vanilla::ttype::lparen);
        std::vector<std::pair<std::string, vanilla::expression_node::ptr>> arguments;
        while(!buffer.accept(vanilla::ttype::rparen))
        {
            vanilla::token* curname_token = buffer.expect(vanilla::ttype::ident);
            std::string curname(curname_token->lexeme.begin(), curname_token->lexeme.end());
            
            vanilla::expression_node::ptr default_expression;
            if(buffer.accept(vanilla::ttype::assign))
                default_expression = parse_expression(buffer);
            
            arguments.push_back(std::make_pair(std::move(curname), std::move(default_expression)));
            if(!buffer.accept(vanilla::ttype::comma))
            {
                buffer.expect(vanilla::ttype::rparen);
                break;
            }
        }
        
        // Parse the function body.
        vanilla::statement_node::ptr body_unique = parse_statement(buffer);
        std::shared_ptr<vanilla::statement_node> body(body_unique.get());
        body_unique.release();
        
        return make_unique<vanilla::function_definition_expression_node>(
            t->line, t->pos, std::move(name), std::move(arguments), std::move(body));
    }
    
    vanilla::expression_node::ptr parse_native_function_definition_expression(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::native)) )
            return vanilla::expression_node::ptr();
        
        std::string function_name = string_lit_to_string(buffer.expect(vanilla::ttype::string_lit));
        buffer.expect(vanilla::ttype::from);
        std::string library_name = string_lit_to_string(buffer.expect(vanilla::ttype::string_lit));
        buffer.expect(vanilla::ttype::declared);
        
        std::string return_type = string_lit_to_string(buffer.expect(vanilla::ttype::string_lit));
        buffer.expect(vanilla::ttype::lparen);
        std::vector<std::string> argtypes;
        while(!buffer.accept(vanilla::ttype::rparen))
        {
            argtypes.push_back(string_lit_to_string(buffer.expect(vanilla::ttype::string_lit)));
            if(!buffer.accept(vanilla::ttype::comma))
            {
                buffer.expect(vanilla::ttype::rparen);
                break;
            }
        }
        
        return make_unique<vanilla::native_function_definition_expression_node>(
            t->line, t->pos, std::move(library_name), std::move(function_name),
            std::move(return_type), std::move(argtypes));
    }
    
    vanilla::expression_node::ptr parse_array_expression(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::lbrack)) )
            return vanilla::expression_node::ptr();
        
        std::vector<vanilla::expression_node::ptr> values;
        while(!buffer.accept(vanilla::ttype::rbrack))
        {
            values.push_back(parse_expression(buffer));
            if(!buffer.accept(vanilla::ttype::comma))
            {
                buffer.expect(vanilla::ttype::rbrack);
                break;
            }
        }
        
        return make_unique<vanilla::array_expression_node>(
            t->line, t->pos, std::move(values));
    }
    
    vanilla::expression_node::ptr parse_primary_expression(token_buffer& buffer)
    {
        vanilla::expression_node::ptr n;
        if( (n = parse_constant(buffer)) )
            return n;
        if( (n = parse_variable_expression(buffer)) )
            return n;
        if( (n = parse_paren_expression(buffer)) )
            return n;
        if( (n = parse_function_definition_expression(buffer)) )
            return n;
        if( (n = parse_native_function_definition_expression(buffer)) )
            return n;
        if( (n = parse_array_expression(buffer)) )
            return n;
        
        vanilla::token* t = buffer.cur();
        BOOST_THROW_EXCEPTION(vanilla::error::expected_primary_expression_error()
            << vanilla::error::line_info(t->line) << vanilla::error::pos_info(t->pos)
            << vanilla::error::received_type(t->type));
    }
    
    vanilla::expression_node::ptr parse_postfix_expression(token_buffer& buffer)
    {
        vanilla::expression_node::ptr expr = parse_primary_expression(buffer);
        for(;;)
        {
            // Function call.
            if(buffer.accept(vanilla::ttype::lparen))
            {
                // Parse function call argument list.
                std::vector<vanilla::expression_node::ptr> arg_list;
                while(!buffer.accept(vanilla::ttype::rparen))
                {
                    arg_list.push_back(parse_expression(buffer));
                    if(!buffer.accept(vanilla::ttype::comma))
                    {
                        buffer.expect(vanilla::ttype::rparen);
                        break;
                    }
                }
                
                expr = make_unique<vanilla::function_call_expression_node>(
                    expr->get_line(), expr->get_pos(), std::move(expr), std::move(arg_list));
                continue;
            }
            
            // Subscripting.
            if(buffer.accept(vanilla::ttype::lbrack))
            {
                vanilla::expression_node::ptr subscript = parse_expression(buffer);
                buffer.expect(vanilla::ttype::rbrack);
                
                expr = make_unique<vanilla::subscript_expression_node>(
                    expr->get_line(), expr->get_pos(), std::move(expr), std::move(subscript));
                continue;
            }
            
            // Element selection.
            if(buffer.accept(vanilla::ttype::element_selection))
            {
                vanilla::token* t = buffer.expect(vanilla::ttype::ident);
                std::string element_name = std::string(t->lexeme.begin(), t->lexeme.end());
                
                expr = make_unique<vanilla::element_selection_expression_node>(
                    expr->get_line(), expr->get_pos(), std::move(expr), std::move(element_name));
                continue;
            }
            
            break;
        }
        
        return expr;
    }
    
    vanilla::expression_node::ptr parse_prefix_expression(token_buffer& buffer)
    {
        vanilla::token* t;
        if( (t = buffer.accept(vanilla::ttype::minus)) )
            return make_unique<vanilla::negation_expression_node>(
                t->line, t->pos, parse_prefix_expression(buffer));
        if( (t = buffer.accept(vanilla::ttype::plus)) )
            return make_unique<vanilla::abs_expression_node>(
                t->line, t->pos, parse_prefix_expression(buffer));
        return parse_postfix_expression(buffer);
    }
    
    vanilla::expression_node::ptr parse_multiplicative_expression(token_buffer& buffer)
    {
        vanilla::expression_node::ptr left = parse_prefix_expression(buffer);
        if(buffer.accept(vanilla::ttype::mul))
        {
            vanilla::expression_node::ptr right = parse_multiplicative_expression(buffer);
            return make_unique<vanilla::multiplication_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        if(buffer.accept(vanilla::ttype::div))
        {
            vanilla::expression_node::ptr right = parse_multiplicative_expression(buffer);
            return make_unique<vanilla::division_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        return left;
    }
    
    vanilla::expression_node::ptr parse_additive_expression(token_buffer& buffer)
    {
        vanilla::expression_node::ptr left = parse_multiplicative_expression(buffer);
        if(buffer.accept(vanilla::ttype::plus))
        {
            vanilla::expression_node::ptr right = parse_additive_expression(buffer);
            return make_unique<vanilla::addition_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        if(buffer.accept(vanilla::ttype::minus))
        {
            vanilla::expression_node::ptr right = parse_additive_expression(buffer);
            return make_unique<vanilla::subtraction_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        if(buffer.accept(vanilla::ttype::concat))
        {
            vanilla::expression_node::ptr right = parse_additive_expression(buffer);
            return make_unique<vanilla::concatenation_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        return left;
    }
    
    vanilla::expression_node::ptr parse_relational_expression(token_buffer& buffer)
    {
        vanilla::expression_node::ptr left = parse_additive_expression(buffer);
        if(buffer.accept(vanilla::ttype::less))
        {
            vanilla::expression_node::ptr right = parse_relational_expression(buffer);
            return make_unique<vanilla::lessthan_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        if(buffer.accept(vanilla::ttype::less_equal))
        {
            vanilla::expression_node::ptr right = parse_relational_expression(buffer);
            return make_unique<vanilla::lessequal_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        if(buffer.accept(vanilla::ttype::greater))
        {
            vanilla::expression_node::ptr right = parse_relational_expression(buffer);
            return make_unique<vanilla::greaterthan_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        if(buffer.accept(vanilla::ttype::greater_equal))
        {
            vanilla::expression_node::ptr right = parse_relational_expression(buffer);
            return make_unique<vanilla::greaterequal_expression_node>(
                left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        return left;
    }
    
    vanilla::expression_node::ptr parse_equality_expression(token_buffer& buffer)
    {
        vanilla::expression_node::ptr left = parse_relational_expression(buffer);
        if(buffer.accept(vanilla::ttype::equal))
        {
            vanilla::expression_node::ptr right = parse_equality_expression(buffer);
            return make_unique<vanilla::equality_expression_node>(left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        if(buffer.accept(vanilla::ttype::not_equal))
        {
            vanilla::expression_node::ptr right = parse_equality_expression(buffer);
            return make_unique<vanilla::inequality_expression_node>(left->get_line(), left->get_pos(), std::move(left), std::move(right));
        }
        
        return left;
    }
    
    vanilla::expression_node::ptr parse_ternary_expression(token_buffer& buffer)
    {
        vanilla::expression_node::ptr cond = parse_equality_expression(buffer);
        if(!buffer.accept(vanilla::ttype::questionmark))
            return cond;
        
        vanilla::expression_node::ptr expr = parse_ternary_expression(buffer);
        buffer.expect(vanilla::ttype::colon);
        vanilla::expression_node::ptr else_ = parse_ternary_expression(buffer);
        
        return make_unique<vanilla::conditional_expression_node>(
            expr->get_line(), expr->get_pos(),
            std::move(cond), std::move(expr), std::move(else_));
    }
    
    vanilla::expression_node::ptr parse_expression(token_buffer& buffer)
    {
        return parse_ternary_expression(buffer);
    }
    
    vanilla::statement_node::ptr parse_assignment_statement(token_buffer& buffer)
    {
        vanilla::expression_node::ptr lhs = parse_expression(buffer);
        if(buffer.accept(vanilla::ttype::assign))
        {
            vanilla::expression_node::ptr rhs = parse_expression(buffer);
            buffer.expect(vanilla::ttype::endstmnt);
            return make_unique<vanilla::assignment_statement_node>(
                lhs->get_line(), lhs->get_pos(), std::move(lhs), std::move(rhs));
        }

        buffer.expect(vanilla::ttype::endstmnt);
        return make_unique<vanilla::expression_statement_node>(
            lhs->get_line(), lhs->get_pos(), std::move(lhs));
    }
    
    vanilla::statement_node::ptr parse_return_statement(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::ret)) )
            return vanilla::statement_node::ptr();
        
        vanilla::expression_node::ptr expr = parse_expression(buffer);
        buffer.expect(vanilla::ttype::endstmnt);
        
        return make_unique<vanilla::return_statement_node>(
            t->line, t->pos, std::move(expr));
    }
    
    vanilla::statement_node::ptr parse_if_statement(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::if_)) )
            return vanilla::statement_node::ptr();
        
        // Parse if.
        std::vector<std::pair<vanilla::expression_node::ptr, vanilla::statement_node::ptr>> ifs;
        ifs.emplace_back(std::make_pair(parse_expression(buffer), parse_statement(buffer)));
        
        // Parse elseifs.
        while(buffer.accept(vanilla::ttype::elseif))
            ifs.emplace_back(std::make_pair(parse_expression(buffer), parse_statement(buffer)));
        
        // Parse else.
        vanilla::statement_node::ptr else_;
        if(buffer.accept(vanilla::ttype::else_))
            else_ = parse_statement(buffer);

        return make_unique<vanilla::if_statement_node>(t->line, t->pos, std::move(ifs), std::move(else_));
    }
    
    vanilla::statement_node::ptr parse_while_statement(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::while_)) )
            return vanilla::statement_node::ptr();
        
        vanilla::expression_node::ptr condition = parse_expression(buffer);
        vanilla::statement_node::ptr code = parse_statement(buffer);
        return make_unique<vanilla::while_statement_node>(t->line, t->pos,
            std::move(condition), std::move(code));
    }
    
    vanilla::statement_node::ptr parse_function_definition_statement(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::function)) )
            return vanilla::statement_node::ptr();
        
        vanilla::token* name_token = buffer.expect(vanilla::ttype::ident);
        std::string name(name_token->lexeme.begin(), name_token->lexeme.end());

        // Parse argument list.
        buffer.expect(vanilla::ttype::lparen);
        std::vector<std::pair<std::string, vanilla::expression_node::ptr>> arguments;
        while(!buffer.accept(vanilla::ttype::rparen))
        {
            vanilla::token* curname_token = buffer.expect(vanilla::ttype::ident);
            std::string curname(curname_token->lexeme.begin(), curname_token->lexeme.end());
            
            vanilla::expression_node::ptr default_expression;
            if(buffer.accept(vanilla::ttype::assign))
                default_expression = parse_expression(buffer);
            
            arguments.push_back(std::make_pair(std::move(curname), std::move(default_expression)));
            if(!buffer.accept(vanilla::ttype::comma))
            {
                buffer.expect(vanilla::ttype::rparen);
                break;
            }
        }
        
        // Parse the function body.
        vanilla::statement_node::ptr body_unique = parse_statement(buffer);
        std::shared_ptr<vanilla::statement_node> body(body_unique.get());
        body_unique.release();
        
        return make_unique<vanilla::function_definition_statement_node>(
            t->line, t->pos, std::move(name), std::move(arguments), std::move(body));
    }
    
    vanilla::statement_node::ptr parse_statement(token_buffer& buffer);    
    vanilla::statement_node::ptr parse_code_block(token_buffer& buffer)
    {
        vanilla::token* t;
        if( !(t = buffer.accept(vanilla::ttype::lbrace)) )
            return vanilla::statement_node::ptr();
        
        std::vector<vanilla::statement_node::ptr> block;
        while(!buffer.accept(vanilla::ttype::rbrace))
            block.push_back(parse_statement(buffer));
        
        return make_unique<vanilla::statement_sequence_node>(t->line, t->pos, std::move(block));
    }
    
    vanilla::statement_node::ptr parse_statement(token_buffer& buffer)
    {
        vanilla::statement_node::ptr n;
        if( (n = parse_return_statement(buffer)) )
            return std::move(n);
        
        if( (n = parse_code_block(buffer)) )
            return std::move(n);
        
        if( (n = parse_if_statement(buffer)) )
            return std::move(n);
        
        if( (n = parse_while_statement(buffer)) )
            return n;
        
        if( (n = parse_function_definition_statement(buffer)) )
            return n;
        
        return parse_assignment_statement(buffer);
    }
}

vanilla::expression_node::ptr vanilla::parse_expr(char const* expr)
{
    scanner scan( (expr) );
    token_buffer buffer( (scan) );
    return parse_expression(buffer);
}

vanilla::statement_node::ptr vanilla::parse_string(char const* str)
{
    scanner scan( (str) );
    token_buffer buffer( (scan) );
    
    std::vector<vanilla::statement_node::ptr> block;
    while(!buffer.accept(vanilla::ttype::eof))
        block.push_back(parse_statement(buffer));
    return make_unique<vanilla::statement_sequence_node>(0, 0, std::move(block));
}

vanilla::statement_node::ptr vanilla::parse_file(char const* filename)
{
    std::ifstream in( (filename) );
    std::string str( (std::istreambuf_iterator<char>(in)) , std::istreambuf_iterator<char>());
    return parse_string(str.c_str());
}