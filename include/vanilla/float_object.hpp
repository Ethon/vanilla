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

#ifndef HEADER_UUID_5FFA7546ADC34569B3893AD92C6C5B5A
#define HEADER_UUID_5FFA7546ADC34569B3893AD92C6C5B5A

// GMP:
#include <gmp.h>

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{
    object_type_id const OBJECT_ID_FLOAT = 0x2;
    
    class float_object : public object
    {
    public:
        class gmp_mpf_wrapper
        {
        private:
            mutable mpf_t _mpf;
            bool _valid;
            
        public:
            gmp_mpf_wrapper();
            gmp_mpf_wrapper(mpf_t);
            gmp_mpf_wrapper(unsigned long);
            gmp_mpf_wrapper(signed long);
            gmp_mpf_wrapper(double);
            gmp_mpf_wrapper(mpz_t);
            gmp_mpf_wrapper(char*, int);
            gmp_mpf_wrapper(gmp_mpf_wrapper const&);
            gmp_mpf_wrapper(gmp_mpf_wrapper&&);
            ~gmp_mpf_wrapper();
            
            mpf_t& mpf() const;
        };
        
        typedef gmp_mpf_wrapper float_type;
        
    private:
        float_type _v;
        
    public:
        template<typename... Args>
        explicit float_object(Args&&... args)
            : _v(std::forward<Args>(args)...)
        { }
        
        virtual object_type_id type_id() const override;
        
        virtual ptr type_name() const override;
        
        virtual ptr copy(bool deep) const override;
        
        virtual ptr to_string() const override;
        
        float_type const& value() const;
    };
    
    namespace error
    {
        struct float_conversion_overflow_error : evaluation_error
        { };
        
        VANILLA_MAKE_ERRINFO(char const*, float_conversion_target_type)
    }
    
    float float_object_to_cpp_float(object::ptr const& obj);
    double float_object_to_double(object::ptr const& obj);
    long double float_object_to_longdouble(object::ptr const& obj);
}

#endif // HEADER_UUID_5FFA7546ADC34569B3893AD92C6C5B5A