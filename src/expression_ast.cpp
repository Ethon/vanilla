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

// Vanilla:
#include <vanilla/expression_ast.hpp>
#include <vanilla/statement_ast.hpp>
#include <vanilla/native_function_object.hpp>
#include <vanilla/native_library_cache.hpp>
#include <vanilla/function_object.hpp>
#include <vanilla/bool_object.hpp>
#include <vanilla/array_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::expression_node::expression_node(
            unsigned line,
            unsigned pos)
    :   ast_node(line, pos)
{ }

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::nullary_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::nullary_expression_node::nullary_expression_node(
            unsigned line,
            unsigned pos)
    :   expression_node(line, pos)
{ }

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::unary_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::unary_expression_node::unary_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr child)
    :   expression_node(line, pos), _child(std::move(child))
{ }
        
vanilla::expression_node* vanilla::unary_expression_node::get_child()
{
    return _child.get();
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::binary_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::binary_expression_node::binary_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   expression_node(line, pos),
        _left(std::move(left)),
        _right(std::move(right))
{ }

vanilla::expression_node* vanilla::binary_expression_node::get_left()
{
    return _left.get();
}

vanilla::expression_node* vanilla::binary_expression_node::get_right()
{
    return _right.get();
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::variable_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::variable_expression_node::variable_expression_node(
            unsigned line,
            unsigned pos,
            std::string name)
    :   nullary_expression_node(line, pos), _name(std::move(name))
{ }
        
vanilla::object::ptr vanilla::variable_expression_node::eval(context& c)
{
    try
    {
        return c.get_value(_name);
    }
    catch(error::undefined_value_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}

std::string const& vanilla::variable_expression_node::get_name() const
{
    return _name;
}

void vanilla::variable_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::int_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::int_expression_node::int_expression_node(
            unsigned line,
            unsigned pos,
            int_object::int_type v)
    :   value_expression_node<int_object::int_type, int_object>(line, pos, std::move(v))
{ }

void vanilla::int_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::float_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::float_expression_node::float_expression_node(
            unsigned line,
            unsigned pos,
            float_object::float_type v)
    :   value_expression_node<float_object::float_type, float_object>(line, pos, std::move(v))
{ }

void vanilla::float_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::string_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::string_expression_node::string_expression_node(
            unsigned line,
            unsigned pos,
            string_object::string_type v)
    :   value_expression_node<string_object::string_type, string_object>(line, pos, std::move(v))
{ }

void vanilla::string_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::bool_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::bool_expression_node::bool_expression_node(
            unsigned line,
            unsigned pos,
            bool_object::bool_type v)
    :   value_expression_node<bool_object::bool_type, bool_object>(line, pos, std::move(v))
{ }

void vanilla::bool_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::array_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::array_expression_node::array_expression_node(
            unsigned line,
            unsigned pos,
            std::vector<expression_node::ptr> values)
    :   expression_node(line, pos),
        _values(std::move(values))        
{ }

vanilla::object::ptr vanilla::array_expression_node::eval(context& c)
{
    array_object::array_type result;
    result.reserve(_values.size());
    for(auto& cur : _values)
        result.push_back(cur->eval(c));
    return allocate_object<array_object>(std::move(result));
}       

void vanilla::array_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

std::vector<vanilla::expression_node::ptr> const&
vanilla::array_expression_node::values()
{
    return _values;
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::negation_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::negation_expression_node::negation_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr child)
    :   unary_expression_node(line, pos, std::move(child))
{ }

vanilla::object::ptr vanilla::negation_expression_node::eval(context& c)
{
    try
    {
        return _child->eval(c)->neg();
    }
    catch(error::bad_unary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}

void vanilla::negation_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::abs_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::abs_expression_node::abs_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr child)
    :   unary_expression_node(line, pos, std::move(child))
{ }

vanilla::object::ptr vanilla::abs_expression_node::eval(context& c)
{
    try
    {
        return _child->eval(c)->abs();
    }
    catch(error::bad_unary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}

void vanilla::abs_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::addition_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::addition_expression_node::addition_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::addition_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->add(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::addition_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::subtraction_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::subtraction_expression_node::subtraction_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::subtraction_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->sub(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::subtraction_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::multiplication_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::multiplication_expression_node::multiplication_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::multiplication_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->mul(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::multiplication_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::division_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::division_expression_node::division_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::division_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->div(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::division_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::lessthan_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::lessthan_expression_node::lessthan_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::lessthan_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->lt(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::lessthan_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::lessequal_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::lessequal_expression_node::lessequal_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::lessequal_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->le(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::lessequal_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::greaterthan_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::greaterthan_expression_node::greaterthan_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::greaterthan_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->gt(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::greaterthan_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::greaterequal_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::greaterequal_expression_node::greaterequal_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::greaterequal_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->ge(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::greaterequal_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::equality_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::equality_expression_node::equality_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::equality_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->eq(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::equality_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::inequality_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::inequality_expression_node::inequality_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::inequality_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->neq(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::inequality_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::concatenation_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::concatenation_expression_node::concatenation_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            expression_node::ptr right)
    :   binary_expression_node(line, pos, std::move(left), std::move(right))
{ }
    
vanilla::object::ptr vanilla::concatenation_expression_node::eval(context& c)
{
    try
    {
        return _left->eval(c)->concat(_right->eval(c));
    }
    catch(error::bad_binary_operation_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}
        
void vanilla::concatenation_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::function_call_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::function_call_expression_node::function_call_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr function,
            std::vector<expression_node::ptr> args )
    :   expression_node(line, pos),
        _function(std::move(function)),
        _args(std::move(args))
{ }

vanilla::object::ptr vanilla::function_call_expression_node::eval(context& c)
{
    unsigned argc = _args.size();
    try
    {
        // Use stack for small argv.
        if(argc < 8)
        {
            object::ptr argv[8];
            for(unsigned i = 0; i < argc; ++i)
                argv[i] = _args[i]->eval(c);
            return _function->eval(c)->call(c, argv, argc);
        }
        
        // Use heap for large argv.
        else
        {
            std::vector<object::ptr> argv( (argc) );
            for(unsigned i = 0; i < argc; ++i)
                argv[i] = _args[i]->eval(c);
            return _function->eval(c)->call(c, argv.data(), argc);
        }
    }
    catch(error::value_not_callable_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}

vanilla::expression_node* vanilla::function_call_expression_node::get_function()
{
    return _function.get();
}

std::vector<vanilla::expression_node::ptr> const&
vanilla::function_call_expression_node::get_args()
{
    return _args;
}

void vanilla::function_call_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::function_definition_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::function_definition_expression_node::function_definition_expression_node(
            unsigned line,
            unsigned pos,
            std::string name,
            std::vector<std::pair<std::string, expression_node::ptr>> arguments,
            std::shared_ptr<statement_node> body)
    :   expression_node(line, pos),
        _name(std::move(name)),
        _arguments(std::move(arguments)),
        _body(std::move(body))
{ }

vanilla::object::ptr vanilla::function_definition_expression_node::eval(context& c)
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
    
    return allocate_object<function_object>(
        _name, std::move(arguments), std::move(callable));
}

std::string const& vanilla::function_definition_expression_node::get_name() const
{
    return _name;
}

std::vector<std::pair<std::string, vanilla::expression_node::ptr>> const&
vanilla::function_definition_expression_node::get_arguments()
{
    return _arguments;
}

vanilla::statement_node* vanilla::function_definition_expression_node::get_body()
{
    return _body.get();
}

void vanilla::function_definition_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::native_function_definition_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::native_function_definition_expression_node::native_function_definition_expression_node(
            unsigned line,
            unsigned pos,
            std::string library,
            std::string name,
            std::string return_type,
            std::vector<std::string> argument_types )
    :   expression_node(line, pos),
        _library(std::move(library)),
        _name(std::move(name)),
        _return_type(std::move(return_type)),
        _argument_types(std::move(argument_types))
{ }
        
vanilla::object::ptr vanilla::native_function_definition_expression_node::eval(context&)
{
    try
    {
        return allocate_object<native_function_object>(
            _name, _library, _return_type, _argument_types);
    }
    catch(error::native_library_loading_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line())
            << error::pos_info(get_pos()));
    }
    catch(error::native_symbol_not_found_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line())
            << error::pos_info(get_pos()));
    }
    catch(error::unknown_native_type_name_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line())
            << error::pos_info(get_pos()));
    }
    catch(error::void_as_argument_type_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line())
            << error::pos_info(get_pos()));
    }
}
        
std::string const&
vanilla::native_function_definition_expression_node::get_library() const
{
    return _library;
}
        
std::string const&
vanilla::native_function_definition_expression_node::get_name() const
{
    return _name;
}
        
std::string const&
vanilla::native_function_definition_expression_node::get_return_type() const
{
    return _return_type;
}
        
std::vector<std::string> const&
vanilla::native_function_definition_expression_node::get_argument_types() const
{
    return _argument_types;
}
        
void vanilla::native_function_definition_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::conditional_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::conditional_expression_node::conditional_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr condition,
            expression_node::ptr expr,
            expression_node::ptr else_ )
    :   expression_node(line, pos),
        _condition(std::move(condition)),
        _expr(std::move(expr)),
        _else(std::move(else_))
{ }

vanilla::object::ptr vanilla::conditional_expression_node::eval(context& c)
{
    try
    {
        return bool_object_to_cpp_bool(_condition->eval(c)->to_bool()) ?
            _expr->eval(c) : _else->eval(c);
    }
    catch(error::bad_cast_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::line_info(get_line()) << error::pos_info(get_pos()));
    }
}

vanilla::expression_node* vanilla::conditional_expression_node::get_condition()
{
    return _condition.get();
}

vanilla::expression_node* vanilla::conditional_expression_node::get_expression()
{
    return _expr.get();
}

vanilla::expression_node* vanilla::conditional_expression_node::get_else()
{
    return _else.get();
}

void vanilla::conditional_expression_node::accept(ast_visitor* v)
{
    v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::subscript_expression_node
///////////////////////////////////////////////////////////////////////////

vanilla::subscript_expression_node::subscript_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr expr,
            expression_node::ptr subscript )
    :   expression_node(line, pos),
        _expr(std::move(expr)),
        _subscript(std::move(subscript))
{ }
        
vanilla::object::ptr vanilla::subscript_expression_node::eval(context& c)
{
    return _expr->eval(c)->sget(_subscript->eval(c));
}
        
vanilla::expression_node* vanilla::subscript_expression_node::get_expression()
{
    return _expr.get();
}
        
vanilla::expression_node* vanilla::subscript_expression_node::get_subscript()
{
    return _subscript.get();
}
        
void vanilla::subscript_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::element_selection_expression_node
///////////////////////////////////////////////////////////////////////////
        
vanilla::element_selection_expression_node::element_selection_expression_node(
            unsigned line,
            unsigned pos,
            expression_node::ptr left,
            std::string element_name)
    :   expression_node(line, pos),
        _left(std::move(left)),
        _element_name(std::move(element_name))
{ }
        
vanilla::object::ptr vanilla::element_selection_expression_node::eval(context& c)
{
    return _left->eval(c)->eget(_element_name);
}
        
vanilla::expression_node* vanilla::element_selection_expression_node::get_left()
{
    return _left.get();
}
        
std::string const& vanilla::element_selection_expression_node::get_element_name()
{
    return _element_name;
}
        
void vanilla::element_selection_expression_node::accept(ast_visitor* v)
{
    //v->visit(this);
}