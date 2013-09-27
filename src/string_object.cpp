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
#include <vanilla/string_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::string_object
///////////////////////////////////////////////////////////////////////////

vanilla::string_object::string_object(string_type v)
    : _v(std::move(v))
{ }

vanilla::string_object::string_type const& vanilla::string_object::value() const
{
    return _v;
}
        
vanilla::object_type_id vanilla::string_object::type_id() const
{
    return OBJECT_ID_STRING;
}

vanilla::object::ptr vanilla::string_object::type_name() const
{
    return allocate_object<string_object>("string");
}
        
vanilla::object::ptr vanilla::string_object::clone() const
{
    return allocate_object<string_object>(_v);
}

vanilla::object::ptr vanilla::string_object::to_string() const
{
    return const_cast<string_object*>(this)->shared_from_this();
}

vanilla::object::ptr vanilla::string_object::concat(object::ptr const& other)
{
    return allocate_object<string_object>(_v +
        string_object_to_cpp_string(other->to_string()));
}

///////////////////////////////////////////////////////////////////////////
/////////// FREE FUNCTIONS
///////////////////////////////////////////////////////////////////////////

std::string const& vanilla::string_object_to_cpp_string(object::ptr const& obj)
{
    if(obj->type_id() != OBJECT_ID_STRING)
    {
        BOOST_THROW_EXCEPTION(error::bad_cast_error()
            << error::first_operand(obj)
            << error::cast_target_name("string"));
    }
    
    return static_cast<string_object const*>(obj.get())->value();
}