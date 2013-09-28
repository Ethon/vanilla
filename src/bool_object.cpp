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
#include <exception>

// Vanilla:
#include <vanilla/bool_object.hpp>
#include <vanilla/string_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::bool_object
///////////////////////////////////////////////////////////////////////////

vanilla::bool_object::bool_object(bool_type v)
    : _v(v)
{ }
        
vanilla::object_type_id vanilla::bool_object::type_id() const
{
    return OBJECT_ID_BOOL;
}

vanilla::object::ptr vanilla::bool_object::type_name() const
{
    return allocate_object<string_object>("bool");
}
        
vanilla::object::ptr vanilla::bool_object::copy(bool) const
{
    return allocate_object<bool_object>(_v);
}

vanilla::object::ptr vanilla::bool_object::to_string() const
{
    if(_v == true)
        return allocate_object<string_object>("true");
    else if(_v == false)
        return allocate_object<string_object>("false");
    else if(_v == boost::indeterminate)
        return allocate_object<string_object>("indeterminate");
    
    assert(false);
    std::terminate();
}

vanilla::object::ptr vanilla::bool_object::to_bool() const
{
    return const_cast<bool_object*>(this)->shared_from_this();
}

vanilla::bool_object::bool_type vanilla::bool_object::value() const
{
    return _v;
}

///////////////////////////////////////////////////////////////////////////
/////////// FREE FUNCTIONS
///////////////////////////////////////////////////////////////////////////

vanilla::bool_object::bool_type
vanilla::bool_object_to_cpp_bool(object::ptr const& obj)
{
    if(obj->type_id() != OBJECT_ID_BOOL)
    {
        BOOST_THROW_EXCEPTION(error::bad_cast_error()
            << error::first_operand(obj)
            << error::cast_target_name("bool"));
    }
    
    return static_cast<bool_object*>(obj.get())->value();
}