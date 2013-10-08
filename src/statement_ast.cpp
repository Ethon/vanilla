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
#include <cassert>

// Vanilla:
#include <vanilla/statement_ast.hpp>
#include <vanilla/object.hpp>
#include <vanilla/bool_object.hpp>
#include <vanilla/function_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::statement_node
///////////////////////////////////////////////////////////////////////////

vanilla::statement_node::statement_node(
            unsigned line,
            unsigned pos)
    :   ast_node(line, pos)
{ }

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::expression_statement_node
///////////////////////////////////////////////////////////////////////////

vanilla::expression_statement_node::expression_statement_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr expression )
    :   statement_node(line, pos),
        _expression(std::move(expression))
{ }

vanilla::expression_node* vanilla::expression_statement_node::get_expression()
{
    return _expression.get();
}

void vanilla::expression_statement_node::eval(context& c)
{
    _expression->eval(c);
}

void vanilla::expression_statement_node::accept(ast_visitor* v)
{
    _expression->accept(v);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::statement_sequence_node
///////////////////////////////////////////////////////////////////////////

vanilla::statement_sequence_node::statement_sequence_node(
            unsigned line,
            unsigned pos,
            std::vector<statement_node::ptr> code )
    :   statement_node(line, pos),
        _code(std::move(code))
{ }

std::vector<vanilla::statement_node::ptr>&
vanilla::statement_sequence_node::get_code()
{
    return _code;
}
        
void vanilla::statement_sequence_node::eval(context& c)
{
    for(statement_node::ptr& cur : _code)
        cur->eval(c);
}

void vanilla::statement_sequence_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::return_statement_node
///////////////////////////////////////////////////////////////////////////

vanilla::return_statement_node::return_statement_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr expression )
    :   statement_node(line, pos),
        _expression(std::move(expression))
{ }

vanilla::expression_node* vanilla::return_statement_node::get_expression()
{
    return _expression.get();
}

void vanilla::return_statement_node::eval(context& c)
{
    object::ptr result = _expression->eval(c);
    throw detail::function_returned{ std::move(result) };
}

void vanilla::return_statement_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::if_statement_node
///////////////////////////////////////////////////////////////////////////

vanilla::if_statement_node::if_statement_node(
            unsigned line,
            unsigned pos,
            std::vector<std::pair<expression_node::ptr, statement_node::ptr>> ifs,
            statement_node::ptr else_ )
    :   statement_node(line, pos),
        _ifs(std::move(ifs)),
        _else(std::move(else_))
{ }
        
std::vector<std::pair<vanilla::expression_node::ptr, vanilla::statement_node::ptr>> const&
vanilla::if_statement_node::get_ifs()
{
    return _ifs;
}
        
vanilla::statement_node* vanilla::if_statement_node::get_else()
{
    return _else.get();
}

void vanilla::if_statement_node::eval(context& c)
{
    for(auto& if_ : _ifs)
    {
        if(bool_object_to_cpp_bool(if_.first->eval(c)->to_bool()))
        {
            if_.second->eval(c);
            return;
        }
    }
    
    if(_else)
        _else->eval(c);
}

void vanilla::if_statement_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::while_statement_node
///////////////////////////////////////////////////////////////////////////

vanilla::while_statement_node::while_statement_node(
                unsigned line,
                unsigned pos,
                expression_node::ptr condition,
                statement_node::ptr code )
    :   statement_node(line, pos),
        _condition(std::move(condition)),
        _code(std::move(code))
{ }

vanilla::expression_node* vanilla::while_statement_node::get_condition()
{
    return _condition.get();
}

vanilla::statement_node* vanilla::while_statement_node::get_code()
{
    return _code.get();
}

void vanilla::while_statement_node::eval(context& c)
{
    while(bool_object_to_cpp_bool(_condition->eval(c)->to_bool()))
        _code->eval(c);
}

void vanilla::while_statement_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::function_definition_statement_node
///////////////////////////////////////////////////////////////////////////

vanilla::function_definition_statement_node::function_definition_statement_node(
            unsigned line,
            unsigned pos,
            std::string name,
            std::vector<std::pair<std::string, expression_node::ptr>> arguments,
            std::shared_ptr<statement_node> body )
    :   statement_node(line, pos),
        _name(std::move(name)),
        _arguments(std::move(arguments)),
        _body(std::move(body))
{ }
        
std::string const& vanilla::function_definition_statement_node::get_name() const
{
    return _name;
}

std::vector<std::pair<std::string, vanilla::expression_node::ptr>> const&
vanilla::function_definition_statement_node::get_arguments()
{
    return _arguments;
}

vanilla::statement_node* vanilla::function_definition_statement_node::get_body()
{
    return _body.get();
}

void vanilla::function_definition_statement_node::eval(context& c)
{
    auto callable = [=](context& c_, object::ptr*, unsigned) -> object::ptr
    {
        try
        {
            _body->eval(c_);
            return allocate_object<none_object>();
        }
        catch(detail::function_returned& e)
        {
            return std::move(e.result);
        }
    };
    
    std::vector<function_argument> arguments;
    arguments.reserve(_arguments.size());
    for(auto& pair : _arguments)
    {
        if(pair.second)
            arguments.push_back(function_argument(pair.first, pair.second->eval(c)));
        else
            arguments.push_back(function_argument(pair.first));
    }
    
    c.set_value(_name, allocate_object<function_object>(_name, std::move(arguments), std::move(callable)));
}

void vanilla::function_definition_statement_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::assignment_statement_node
///////////////////////////////////////////////////////////////////////////

vanilla::assignment_statement_node::assignment_statement_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr lhs,
            expression_node::ptr rhs )
    :   statement_node(line, pos),
        _lhs(std::move(lhs)),
        _rhs(std::move(rhs))
{ }

vanilla::expression_node* vanilla::assignment_statement_node::get_left()
{
    return _lhs.get();
}
        
vanilla::expression_node* vanilla::assignment_statement_node::get_right()
{
    return _rhs.get();
}

void vanilla::assignment_statement_node::eval(context& c)
{
    // We need to inspect the left side argument first.
    variable_expression_node* var_node = dynamic_cast<variable_expression_node*>(_lhs.get());
    if(var_node)
    {
        c.set_value(var_node->get_name(), _rhs->eval(c));
        return;
    }
    
    assert(false);
}

void vanilla::assignment_statement_node::accept(ast_visitor* v)
{
    v->visit(this);
}
