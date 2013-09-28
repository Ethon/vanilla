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
#include <map>

// Vanilla:
#include <vanilla/native_function_object.hpp>
#include <vanilla/native_library_cache.hpp>
#include <vanilla/none_object.hpp>
#include <vanilla/int_object.hpp>
#include <vanilla/string_object.hpp>
#include <vanilla/float_object.hpp>
#include <vanilla/function_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// UTILITY
///////////////////////////////////////////////////////////////////////////

namespace
{
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique(Args&&... args)
    {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
    
#define DEFINE_PRIMITIVE_RESULT_CONVERTER(name, native, native_type, vanilla_type) \
    struct ffi_result_converter_##name : public vanilla::detail::ffi_result_converter \
    { \
        virtual vanilla::object::ptr convert( \
            vanilla::detail::native_datatype const& in) override \
        { \
            return vanilla::allocate_object<vanilla::vanilla_type>((native_type)(in.native)); \
        } \
    }
    
    DEFINE_PRIMITIVE_RESULT_CONVERTER(ui8, ui8, unsigned long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(ui16, ui16, unsigned long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(ui32, ui32, unsigned long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(ui64, ui64, unsigned long, int_object);

    DEFINE_PRIMITIVE_RESULT_CONVERTER(i8, i8, signed long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(i16, i16, signed long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(i32, i32, signed long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(i64, i64, signed long, int_object);
    
    DEFINE_PRIMITIVE_RESULT_CONVERTER(f, f, double, float_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(d, d, double, float_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(ld, ld, double, float_object);
    
    DEFINE_PRIMITIVE_RESULT_CONVERTER(uc, uc, unsigned long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(us, us, unsigned long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(ui, ui, unsigned long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(ul, ul, unsigned long, int_object);
    
    DEFINE_PRIMITIVE_RESULT_CONVERTER(c, c, signed long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(s, s, signed long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(i, i, signed long, int_object);
    DEFINE_PRIMITIVE_RESULT_CONVERTER(l, l, signed long, int_object);
    
    class ffi_result_converter_void : public vanilla::detail::ffi_result_converter
    {
    public:
        virtual vanilla::object::ptr convert(vanilla::detail::native_datatype const&) override
        {
            return vanilla::allocate_object<vanilla::none_object>();
        }
    };
    
    class ffi_result_converter_string8 : public vanilla::detail::ffi_result_converter
    {
    public:
        virtual vanilla::object::ptr convert(vanilla::detail::native_datatype const& in) override
        {
            return vanilla::allocate_object<vanilla::string_object>(
                static_cast<char const*>(in.p));
        }
    };
    
#define DEFINE_PRIMITIVE_ARG_CONVERTER(name, native, accessor, extractor) \
    struct ffi_argument_converter_##name : public vanilla::detail::ffi_argument_converter \
    { \
        virtual vanilla::detail::native_datatype convert(vanilla::object::ptr const& in) override \
        { \
            vanilla::detail::native_datatype result; \
            result.native = vanilla::extractor(in->accessor()); \
            return result; \
        } \
        \
        virtual bool has_state() const override \
        { \
            return false; \
        } \
    }
    
    static_assert(sizeof(std::int8_t) == sizeof(char), "");
    static_assert(sizeof(std::int16_t) == sizeof(short), "");
    static_assert(sizeof(std::int32_t) == sizeof(int), "");
    static_assert(sizeof(std::int64_t) == sizeof(long long), "");
    
    DEFINE_PRIMITIVE_ARG_CONVERTER(ui8, ui8, to_int, int_object_to_unsigned_char);
    DEFINE_PRIMITIVE_ARG_CONVERTER(ui16, ui16, to_int, int_object_to_unsigned_short);
    DEFINE_PRIMITIVE_ARG_CONVERTER(ui32, ui32, to_int, int_object_to_unsigned_int);
    DEFINE_PRIMITIVE_ARG_CONVERTER(ui64, ui64, to_int, int_object_to_unsigned_longlong);
    
    DEFINE_PRIMITIVE_ARG_CONVERTER(i8, i8, to_int, int_object_to_signed_char);
    DEFINE_PRIMITIVE_ARG_CONVERTER(i16, i16, to_int, int_object_to_signed_short);
    DEFINE_PRIMITIVE_ARG_CONVERTER(i32, i32, to_int, int_object_to_signed_int);
    DEFINE_PRIMITIVE_ARG_CONVERTER(i64, i64, to_int, int_object_to_signed_longlong);
    
    DEFINE_PRIMITIVE_ARG_CONVERTER(f, f, to_float, float_object_to_cpp_float);
    DEFINE_PRIMITIVE_ARG_CONVERTER(d, d, to_float, float_object_to_double);
    DEFINE_PRIMITIVE_ARG_CONVERTER(ld, ld, to_float, float_object_to_longdouble);
    
    DEFINE_PRIMITIVE_ARG_CONVERTER(uc, uc, to_int, int_object_to_unsigned_char);
    DEFINE_PRIMITIVE_ARG_CONVERTER(us, us, to_int, int_object_to_unsigned_short);
    DEFINE_PRIMITIVE_ARG_CONVERTER(ui, ui, to_int, int_object_to_unsigned_int);
    DEFINE_PRIMITIVE_ARG_CONVERTER(ul, ul, to_int, int_object_to_unsigned_long);
    
    DEFINE_PRIMITIVE_ARG_CONVERTER(c, c, to_int, int_object_to_signed_char);
    DEFINE_PRIMITIVE_ARG_CONVERTER(s, s, to_int, int_object_to_signed_short);
    DEFINE_PRIMITIVE_ARG_CONVERTER(i, i, to_int, int_object_to_signed_int);
    DEFINE_PRIMITIVE_ARG_CONVERTER(l, l, to_int, int_object_to_signed_long);
    
    class ffi_argument_converter_string8 : public vanilla::detail::ffi_argument_converter
    {
    private:
        std::string _string8;
        
    public:
        virtual vanilla::detail::native_datatype convert(vanilla::object::ptr const& in) override
        {
            vanilla::detail::native_datatype result;
            _string8 = vanilla::string_object_to_cpp_string(in->to_string());
            result.p =  const_cast<char*>(_string8.c_str());
            return result;
        }
        
        virtual bool has_state() const override
        {
            return true;
        }
    };
    
    ffi_type* lookup_ffi_type(std::string const& name)
    {
        static std::map<std::string, ffi_type*> map =
        {
            { "void", &ffi_type_void },
            
            { "uint8", &ffi_type_uint8 },
            { "uint16", &ffi_type_uint16 },
            { "uint32", &ffi_type_uint32 },
            { "uint64", &ffi_type_uint64 },
            
            { "int8", &ffi_type_sint8 },
            { "int16", &ffi_type_sint16 },
            { "int32", &ffi_type_sint32 },
            { "int64", &ffi_type_sint64 },
            
            { "float", &ffi_type_float },
            { "double", &ffi_type_double },
            { "long double", &ffi_type_longdouble },
            
            { "unsigned char", &ffi_type_uchar },
            { "unsigned short", &ffi_type_ushort },
            { "unsigned int", &ffi_type_uint },
            { "unsigned long", &ffi_type_ulong },
            
            { "char", &ffi_type_schar },
            { "short", &ffi_type_sshort },
            { "int", &ffi_type_sint },
            { "long", &ffi_type_slong },
            
            { "string8", &ffi_type_pointer },
            { "const char*", &ffi_type_pointer }
        };
        
        auto iter = map.find(name);
        if(iter == map.end())
        {
            BOOST_THROW_EXCEPTION(vanilla::error::unknown_native_type_name_error()
                << vanilla::error::native_type_name(name));
        }
        
        return iter->second;
    }
    
#define RETURN_RESULT_CONVERTER_IF_NAME(namestr, rc_name) \
    if(name == namestr) return make_unique<ffi_result_converter_##rc_name>()
    
    std::unique_ptr<vanilla::detail::ffi_result_converter> create_result_converter(std::string const& name)
    {
        RETURN_RESULT_CONVERTER_IF_NAME("void", void);
                    
        RETURN_RESULT_CONVERTER_IF_NAME("uint8", ui8);
        RETURN_RESULT_CONVERTER_IF_NAME("uint16", ui16);
        RETURN_RESULT_CONVERTER_IF_NAME("uint32", ui32);
        RETURN_RESULT_CONVERTER_IF_NAME("uint64", ui64);
                    
        RETURN_RESULT_CONVERTER_IF_NAME("int8", i8);
        RETURN_RESULT_CONVERTER_IF_NAME("int16", i16);
        RETURN_RESULT_CONVERTER_IF_NAME("int32", i32);
        RETURN_RESULT_CONVERTER_IF_NAME("int64", i64);
                    
        RETURN_RESULT_CONVERTER_IF_NAME("float", f);
        RETURN_RESULT_CONVERTER_IF_NAME("double", d);
        RETURN_RESULT_CONVERTER_IF_NAME("long double", ld);
                    
        RETURN_RESULT_CONVERTER_IF_NAME("unsigned char", uc);
        RETURN_RESULT_CONVERTER_IF_NAME("unsigned short", us);
        RETURN_RESULT_CONVERTER_IF_NAME("unsigned int", ui);
        RETURN_RESULT_CONVERTER_IF_NAME("unsigned long", ul);
                    
        RETURN_RESULT_CONVERTER_IF_NAME("char", c);
        RETURN_RESULT_CONVERTER_IF_NAME("short", s);
        RETURN_RESULT_CONVERTER_IF_NAME("int", i);
        RETURN_RESULT_CONVERTER_IF_NAME("long", l);
                    
        RETURN_RESULT_CONVERTER_IF_NAME("string8", string8);
        RETURN_RESULT_CONVERTER_IF_NAME("const char*", string8);

        BOOST_THROW_EXCEPTION(vanilla::error::unknown_native_type_name_error()
            << vanilla::error::native_type_name(name));
    }
    
#define RETURN_ARG_CONVERTER_IF_NAME(namestr, rc_name) \
    if(name == namestr) return make_unique<ffi_argument_converter_##rc_name>()
        
    std::unique_ptr<vanilla::detail::ffi_argument_converter> create_argument_converter(std::string const& name)
    {
        RETURN_ARG_CONVERTER_IF_NAME("uint8", ui8);
        RETURN_ARG_CONVERTER_IF_NAME("uint16", ui16);
        RETURN_ARG_CONVERTER_IF_NAME("uint32", ui32);
        RETURN_ARG_CONVERTER_IF_NAME("uint64", ui64);
                    
        RETURN_ARG_CONVERTER_IF_NAME("int8", i8);
        RETURN_ARG_CONVERTER_IF_NAME("int16", i16);
        RETURN_ARG_CONVERTER_IF_NAME("int32", i32);
        RETURN_ARG_CONVERTER_IF_NAME("int64", i64);
                    
        RETURN_ARG_CONVERTER_IF_NAME("float", f);
        RETURN_ARG_CONVERTER_IF_NAME("double", d);
        RETURN_ARG_CONVERTER_IF_NAME("long double", ld);
                    
        RETURN_ARG_CONVERTER_IF_NAME("unsigned char", uc);
        RETURN_ARG_CONVERTER_IF_NAME("unsigned short", us);
        RETURN_ARG_CONVERTER_IF_NAME("unsigned int", ui);
        RETURN_ARG_CONVERTER_IF_NAME("unsigned long", ul);
                    
        RETURN_ARG_CONVERTER_IF_NAME("char", c);
        RETURN_ARG_CONVERTER_IF_NAME("short", s);
        RETURN_ARG_CONVERTER_IF_NAME("int", i);
        RETURN_ARG_CONVERTER_IF_NAME("long", l);
                    
        RETURN_ARG_CONVERTER_IF_NAME("string8", string8);
        RETURN_ARG_CONVERTER_IF_NAME("const char*", string8);
        
        if(name == "void")
            BOOST_THROW_EXCEPTION(vanilla::error::void_as_argument_type_error());

        BOOST_THROW_EXCEPTION(vanilla::error::unknown_native_type_name_error()
            << vanilla::error::native_type_name(name));
    }
}

vanilla::detail::ffi_result_converter::~ffi_result_converter()
{ }

vanilla::detail::ffi_argument_converter::~ffi_argument_converter()
{ }

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::native_function_object
///////////////////////////////////////////////////////////////////////////

vanilla::native_function_object::native_function_object(
            std::string name,
            std::string library,
            std::string result,
            std::vector<std::string> args,
            ffi_abi abi)
    :   _name(std::move(name)),
        _library(std::move(library)),
        _result(std::move(result)),
        _args(std::move(args)),
        _abi(abi),
        _cif(),
        _function(native_library_cache::get().get_function(_library, _name)), 
        _ffiargs(new ffi_type*[_args.size()]),
        _argument_converter(),
        _result_converter(create_result_converter(_result)),
        _lock(),
        _requires_locking(false)
{
    // Fill ffiargs.
    for(unsigned i = 0; i < _args.size(); ++i)
        _ffiargs[i] = lookup_ffi_type(_args[i]);
    
    // Create the cif.
    ffi_status s = ffi_prep_cif(&_cif, abi, _args.size(),
        lookup_ffi_type(_result), &_ffiargs[0]);
    assert(s == FFI_OK);
    
    // Fill argument converters and check if we need to lock on call.
    _argument_converter.reserve(_args.size());
    for(unsigned i = 0; i < _args.size(); ++i)
    {
        _argument_converter.push_back(create_argument_converter(_args[i]));
        _requires_locking =  _requires_locking || _argument_converter.back()->has_state();
    }
}

vanilla::native_function_object::~native_function_object()
{
    native_library_cache::get().update();
}

vanilla::object_type_id vanilla::native_function_object::type_id() const
{
    return OBJECT_ID_NATIVE_FUNCTION;
}
        
vanilla::object::ptr vanilla::native_function_object::type_name() const
{
    return allocate_object<string_object>("native function");
}
        
vanilla::object::ptr vanilla::native_function_object::copy(bool) const
{
    return allocate_object<native_function_object>(
        _name, _library, _result, _args, _abi);
}
        
vanilla::object::ptr vanilla::native_function_object::to_string() const
{
    std::string result;
    result += "<native function '";
    result += _name;
    result += "' from '";
    result += _library;
    result += "'>";
    return allocate_object<string_object>(std::move(result));
}

vanilla::object::ptr vanilla::native_function_object::call(
    context& c, ptr* argv, unsigned argc)
{
    if(argc < _args.size())
    {
        BOOST_THROW_EXCEPTION(error::not_enough_arguments_error()
            << error::function_name(_name)
            << error::num_arguments_expected(_args.size())
            << error::num_arguments_received(argc));
    }
    
    if(argc > _args.size())
    {
        BOOST_THROW_EXCEPTION(error::too_many_arguments_error()
            << error::function_name(_name)
            << error::num_arguments_expected(_args.size())
            << error::num_arguments_received(argc));
    }
    
    std::vector<detail::native_datatype> converted_args( (argc) );
    std::vector<void*> native_argv( (argc) );
    detail::native_datatype result;
    
    if(_requires_locking)
    {
        // Convert arguments.
        std::lock_guard<std::mutex> lock_guard(_lock);
        for(unsigned i = 0; i < argc; ++i)
        {
            converted_args[i] = _argument_converter[i]->convert(argv[i]);
            native_argv[i] = &converted_args[i];
        }   
    
        // Call the function.
        ffi_call(&_cif, _function, &result, &native_argv[0]);
    }
    else
    {
        // Convert arguments.
        for(unsigned i = 0; i < argc; ++i)
        {
            converted_args[i] = _argument_converter[i]->convert(argv[i]);
            native_argv[i] = &converted_args[i];
        }   
    
        // Call the function.
        ffi_call(&_cif, _function, &result, &native_argv[0]);
    } 
    
    // Return the result.
    return _result_converter->convert(result);
}