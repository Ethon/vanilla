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

#ifndef HEADER_UUID_B49970A649644885B6A6798FA8E7867A
#define HEADER_UUID_B49970A649644885B6A6798FA8E7867A

// C++ Standard Library:
#include <string>
#include <functional>

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{    
    class function_argument
    {
    private:
        std::string _name;
        object::ptr _default_value;
        
    public:
        function_argument(  std::string name,
                            object::ptr default_value = object::ptr());
        
        std::string const& get_name() const;
        
        object::ptr get_default_value() const;
    };
    
    class context;
    
    class function_object : public object
    {
    public:
        typedef std::function<ptr (context&, ptr*, unsigned)> callable_type;
        
    private:
        std::string _name;
        std::vector<function_argument> _arguments;
        callable_type _f;
        bool _set_context;
        bool _variadic;
        unsigned _min_args;
        
    public:
        function_object(    std::string name,
                            std::vector<function_argument> arguments,
                            callable_type f,
                            bool set_context = true,
                            bool variadic = false);
        
        virtual object_type_id type_id() const override;
        
        virtual ptr type_name() const override;
        
        virtual ptr copy(bool deep) const override;
        
        virtual ptr to_string() const override;
        
        virtual ptr call(context& c, ptr* argv, unsigned argc) override;
    };
    
    namespace error
    {
        struct too_many_arguments_error : evaluation_error
        { };
        
        struct not_enough_arguments_error : evaluation_error
        { };
        
        struct missing_default_argument_error : evaluation_error
        { };
        
        VANILLA_MAKE_ERRINFO(std::string, function_name);
        VANILLA_MAKE_ERRINFO(std::string, argument_name);
        VANILLA_MAKE_ERRINFO(unsigned, num_arguments_expected);
        VANILLA_MAKE_ERRINFO(unsigned, num_arguments_received);
    }
}

#endif // HEADER_UUID_B49970A649644885B6A6798FA8E7867A