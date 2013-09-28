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
#include <climits>
#include <unordered_map>

// Vanilla:
#include <vanilla/int_object.hpp>
#include <vanilla/string_object.hpp>
#include <vanilla/float_object.hpp>
#include <vanilla/bool_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// UTILITY
///////////////////////////////////////////////////////////////////////////

namespace
{
    std::unordered_map
    <
        std::string,
        vanilla::object::ptr (*)(vanilla::int_object*)
    > const int_object_attributes =
    {
        {   
            "int", [](vanilla::int_object* obj) -> vanilla::object::ptr
            {
                return obj->clone();
            }
        },
        
        {
            "float", [](vanilla::int_object* obj) -> vanilla::object::ptr
            {
                return vanilla::allocate_object<vanilla::float_object>(obj->value().mpz());
            }
        },
        
        {
            "string", [](vanilla::int_object* obj) -> vanilla::object::ptr
            {
                return obj->to_string();
            }
        },
        
        {
            "sqrt", [](vanilla::int_object* obj) -> vanilla::object::ptr
            {
                vanilla::float_object::float_type mpf(obj->value().mpz());
                vanilla::float_object::float_type result;
                mpf_sqrt(result.mpf(), mpf.mpf());
                return  vanilla::allocate_object<vanilla::float_object>(std::move(result));
            }
        },
    };
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::int_object::gmp_mpz_wrapper
///////////////////////////////////////////////////////////////////////////

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper()
    :   _mpz(), _valid(true)
{
    mpz_init(_mpz);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(mpz_t op)
    :   _mpz(), _valid(true)
{
    mpz_init(_mpz);
    mpz_set(_mpz, op);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(unsigned long op)
    :   _mpz(), _valid(true)
{
    mpz_init(_mpz);
    mpz_set_ui(_mpz, op);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(signed long op)
    :   _mpz(), _valid(true)
{
    mpz_init(_mpz);
    mpz_set_si(_mpz, op);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(double op)
    :   _mpz(), _valid(true)
{
    mpz_init(_mpz);
    mpz_set_d(_mpz, op);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(mpf_t op)
    :   _mpz(), _valid(true)
{
    mpz_init(_mpz);
    mpz_set_f(_mpz, op);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(char* str, int base)
    : _mpz(), _valid(true)
{
    mpz_init(_mpz);
    mpz_set_str(_mpz, str, base);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(gmp_mpz_wrapper const& op)
    :   _mpz(), _valid(true)
{
    mpz_init(_mpz);
    mpz_set(_mpz, op._mpz);
}

vanilla::int_object::gmp_mpz_wrapper::gmp_mpz_wrapper(gmp_mpz_wrapper&& op)
    :   _mpz(), _valid(true)
{
    std::memcpy(_mpz, op._mpz, sizeof(_mpz[0]));
    op._valid = false;
}

vanilla::int_object::gmp_mpz_wrapper::~gmp_mpz_wrapper()
{
    if(_valid)
        mpz_clear(_mpz);
}

mpz_t& vanilla::int_object::gmp_mpz_wrapper::mpz() const
{
    return _mpz;
}
            
///////////////////////////////////////////////////////////////////////////
/////////// vanilla::int_object
///////////////////////////////////////////////////////////////////////////
        
vanilla::object_type_id vanilla::int_object::type_id() const
{
    return OBJECT_ID_INTEGER;
}

vanilla::object::ptr vanilla::int_object::type_name() const
{
    return allocate_object<string_object>("int");
}
        
vanilla::object::ptr vanilla::int_object::clone() const
{
    return allocate_object<int_object>(_v);
}

vanilla::object::ptr vanilla::int_object::to_string() const
{
    std::string result;
    result.resize(mpz_sizeinbase(_v.mpz(), 10) + 2);
    mpz_get_str(&result[0], 10, _v.mpz());
    result.pop_back(); // Remove null terminator.
    return allocate_object<string_object>(std::move(result));
}

vanilla::object::ptr vanilla::int_object::to_int() const
{
    return const_cast<int_object*>(this)->shared_from_this();
}

vanilla::object::ptr vanilla::int_object::to_float() const
{
    return allocate_object<float_object>(_v.mpz());
}

vanilla::int_object::int_type const& vanilla::int_object::value() const
{
    return _v;
}

vanilla::object::ptr vanilla::int_object::neg()
{
    int_type result;
    mpz_neg(result.mpz(), _v.mpz());
    return allocate_object<int_object>(std::move(result));
}

vanilla::object::ptr vanilla::int_object::abs()
{
    int_type result;
    mpz_abs(result.mpz(), _v.mpz());
    return allocate_object<int_object>(std::move(result));
}
        
vanilla::object::ptr vanilla::int_object::add(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int_type result;
            mpz_add(result.mpz(), _v.mpz(), rhs->value().mpz());
            return allocate_object<int_object>(std::move(result));
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            float_object::float_type result;
            mpf_add(result.mpf(), lhs.mpf(), rhs->value().mpf());
            return allocate_object<float_object>(std::move(result));
        }
        
        default:
        {
            return object::add(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::sub(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int_type result;
            mpz_sub(result.mpz(), _v.mpz(), rhs->value().mpz());
            return allocate_object<int_object>(std::move(result));
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            float_object::float_type result;
            mpf_sub(result.mpf(), lhs.mpf(), rhs->value().mpf());
            return allocate_object<float_object>(std::move(result));
        }
        
        default:
        {
            return object::sub(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::mul(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int_type result;
            mpz_mul(result.mpz(), _v.mpz(), rhs->value().mpz());
            return allocate_object<int_object>(std::move(result));
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            float_object::float_type result;
            mpf_mul(result.mpf(), lhs.mpf(), rhs->value().mpf());
            return allocate_object<float_object>(std::move(result));
        }
        
        default:
        {
            return object::mul(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::div(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object::float_type rhs( (static_cast<int_object const*>(other.get())->value().mpz()) );
            float_object::float_type result;
            mpf_div(result.mpf(), lhs.mpf(), rhs.mpf());
            return allocate_object<float_object>(std::move(result));
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            float_object::float_type result;
            mpf_div(result.mpf(), lhs.mpf(), rhs->value().mpf());
            return allocate_object<float_object>(std::move(result));
        }
        
        default:
        {
            return object::div(other);
        }
    }
}
        
vanilla::object::ptr vanilla::int_object::lt(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int result = mpz_cmp(_v.mpz(), rhs->value().mpz());
            return allocate_object<bool_object>(result < 0);
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            return allocate_object<bool_object>(mpf_cmp(lhs.mpf(), rhs->value().mpf()) < 0);
        }
        
        default:
        {
            return object::lt(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::le(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int result = mpz_cmp(_v.mpz(), rhs->value().mpz());
            return allocate_object<bool_object>(result <= 0);
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            return allocate_object<bool_object>(mpf_cmp(lhs.mpf(), rhs->value().mpf()) <= 0);
        }
        
        default:
        {
            return object::le(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::gt(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int result = mpz_cmp(_v.mpz(), rhs->value().mpz());
            return allocate_object<bool_object>(result > 0);
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            return allocate_object<bool_object>(mpf_cmp(lhs.mpf(), rhs->value().mpf()) > 0);
        }
        
        default:
        {
            return object::gt(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::ge(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int result = mpz_cmp(_v.mpz(), rhs->value().mpz());
            return allocate_object<bool_object>(result >= 0);
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object::float_type lhs( (_v.mpz()) );
            float_object const* rhs = static_cast<float_object const*>(other.get());
            return allocate_object<bool_object>(mpf_cmp(lhs.mpf(), rhs->value().mpf()) >= 0);
        }
        
        default:
        {
            return object::ge(other);
        }
    }
}
        
vanilla::object::ptr vanilla::int_object::eq(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int result = mpz_cmp(_v.mpz(), rhs->value().mpz());
            return allocate_object<bool_object>(result == 0);
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object const* rhs = static_cast<float_object const*>(other.get());
            if(!mpf_integer_p(rhs->value().mpf()))
                return allocate_object<bool_object>(false);
            
            float_object::float_type lhs( (_v.mpz()) );
            return allocate_object<bool_object>(mpf_cmp(lhs.mpf(), rhs->value().mpf()) == 0);
        }
        
        default:
        {
            return object::ge(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::neq(object::ptr const& other)
{
    switch(other->type_id())
    {
        case OBJECT_ID_INTEGER:
        {
            int_object const* rhs = static_cast<int_object const*>(other.get());
            int result = mpz_cmp(_v.mpz(), rhs->value().mpz());
            return allocate_object<bool_object>(result != 0);
        }
        
        case OBJECT_ID_FLOAT:
        {
            float_object const* rhs = static_cast<float_object const*>(other.get());
            if(!mpf_integer_p(rhs->value().mpf()))
                return allocate_object<bool_object>(true);
            
            float_object::float_type lhs( (_v.mpz()) );
            return allocate_object<bool_object>(mpf_cmp(lhs.mpf(), rhs->value().mpf()) != 0);
        }
        
        default:
        {
            return object::ge(other);
        }
    }
}

vanilla::object::ptr vanilla::int_object::eget(std::string const& name)
{
    auto iter = int_object_attributes.find(name);
    if(iter == int_object_attributes.end())
        return object::eget(name);
    
    return iter->second(this);
}

void vanilla::int_object::eset(std::string const& name, ptr value)
{
    return object::eset(name, std::move(value));
}

///////////////////////////////////////////////////////////////////////////
/////////// FREE FUNCTIONS
///////////////////////////////////////////////////////////////////////////

#pragma clang diagnostic push
#pragma GCC diagnostic ignored "-Wtautological-compare"

template<typename T, typename LongGetter>
T int_object_to_small(vanilla::object::ptr const& obj, char const* name, LongGetter get_long)
{
    static_assert(sizeof(T) < sizeof(long), "");
    
    auto temp = get_long(obj);
    if(temp > std::numeric_limits<T>::max() || temp < std::numeric_limits<T>::min())
    {
        BOOST_THROW_EXCEPTION(vanilla::error::integer_conversion_overflow_error()
            << vanilla::error::first_operand(obj)
            << vanilla::error::integer_conversion_target_type(name));
    }
    
    return static_cast<T>(temp);
}

#pragma clang diagnostic pop

unsigned char vanilla::int_object_to_unsigned_char(object::ptr const& obj)
{
    return int_object_to_small<unsigned char>(
        obj, "unsigned char", int_object_to_unsigned_long);
}

unsigned short vanilla::int_object_to_unsigned_short(object::ptr const& obj)
{
    return int_object_to_small<unsigned short>(
        obj, "unsigned short", int_object_to_unsigned_long);
}

unsigned int vanilla::int_object_to_unsigned_int(object::ptr const& obj)
{
    return int_object_to_small<unsigned int>(
        obj, "unsigned int", int_object_to_unsigned_long);
}

unsigned long vanilla::int_object_to_unsigned_long(object::ptr const& obj)
{
    if(obj->type_id() != OBJECT_ID_INTEGER)
    {
        BOOST_THROW_EXCEPTION(error::bad_cast_error()
            << error::first_operand(obj)
            << error::cast_target_name("int"));
    }
    
    mpz_t& mpz = static_cast<int_object const*>(obj.get())->value().mpz();
    if(!mpz_fits_ulong_p(mpz) || mpz_sgn(mpz) == -1)
    {
        BOOST_THROW_EXCEPTION(error::integer_conversion_overflow_error()
            << error::first_operand(obj)
            << error::integer_conversion_target_type("unsigned long"));
    }
    return mpz_get_ui(mpz);
}

unsigned long long vanilla::int_object_to_unsigned_longlong(object::ptr const& obj)
{
#if ULLONG_MAX == ULONG_MAX
    return int_object_to_unsigned_long(obj);
#else
    
    if(obj->type_id() != OBJECT_ID_INTEGER)
    {
        BOOST_THROW_EXCEPTION(error::bad_cast_error()
            << error::first_operand(obj)
            << error::cast_target_name("int"));
    }
    
    mpz_t& mpz = static_cast<int_object const*>(obj.get())->value().mpz();
    if(mpz_sizeinbase(mpz, 2) > sizeof(long long) * CHAR_BIT || mpz_sgn(mpz) == -1)
    {
        BOOST_THROW_EXCEPTION(error::integer_conversion_overflow_error()
            << error::first_operand(obj)
            << error::integer_conversion_target_type("unsigned long long"));
    }
    
    size_t count = 0;
    unsigned long long result = 0;
    mpz_export(&result, &count, -1, sizeof(result), 0, 0, mpz);
    assert(count == 1);
    return result;
#endif
}

signed char vanilla::int_object_to_signed_char(object::ptr const& obj)
{
    return int_object_to_small<signed char>(
        obj, "signed char", int_object_to_signed_long);
}

signed short vanilla::int_object_to_signed_short(object::ptr const& obj)
{
    return int_object_to_small<signed short>(
        obj, "signed short", int_object_to_signed_long);
}

signed int vanilla::int_object_to_signed_int(object::ptr const& obj)
{
    return int_object_to_small<signed int>(
        obj, "signed int", int_object_to_signed_long);
}

signed long vanilla::int_object_to_signed_long(object::ptr const& obj)
{
    if(obj->type_id() != OBJECT_ID_INTEGER)
    {
        BOOST_THROW_EXCEPTION(error::bad_cast_error()
            << error::first_operand(obj)
            << error::cast_target_name("int"));
    }
    
    mpz_t& mpz = static_cast<int_object const*>(obj.get())->value().mpz();
    if(!mpz_fits_slong_p(mpz))
    {
        BOOST_THROW_EXCEPTION(error::integer_conversion_overflow_error()
            << error::first_operand(obj)
            << error::integer_conversion_target_type("signed long"));
    }
    return mpz_get_si(mpz);
}

signed long long vanilla::int_object_to_signed_longlong(object::ptr const& obj)
{
#if LLONG_MAX == LONG_MAX
    return int_object_to_signed_long(obj);
#endif
    
    if(obj->type_id() != OBJECT_ID_INTEGER)
    {
        BOOST_THROW_EXCEPTION(error::bad_cast_error()
            << error::first_operand(obj)
            << error::cast_target_name("int"));
    }
    
    mpz_t& mpz = static_cast<int_object const*>(obj.get())->value().mpz();
    if(mpz_sizeinbase(mpz, 2) + 1 > sizeof(long long) * CHAR_BIT)
    {
        BOOST_THROW_EXCEPTION(error::integer_conversion_overflow_error()
            << error::first_operand(obj)
            << error::integer_conversion_target_type("signed long long"));
    }
    
    size_t count = 0;
    long long result = 0;
    mpz_export(&result, &count, -1, sizeof(result), 0, 0, mpz);
    assert(count == 1);
    return result;
}