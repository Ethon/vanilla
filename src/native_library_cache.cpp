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

// System:
#include <dlfcn.h>

// Vanilla:
#include <vanilla/native_library_cache.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// UTILITY
///////////////////////////////////////////////////////////////////////////

namespace
{
    vanilla::native_library_cache::fptr retrieve_function_symbol(
        std::shared_ptr<void>& handle, std::string const& name)
    {
        dlerror();
        void* addr = dlsym(handle.get(), name.c_str());
        char const* error = dlerror();
        if(error)
        {
            BOOST_THROW_EXCEPTION(vanilla::error::native_symbol_not_found_error()
                << vanilla::error::dlerror_string(std::string(error))
                << vanilla::error::native_symbol_name(name));
        }
        
        static_assert(sizeof(void*) == sizeof(void(*)()), "sizeof(void*) != sizeof(void(*)())");
        return reinterpret_cast<vanilla::native_library_cache::fptr>(addr);
    }
    
    std::shared_ptr<void> load_library(std::string const& name)
    {
        void* handle = dlopen(name.c_str(), RTLD_LAZY);
        if(handle == 0)
        {
            BOOST_THROW_EXCEPTION(vanilla::error::native_library_loading_error()
                << vanilla::error::dlerror_string(std::string(dlerror()))
                << vanilla::error::native_library_name(name));
        }
        
        return std::shared_ptr<void>(handle, dlclose);
    }
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::native_library_cache
///////////////////////////////////////////////////////////////////////////

vanilla::native_library_cache& vanilla::native_library_cache::get()
{
    static native_library_cache instance;
    return instance;
}
        
vanilla::native_library_cache::fptr vanilla::native_library_cache::get_function(
    std::string const& library, std::string const& name)
{
    // If the library is not already cached then try to load it.
    auto iter = _cache.find(library);
    if(iter == _cache.end())
        iter = _cache.insert(std::make_pair(library, load_library(library))).first;
    
    // Load the function from the library.
    try
    {
        return retrieve_function_symbol(iter->second, name);
    }
    catch(error::native_symbol_not_found_error& e)
    {
        BOOST_THROW_EXCEPTION(e << error::native_library_name(library));
    }
}
        
void vanilla::native_library_cache::update()
{
    // Delete every module in the cache which isn't referenced anymore.
    for(auto iter = _cache.begin(); iter != _cache.end(); ++iter)
    {
        if(iter->second.unique())
        {
            iter = _cache.erase(iter);
            if(iter == _cache.end())
                return;
        }
    }
}