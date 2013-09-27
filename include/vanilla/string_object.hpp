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

#ifndef HEADER_UUID_1DD4B72D0EF4494E884EB43C4103E74D
#define HEADER_UUID_1DD4B72D0EF4494E884EB43C4103E74D

// C++ Standard Library:
#include <string>

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{
    object_type_id const OBJECT_ID_STRING = 0x4;
    
    class string_object : public object
    {
    public:
        typedef std::string string_type;
        
    private:
        string_type _v;
        
    public:
        explicit string_object(string_type v);
        
        string_type const& value() const;
        
        virtual object_type_id type_id() const override;
        virtual ptr type_name() const override;
        
        virtual ptr clone() const override;
        
        virtual ptr to_string() const override;
        
        // Other.
        virtual ptr concat(object::ptr const& other);
    };
    
    std::string const& string_object_to_cpp_string(object::ptr const& obj);
}

#endif // HEADER_UUID_1DD4B72D0EF4494E884EB43C4103E74D