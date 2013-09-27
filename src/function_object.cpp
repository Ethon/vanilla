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
#include <vanilla/function_object.hpp>
#include <vanilla/string_object.hpp>
#include <vanilla/context.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::function_argument
///////////////////////////////////////////////////////////////////////////

vanilla::function_argument::function_argument(
            std::string name,
            object::ptr default_value)
    :   _name(std::move(name)),
        _default_value(std::move(default_value))
{ }
        
std::string const& vanilla::function_argument::get_name() const
{
    return _name;
}
        
vanilla::object::ptr vanilla::function_argument::get_default_value() const
{
    return _default_value;
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::function_object
///////////////////////////////////////////////////////////////////////////

vanilla::function_object::function_object(
            std::string name,
            std::vector<function_argument> arguments,
            callable_type f,
            bool set_context,
            bool variadic)
    :   _name(std::move(name)),
        _arguments(std::move(arguments)),
        _f(std::move(f)),
        _set_context(set_context),
        _variadic(variadic),
        _min_args(0)
{
    // Validate default arguments.
    for(unsigned i = 0; i < _arguments.size() && !_arguments[i].get_default_value(); ++i)
        ++_min_args;
    for(unsigned i = _min_args; i < _arguments.size(); ++i)
    {
        if(!_arguments[i].get_default_value())
        {
            BOOST_THROW_EXCEPTION(error::missing_default_argument_error()
                << error::argument_name(_arguments[i].get_name()));
        }
    }
}
     
vanilla::object_type_id vanilla::function_object::type_id() const
{
    return OBJECT_ID_FUNCTION;
}
        
vanilla::object::ptr vanilla::function_object::type_name() const
{
    return allocate_object<string_object>("function");
}
        
vanilla::object::ptr vanilla::function_object::clone() const
{
    return const_cast<function_object*>(this)->shared_from_this();
}
        
vanilla::object::ptr vanilla::function_object::to_string() const
{
    std::string result;
    result += "<function '";
    result += _name;
    result += "'>";
    return allocate_object<string_object>(std::move(result));
}
        
vanilla::object::ptr vanilla::function_object::call(context& c, ptr* argv, unsigned argc)
{
    if(argc < _min_args)
    {
        BOOST_THROW_EXCEPTION(error::not_enough_arguments_error()
            << error::function_name(_name)
            << error::num_arguments_expected(_min_args)
            << error::num_arguments_received(argc));
    }
    
    if(argc > _arguments.size())
    {
        BOOST_THROW_EXCEPTION(error::too_many_arguments_error()
            << error::function_name(_name)
            << error::num_arguments_expected(_min_args)
            << error::num_arguments_received(argc));
    }
    
    struct stackframe_helper
    {
        context& c;
        
        stackframe_helper(context& c)
            : c(c)
        {
            c.begin_stackframe();
        }
        
        ~stackframe_helper()
        {
            c.end_stackframe();
        }
    } helper( (c) );
    
    if(_set_context)
    {
        for(unsigned i = 0; i < argc; ++i)
            c.set_local_value(_arguments[i].get_name(), argv[i]);
        for(unsigned i = argc; i < _arguments.size(); ++i)
            c.set_local_value(_arguments[i].get_name(), _arguments[i].get_default_value());
        
        return _f(c, nullptr, 0);
    }
    else
    {
        // All arguments given - just call.
        if(argc == _arguments.size())
        {
            return _f(c, argv, argc);
        }
        
        // Use stack for small argv.
        if(_arguments.size() < 8)
        {
            ptr complete_argv[8];
            for(unsigned i = 0; i < argc; ++i)
                complete_argv[i] = argv[i];
            for(unsigned i = argc; i < _arguments.size(); ++i)
                complete_argv[i] = _arguments[i].get_default_value();
            return _f(c, complete_argv, _arguments.size());
        }
        
        // Use heap for large argv.
        else
        {
            std::vector<ptr> complete_argv(_arguments.size());
            for(unsigned i = 0; i < argc; ++i)
                complete_argv[i] = argv[i];
            for(unsigned i = argc; i < _arguments.size(); ++i)
                complete_argv[i] = _arguments[i].get_default_value();
            return _f(c, complete_argv.data(), _arguments.size());
        }
    }
}