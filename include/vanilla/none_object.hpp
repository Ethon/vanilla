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

#ifndef HEADER_UUID_A07870E0B2DE4747B06CDFACED1847B3
#define HEADER_UUID_A07870E0B2DE4747B06CDFACED1847B3

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{
    object_type_id const OBJECT_ID_NONE = 0x0;
    
    class none_object : public object
    {
    public:
        virtual object_type_id type_id() const override;
        
        virtual ptr type_name() const override;
        
        virtual ptr copy(bool deep) const override;
        
        virtual ptr to_string() const override;
    };
}

#endif // HEADER_UUID_A07870E0B2DE4747B06CDFACED1847B3