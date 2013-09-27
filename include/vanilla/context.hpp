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

#ifndef HEADER_UUID_D867F1EB89C14CC2BA7E9603CADFDFAB
#define HEADER_UUID_D867F1EB89C14CC2BA7E9603CADFDFAB

// C++ Standard Library:
#include <unordered_map>
#include <vector>
#include <string>
#include <stdexcept>

// Vanilla:
#include <vanilla/object.hpp>
#include <vanilla/error.hpp>

namespace vanilla
{
    namespace error
    {
        struct undefined_value_error : evaluation_error
        { };
        
        VANILLA_MAKE_ERRINFO(std::string, value_name)
    }
    
    class context
    {
    private:
        std::unordered_map<std::string, object::ptr> _globals;
        std::vector<std::unordered_map<std::string, object::ptr>> _locals;
        
    public:
        context();
        
        object::ptr get_value(std::string const& name) const;
        object::ptr get_global_value(std::string const& name) const;
        object::ptr get_local_value(std::string const& name) const;
        
        void set_value(std::string name, object::ptr v);
        void set_global_value(std::string name, object::ptr v);
        void set_local_value(std::string name, object::ptr v);
        
        void begin_stackframe();
        void end_stackframe();
    };
    
    namespace detail
    {
        struct function_returned
        {
            object::ptr result;
        };
    }
}

#endif // HEADER_UUID_D867F1EB89C14CC2BA7E9603CADFDFAB