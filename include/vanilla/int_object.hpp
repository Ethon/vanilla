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

#ifndef HEADER_UUID_0E1F882948B04D27BA2094427AE9B18D
#define HEADER_UUID_0E1F882948B04D27BA2094427AE9B18D

// GMP:
#include <gmp.h>

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{
    object_type_id const OBJECT_ID_INTEGER = 0x1;
    
    class int_object : public object
    {
    public:
        class gmp_mpz_wrapper
        {
        private:
            mutable mpz_t _mpz;
            bool _valid;
            
        public:
            gmp_mpz_wrapper();
            gmp_mpz_wrapper(mpz_t);
            gmp_mpz_wrapper(unsigned long);
            gmp_mpz_wrapper(signed long);
            gmp_mpz_wrapper(double);
            gmp_mpz_wrapper(mpf_t);
            gmp_mpz_wrapper(char*, int);
            gmp_mpz_wrapper(gmp_mpz_wrapper const&);
            gmp_mpz_wrapper(gmp_mpz_wrapper&&);
            ~gmp_mpz_wrapper();
            
            mpz_t& mpz() const;
        };
        
        typedef gmp_mpz_wrapper int_type;
        
    private:
        int_type _v;
        
    public:
        template<typename... Args>
        explicit int_object(Args&&... args)
            : _v(std::forward<Args>(args)...)
        { }
        
        virtual object_type_id type_id() const override;
        
        virtual ptr type_name() const override;
        
        virtual ptr clone() const override;
        
        virtual ptr to_string() const override;
        virtual ptr to_int() const override;
        virtual ptr to_float() const override;
        
        int_type const& value() const;
        
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
    };
    
    namespace error
    {
        struct integer_conversion_overflow_error : evaluation_error
        { };
        
        VANILLA_MAKE_ERRINFO(char const*, integer_conversion_target_type)
    }
    
    unsigned char int_object_to_unsigned_char(object::ptr const& obj);
    unsigned short int_object_to_unsigned_short(object::ptr const& obj);
    unsigned int int_object_to_unsigned_int(object::ptr const& obj);
    unsigned long int_object_to_unsigned_long(object::ptr const& obj);
    unsigned long long int_object_to_unsigned_longlong(object::ptr const& obj);
    
    signed char int_object_to_signed_char(object::ptr const& obj);
    signed short int_object_to_signed_short(object::ptr const& obj);
    signed int int_object_to_signed_int(object::ptr const& obj);
    signed long int_object_to_signed_long(object::ptr const& obj);
    signed long long int_object_to_signed_longlong(object::ptr const& obj);
}

#endif // HEADER_UUID_0E1F882948B04D27BA2094427AE9B18D