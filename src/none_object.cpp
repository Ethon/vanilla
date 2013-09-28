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
#include <vanilla/none_object.hpp>
#include <vanilla/string_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::none_object
///////////////////////////////////////////////////////////////////////////

vanilla::object_type_id vanilla::none_object::type_id() const
{
    return OBJECT_ID_NONE;
}
        
vanilla::object::ptr vanilla::none_object::type_name() const
{
    return allocate_object<string_object>("none");
}
        
vanilla::object::ptr vanilla::none_object::copy(bool) const
{
    return const_cast<none_object*>(this)->shared_from_this();
}
        
vanilla::object::ptr vanilla::none_object::to_string() const
{
    return allocate_object<string_object>("none");
}

