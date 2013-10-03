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

#ifndef HEADER_UUID_E9DE315493894E35B07193390513AD78
#define HEADER_UUID_E9DE315493894E35B07193390513AD78

// C++ Standard Library:
#include <vector>

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{
    class array_object : public object
    {
    public:
        typedef std::vector<object::ptr> array_type;
        
    private:
        array_type _v;
        
    public:
        explicit array_object(array_type v);
        
        array_type const& value() const;
        
        virtual object_type_id type_id() const override;
        virtual ptr type_name() const override;
        
        virtual ptr copy(bool deep = false) const override;
        
        // Subscript.
        virtual ptr sget(object::ptr const& subscript);
        virtual void sset(object::ptr const& subscript, ptr value);
        
        // Element selection.
        virtual ptr eget(std::string const& name);
        virtual void eset(std::string const& name, ptr value);
    };
}

#endif // HEADER_UUID_E9DE315493894E35B07193390513AD78