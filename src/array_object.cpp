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
#include <unordered_map>

// Vanilla:
#include <vanilla/array_object.hpp>
#include <vanilla/string_object.hpp>
#include <vanilla/int_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// UTILITY
///////////////////////////////////////////////////////////////////////////

namespace
{
    std::unordered_map
    <
        std::string,
        vanilla::object::ptr (*)(vanilla::array_object*)
    > const array_object_attributes_getter =
    {
        {   
            "length", [](vanilla::array_object* obj) -> vanilla::object::ptr
            {
                return vanilla::allocate_object<vanilla::int_object>(
                    obj->value().size());
            }
        },
    };
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::array_object
///////////////////////////////////////////////////////////////////////////
 
vanilla::array_object::array_object(array_type v)
    : _v(std::move(v))
{ }
        
vanilla::array_object::array_type const& vanilla::array_object::value() const
{
    return _v;
}
        
vanilla::object_type_id vanilla::array_object::type_id() const
{
    return OBJECT_ID_ARRAY;
}
        
vanilla::object::ptr vanilla::array_object::type_name() const
{
    return allocate_object<string_object>("array");
}
        
vanilla::object::ptr vanilla::array_object::copy(bool deep) const
{
    if(!deep)
        return allocate_object<array_object>(_v);
    
    array_type deep_copies;
    deep_copies.reserve(_v.size());
    for(object::ptr const& o : _v)
        deep_copies.emplace_back(o->copy(true));
    return allocate_object<array_object>(std::move(deep_copies));
}

vanilla::object::ptr vanilla::array_object::sget(object::ptr const& subscript)
{
    long index = int_object_to_signed_long(subscript->to_int());
    if(index < 0 || index >= _v.size())
        BOOST_THROW_EXCEPTION(error::invalid_index_error());
    return _v[index];
}

void vanilla::array_object::sset(object::ptr const& subscript, ptr value)
{
    long index = int_object_to_signed_long(subscript->to_int());
    if(index < 0 || index >= _v.size())
        BOOST_THROW_EXCEPTION(error::invalid_index_error());
    _v[index] = std::move(value);
}

vanilla::object::ptr vanilla::array_object::eget(std::string const& name)
{
    auto iter = array_object_attributes_getter.find(name);
    if(iter != array_object_attributes_getter.end())
        return iter->second(this);
    return object::eget(name);
}

void vanilla::array_object::eset(std::string const& name, ptr value)
{
    return object::eset(name, std::move(value));
}