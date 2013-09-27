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

#ifndef HEADER_UUID_74DF9C87EAB642029E9094BA63880C3A
#define HEADER_UUID_74DF9C87EAB642029E9094BA63880C3A

// Boost:
#include <boost/logic/tribool.hpp>

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{
    object_type_id const OBJECT_ID_BOOL = 0x3;
    
    class bool_object : public object
    {
    public:
        typedef boost::logic::tribool bool_type;
        
    private:
        bool_type _v;
        
    public:
        explicit bool_object(bool_type v);
        
        virtual object_type_id type_id() const override;
        
        virtual ptr type_name() const override;
        
        virtual ptr clone() const override;
        
        virtual ptr to_string() const override;
        virtual ptr to_bool() const override;
        
        bool_type value() const;
    };
    
    bool_object::bool_type bool_object_to_cpp_bool(object::ptr const& obj);
}

#endif // HEADER_UUID_74DF9C87EAB642029E9094BA63880C3A