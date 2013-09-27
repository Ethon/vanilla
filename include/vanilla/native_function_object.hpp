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

#ifndef HEADER_UUID_A2E2D9BCF7634E7FA39CA9891C41BD53
#define HEADER_UUID_A2E2D9BCF7634E7FA39CA9891C41BD53

// C++ Standard Library:
#include <memory>
#include <cstdint>
#include <mutex>

// libffi:
#include <ffi.h>

// Vanilla:
#include <vanilla/object.hpp>

namespace vanilla
{
    namespace detail
    {
        union native_datatype
        {
            std::uint8_t ui8;
            std::uint16_t ui16;
            std::uint32_t ui32;
            std::uint64_t ui64;
            
            std::int8_t i8;
            std::int16_t i16;
            std::int32_t i32;
            std::int64_t i64;
            
            float f;
            double d;
            long double ld;
            
            unsigned char uc;
            unsigned short us;
            unsigned int ui;
            unsigned long ul;
            
            char c;
            short s;
            int i;
            long l;
            
            void* p;
        };
            
        struct ffi_result_converter
        {
            virtual ~ffi_result_converter();
            virtual object::ptr convert(detail::native_datatype const&) = 0;
        };
        
        struct ffi_argument_converter
        {
            virtual ~ffi_argument_converter();
            virtual detail::native_datatype convert(object::ptr const&) = 0;
            virtual bool has_state() const = 0;
        };
    }
    
    object_type_id const OBJECT_ID_NATIVE_FUNCTION = 0x8;
    
    class native_function_object : public object
    {
    private:
        std::string _name;
        std::string _library;
        std::string _result;
        std::vector<std::string> _args;
        ffi_abi _abi;
        ffi_cif _cif;
        void(*_function)();
        std::unique_ptr<ffi_type*[]> _ffiargs;
        std::vector<std::unique_ptr<detail::ffi_argument_converter>> _argument_converter;
        std::unique_ptr<detail::ffi_result_converter> _result_converter;
        std::mutex _lock;
        bool _requires_locking;
        
    public:
        native_function_object( std::string name,
                                std::string library,
                                std::string result,
                                std::vector<std::string> args,
                                ffi_abi abi = FFI_DEFAULT_ABI );
        
        ~native_function_object();
        
        virtual object_type_id type_id() const override;
        
        virtual ptr type_name() const override;
        
        virtual ptr clone() const override;
        
        virtual ptr to_string() const override;
        
        virtual ptr call(context& c, ptr* argv, unsigned argc) override;
    };
    
    namespace error
    {
        struct unknown_native_type_name_error : evaluation_error
        { };
        
        struct void_as_argument_type_error : evaluation_error
        { };
        
        VANILLA_MAKE_ERRINFO(std::string, native_type_name)
    }
}

#endif // HEADER_UUID_A2E2D9BCF7634E7FA39CA9891C41BD53