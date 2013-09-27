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

// C++ Standard Library:
#include <cstring>
#include <cstdlib>
#include <limits>

// Vanilla:
#include <vanilla/float_object.hpp>
#include <vanilla/string_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::float_object
///////////////////////////////////////////////////////////////////////////

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper()
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(mpf_t op)
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
    mpf_set(_mpf, op);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(unsigned long op)
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
    mpf_set_ui(_mpf, op);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(signed long op)
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
    mpf_set_si(_mpf, op);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(double op)
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
    mpf_set_d(_mpf, op);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(mpz_t op)
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
    mpf_set_z(_mpf, op);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(char* str, int base)
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
    int result = mpf_set_str(_mpf, str, base);
    assert(result == 0);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(gmp_mpf_wrapper const& op)
    : _mpf(), _valid(true)
{
    mpf_init(_mpf);
    mpf_set(_mpf, op._mpf);
}

vanilla::float_object::gmp_mpf_wrapper::gmp_mpf_wrapper(gmp_mpf_wrapper&& op)
    : _mpf(), _valid(true)
{
    std::memcpy(_mpf, op._mpf, sizeof(_mpf[0]));
    op._valid = false;
}

vanilla::float_object::gmp_mpf_wrapper::~gmp_mpf_wrapper()
{
    if(_valid)
        mpf_clear(_mpf);
}
            
mpf_t& vanilla::float_object::gmp_mpf_wrapper::mpf() const
{
    return _mpf;
}
            
///////////////////////////////////////////////////////////////////////////
/////////// vanilla::float_object
///////////////////////////////////////////////////////////////////////////
        
vanilla::object_type_id vanilla::float_object::type_id() const
{
    return OBJECT_ID_FLOAT;
}

vanilla::object::ptr vanilla::float_object::type_name() const
{
    return allocate_object<string_object>("float");
}
        
vanilla::object::ptr vanilla::float_object::clone() const
{
    return allocate_object<float_object>(_v);
}

vanilla::object::ptr vanilla::float_object::to_string() const
{
    mp_exp_t exp;
    char* str = mpf_get_str(nullptr, &exp, 10, 0, _v.mpf());
    std::unique_ptr<char, void (*)(char*)> tmp(str, [](char* ptr) { std::free(ptr); } );
    
    if(exp > 0)
    {
        std::string result(str, str + exp);
        result += '.';
        for(char* s = str + exp; *s; ++s)
            result += *s;
        return allocate_object<string_object>(std::move(result));
    }
    else
    {
       std::string result("0.");
       for(mp_exp_t i = 0; i < exp; ++i)
           result += '0';
       result.append(str);
       return allocate_object<string_object>(std::move(result));
    }
}

vanilla::float_object::float_type const& vanilla::float_object::value() const
{
    return _v;
}

///////////////////////////////////////////////////////////////////////////
/////////// FREE FUNCTIONS
///////////////////////////////////////////////////////////////////////////

float vanilla::float_object_to_cpp_float(object::ptr const& obj)
{
    double temp = float_object_to_double(obj);
    if(temp > std::numeric_limits<float>::max() ||
        temp < std::numeric_limits<float>::min())
    {
        BOOST_THROW_EXCEPTION(error::float_conversion_overflow_error()
            << error::first_operand(obj)
            << error::float_conversion_target_type("double"));
    }
    
    return static_cast<float>(temp);
}

double vanilla::float_object_to_double(object::ptr const& obj)
{
    if(obj->type_id() != OBJECT_ID_FLOAT)
    {
        BOOST_THROW_EXCEPTION(error::bad_cast_error()
            << error::first_operand(obj)
            << error::cast_target_name("float"));
    }
    
    mpf_t& mpf = static_cast<float_object const*>(obj.get())->value().mpf();
    double result = mpf_get_d(mpf);
    if(std::numeric_limits<double>::has_infinity &&
        result == std::numeric_limits<double>::infinity())
    {
        BOOST_THROW_EXCEPTION(error::float_conversion_overflow_error()
            << error::first_operand(obj)
            << error::float_conversion_target_type("double"));
    }
    
    return result;
}

long double vanilla::float_object_to_longdouble(object::ptr const& obj)
{
    return float_object_to_double(obj);
}