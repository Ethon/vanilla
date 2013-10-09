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

#include <iostream>
#include <fstream>
#include <vanilla/error.hpp>
#include <vanilla/parsing.hpp>
#include <vanilla/native_library_cache.hpp>
#include <vanilla/gen/xml.hpp>
#include <vanilla/native_function_object.hpp>

using namespace vanilla;

int main(int argc, char **argv)
{
    using namespace std;
    
    if(argc != 2)
    {
        cerr << "Usage: " << argv[0] << " <filename>\n";
        return -1;
    }
    
    try
    {
        context c;
        
        auto ast = vanilla::parse_file(argv[1]);
        ast->eval(c);
        
        std::string filename(argv[1]);
        std::ofstream out(filename + ".xml");
        gen::emit_xml(ast.get(), out);
    }
    catch(error::invalid_token_error const& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Scanning error : Start of invalid token\n";
    }
    catch(error::unexpected_token_error const& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Parsing error : Expected '" << *error::get_expected_type(e)
                << "' token but got '" << *error::get_received_type(e) << "' token\n";
    }
    catch(error::expected_primary_expression_error const& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Parsing error : Expected primary expression but received '"
                << *error::get_received_type(e) << "' token\n";
    }
    catch(error::undefined_value_error const& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Evaluation error : Undefined value '" << *error::get_value_name(e) << "'\n";
    }
    catch(error::bad_binary_operation_error const& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Evaluation error : Can't apply binary operator '"
                << *error::get_operation_name(e)
                << "' to values of types '"
                << string_object_to_cpp_string((*error::get_first_operand(e))->type_name())
                << "' and '"
                << string_object_to_cpp_string((*error::get_second_operand(e))->type_name())
                << "'\n"; 
    }
    catch(error::native_library_loading_error& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Evaluation error : Failed to load native library '"
                << *error::get_native_library_name(e) << '\'';
                
        if(error::get_dlerror_string(e))
        {
            cerr << "(dlerror(): '" << *error::get_dlerror_string(e) << "')\n";
        }
        else
        {
            cerr << '\n';
        }
    }
    catch(error::native_symbol_not_found_error& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Evaluation error : Failed to load symbol '"
                << *error::get_native_symbol_name(e) << "' from native library '"
                << *error::get_native_library_name(e) << '\'';
                
        if(error::get_dlerror_string(e))
        {
            cerr << "(dlerror(): '" << *error::get_dlerror_string(e) << "')\n";
        }
        else
        {
            cerr << '\n';
        }
    }
    catch(error::unknown_native_type_name_error& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Evaluation error : Native type name '"
                << *error::get_native_type_name(e) << "' is currently not supported\n";
    }
    catch(error::void_as_argument_type_error& e)
    {
        cerr    << "[" << *error::get_line_info(e) << ':' << *error::get_pos_info(e)
                << "] Evaluation error : void used as argument type for native function\n";
    }
    
}
