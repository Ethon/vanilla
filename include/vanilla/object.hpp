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

#ifndef HEADER_UUID_3F8C2284A47547D1B07D56E764241868
#define HEADER_UUID_3F8C2284A47547D1B07D56E764241868 

// C++ Standard Library:
#include <memory>
#include <cstdint>

// Vanilla:
#include <vanilla/error.hpp>

namespace vanilla
{
    typedef std::uint32_t object_type_id;
    
    struct context;
    
    object_type_id const OBJECT_ID_NONE = 0x0;
    object_type_id const OBJECT_ID_INT = 0x1;
    object_type_id const OBJECT_ID_FLOAT = 0x2;
    object_type_id const OBJECT_ID_BOOL = 0x3;
    object_type_id const OBJECT_ID_STRING = 0x4;
    object_type_id const OBJECT_ID_ARRAY = 0x5;
    object_type_id const OBJECT_ID_TUPLE = 0x6;
    object_type_id const OBJECT_ID_DICT = 0x7;
    object_type_id const OBJECT_ID_HASH = 0x8;
    object_type_id const OBJECT_ID_FUNCTION = 0x9;
    object_type_id const OBJECT_ID_NATIVE_FUNCTION = 0xA;
    object_type_id const OBJECT_ID_MEMBER_FUNCTION = 0xB;
    object_type_id const OBJECT_ID_CLASS = 0xC;
    object_type_id const OBJECT_ID_CLASSFLAG = 1 << 31;
    
 
    class object : public std::enable_shared_from_this<object>
    {
    public:
        typedef std::shared_ptr<object> ptr;
        
        virtual ~object();
        
        virtual object_type_id type_id() const = 0;
        
        virtual ptr type_name() const = 0;
        
        virtual ptr copy(bool deep = false) const;
        
        virtual ptr to_string() const;
        virtual ptr to_int() const;
        virtual ptr to_float() const;
        virtual ptr to_bool() const;
        
        virtual ptr call(context& c, ptr* argv, unsigned argc);
        
        // Unary operations.
        virtual ptr neg();
        virtual ptr abs();
        
        // Arithmetic operations.
        virtual ptr add(object::ptr const& other);
        virtual ptr sub(object::ptr const& other);
        virtual ptr mul(object::ptr const& other);
        virtual ptr div(object::ptr const& other);
        
        // Relational operations.
        virtual ptr lt(object::ptr const& other);
        virtual ptr le(object::ptr const& other);
        virtual ptr gt(object::ptr const& other);
        virtual ptr ge(object::ptr const& other);
        
        // Equality operations.
        virtual ptr eq(object::ptr const& other);
        virtual ptr neq(object::ptr const& other);
        
        // Other.
        virtual ptr concat(object::ptr const& other);
        
        // Subscript.
        virtual ptr sget(object::ptr const& subscript);
        virtual void sset(object::ptr const& subscript, ptr value);
        
        // Element selection.
        virtual ptr eget(std::string const& name);
        virtual void eset(std::string const& name, ptr value);
    };
    
    template<typename T, typename... Args>
    object::ptr allocate_object(Args&&... args)
    {
        return std::make_shared<T>(std::forward<Args>(args)...);
    }
    
    namespace error
    {
        struct invalid_operation_error : evaluation_error
        { };
        
        struct bad_unary_operation_error : invalid_operation_error
        { };
        
        struct bad_binary_operation_error : invalid_operation_error
        { };
        
        struct bad_cast_error : invalid_operation_error
        { };
        
        struct value_not_callable_error : invalid_operation_error
        { };
        
        struct unsupported_operation_error : invalid_operation_error
        { };
        
        struct invalid_index_error : invalid_operation_error
        { };
        
        VANILLA_MAKE_ERRINFO(object::ptr, first_operand)
        VANILLA_MAKE_ERRINFO(object::ptr, second_operand)
        VANILLA_MAKE_ERRINFO(char const*, operation_name)
        VANILLA_MAKE_ERRINFO(std::string, cast_target_name)
        VANILLA_MAKE_ERRINFO(std::string, element_name)
    }
}

#endif // HEADER_UUID_3F8C2284A47547D1B07D56E764241868