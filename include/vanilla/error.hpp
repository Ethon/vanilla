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

#ifndef HEADER_UUID_8ECDF5608B064079AAF9FBE094BCFC47
#define HEADER_UUID_8ECDF5608B064079AAF9FBE094BCFC47

// C++ Standard Library:
#include <exception>
#include <string>

// Boost:
#include <boost/exception/all.hpp>

namespace vanilla
{
    namespace error
    {
        struct base_error : virtual boost::exception, virtual std::exception
        { };
        
        struct scanner_error : base_error
        { };
        
        struct parser_error : base_error
        { };
        
        struct evaluation_error : base_error
        { };
        
        #define VANILLA_MAKE_ERRINFO(type, name) \
            typedef boost::error_info<struct tag_##name, type> name; \
            inline auto get_##name(base_error const& e) -> decltype(boost::get_error_info<name>(e)) \
            { return boost::get_error_info<name>(e); }
            
        VANILLA_MAKE_ERRINFO(std::string, error_string)
        VANILLA_MAKE_ERRINFO(unsigned, line_info)
        VANILLA_MAKE_ERRINFO(unsigned, pos_info)
    }
}

#endif // HEADER_UUID_8ECDF5608B064079AAF9FBE094BCFC47