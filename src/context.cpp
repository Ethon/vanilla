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
#include <vanilla/context.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::context
///////////////////////////////////////////////////////////////////////////

vanilla::context::context()
    : _globals(), _locals()
{ }

vanilla::object::ptr
vanilla::context::get_value(std::string const& name) const
{
    if(_locals.empty())
        return get_global_value(name);
    
    auto iter = _locals.back().find(name);
    if(iter == _locals.back().end())
        return get_global_value(name);
    return iter->second;
}

vanilla::object::ptr
vanilla::context::get_global_value(std::string const& name) const
{
    auto iter = _globals.find(name);
    if(iter == _globals.end())
        BOOST_THROW_EXCEPTION(error::undefined_value_error() << error::value_name(name) );
    return iter->second;
}

vanilla::object::ptr
vanilla::context::get_local_value(std::string const& name) const
{
    assert(!_locals.empty());
    auto iter = _locals.back().find(name);
    if(iter == _locals.back().end())
        BOOST_THROW_EXCEPTION(error::undefined_value_error() << error::value_name(name) );
    return iter->second;
}

void vanilla::context::set_value(std::string name, object::ptr v)
{
    if(!_locals.empty())
        _locals.back()[std::move(name)] = std::move(v);
    else
        _globals[std::move(name)] = std::move(v);
}

void vanilla::context::set_global_value(std::string name, object::ptr v)
{
    _globals[std::move(name)] = std::move(v);
}

void vanilla::context::set_local_value(std::string name, object::ptr v)
{
    assert(!_locals.empty());
    _locals.back()[std::move(name)] = std::move(v);
}

void vanilla::context::begin_stackframe()
{
    _locals.resize(_locals.size() + 1);
}

void vanilla::context::end_stackframe()
{
    assert(!_locals.empty());
    _locals.pop_back();
}