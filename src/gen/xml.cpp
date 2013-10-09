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

// Vanilla:
#include <vanilla/gen/xml.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::xml_generator
///////////////////////////////////////////////////////////////////////////

void vanilla::gen::xml_generator::indent()
{
    for(unsigned i = 0; i < _indent_level; ++i)
        for(unsigned j = 0; j < _indent_spaces; ++j)
            _o << ' ';
}
    
void vanilla::gen::xml_generator::increase_indent()
{
    ++_indent_level;
}

void vanilla::gen::xml_generator::decrease_indent()
{
    --_indent_level;
}

void vanilla::gen::xml_generator::print_line(char const* s)
{
    indent();
    _o << s << '\n';
}
    
vanilla::gen::xml_generator::xml_generator(std::ostream& o, unsigned indent_spaces)
    : _o(o), _indent_spaces(indent_spaces), _indent_level(0)
{ }
    
// Nullary expressions.
void vanilla::gen::xml_generator::visit(variable_expression_node* n)
{
    indent();
    _o << "<variable_expression_node>" << n->get_name() << "</variable_expression_node>\n";
}

void vanilla::gen::xml_generator::visit(int_expression_node* n)
{
    indent();
    _o << "<int_expression_node>";
    context dummy;
    _o << string_object_to_cpp_string(n->eval(dummy)->to_string());
    _o << "</int_expression_node>\n";
}

void vanilla::gen::xml_generator::visit(float_expression_node* n)
{
    indent();
    _o << "<float_expression_node>";
    context dummy;
    _o << string_object_to_cpp_string(n->eval(dummy)->to_string());
    _o << "</float_expression_node>\n";
}

void vanilla::gen::xml_generator::visit(string_expression_node* n)
{
    indent();
    _o << "<string_expression_node>" << n->get_value() << "</string_expression_node>\n";
}

void vanilla::gen::xml_generator::visit(bool_expression_node* n)
{
    indent();
    _o << "<bool_expression_node>" << n->get_value() << "</bool_expression_node>\n";
}

void vanilla::gen::xml_generator::visit(array_expression_node* n)
{
    print_line("<array_expression_node>");
    increase_indent();
    for(auto& value : n->values())
        value->accept(this);
    decrease_indent();
    print_line("</array_expression_node>");
}
    
// Unary expressions.
void vanilla::gen::xml_generator::visit(negation_expression_node* n)
{
    print_line("<negation_expression_node>");
    increase_indent();
    n->get_child()->accept(this);
    decrease_indent();
    print_line("</negation_expression_node>");
}

void vanilla::gen::xml_generator::visit(abs_expression_node* n)
{
    print_line("<abs_expression_node>");
    increase_indent();
    n->get_child()->accept(this);
    decrease_indent();
    print_line("</abs_expression_node>");
}
    
// Binary expressions.
void vanilla::gen::xml_generator::visit(addition_expression_node* n)
{
    print_line("<addition_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</addition_expression_node>");
}
    
void vanilla::gen::xml_generator::visit(subtraction_expression_node* n)
{
    print_line("<subtraction_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</subtraction_expression_node>");
}

void vanilla::gen::xml_generator::visit(multiplication_expression_node* n)
{
    print_line("<multiplication_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</multiplication_expression_node>");
}
    
void vanilla::gen::xml_generator::visit(division_expression_node* n)
{
    print_line("<division_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</division_expression_node>");
}

void vanilla::gen::xml_generator::visit(concatenation_expression_node* n)
{
    print_line("<concatenation_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</concatenation_expression_node>");
}

void vanilla::gen::xml_generator::visit(lessthan_expression_node* n)
{
    print_line("<lessthan_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</lessthan_expression_node>");
}

void vanilla::gen::xml_generator::visit(lessequal_expression_node* n)
{
    print_line("<lessequal_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</lessequal_expression_node>");
}

void vanilla::gen::xml_generator::visit(greaterthan_expression_node* n)
{
    print_line("<greaterthan_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</greaterthan_expression_node>");
}
    
void vanilla::gen::xml_generator::visit(greaterequal_expression_node* n)
{
    print_line("<greaterequal_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</greaterequal_expression_node>");
}

void vanilla::gen::xml_generator::visit(equality_expression_node* n)
{
    print_line("<equality_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</equality_expression_node>");
}

void vanilla::gen::xml_generator::visit(inequality_expression_node* n)
{
    print_line("<inequality_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</inequality_expression_node>");
}

void vanilla::gen::xml_generator::visit(function_call_expression_node* n)
{
    print_line("<function_call_expression_node>");
    increase_indent();
    
    print_line("<target>");
    increase_indent();
    n->get_function()->accept(this);
    decrease_indent();
    print_line("</target>");
    
    print_line("<arguments>");
    increase_indent();
    for(auto& cur : n->get_args())
        cur->accept(this);
    decrease_indent();
    print_line("</arguments>");
    
    decrease_indent();
    print_line("</function_call_expression_node>");
}

void vanilla::gen::xml_generator::visit(function_definition_expression_node* n)
{
    print_line("<function_definition_expression_node>");
    increase_indent();
    
    indent();
    _o << "<name>" << n->get_name() << "</name>\n";
    
    print_line("<arguments>");
    increase_indent();
    for(auto const& cur : n->get_arguments())
    {
        indent();
        _o << "<name>" << n->get_name() << "</name>\n";
        
        if(cur.second)
        {
            print_line("<default>");
            increase_indent();
            cur.second->accept(this);
            decrease_indent();
            print_line("</default>");
        }
    }
    decrease_indent();
    print_line("</arguments>");
    
    print_line("<body>");
    increase_indent();
    n->get_body()->accept(this);
    decrease_indent();
    print_line("</body>");
    
    decrease_indent();
    print_line("</function_definition_expression_node>");
}

void vanilla::gen::xml_generator::visit(native_function_definition_expression_node* n)
{
    print_line("<native_function_definition_expression_node>");
    increase_indent();
    
    indent();
    _o << "<name>" << n->get_name() << "</name>\n";
    
    indent();
    _o << "<library>" << n->get_library() << "</library>\n";
    
    indent();
    _o << "<returns>" << n->get_return_type() << "</returns>\n";
    
    print_line("<arguments>");
    increase_indent();
    for(std::string const& cur : n->get_argument_types())
    {
        indent();
        _o << "<type>" << cur << "</type>\n";
    }
    decrease_indent();
    print_line("</arguments>");
    
    decrease_indent();
    print_line("</native_function_definition_expression_node>");
}

void vanilla::gen::xml_generator::visit(conditional_expression_node* n)
{
    print_line("<conditional_expression_node>");
    increase_indent();
    
    n->get_condition()->accept(this);
    n->get_expression()->accept(this);
    n->get_else()->accept(this);
    
    decrease_indent();
    print_line("</conditional_expression_node>");
}

void vanilla::gen::xml_generator::visit(subscript_expression_node* n)
{
    print_line("<subscript_expression_node>");
    increase_indent();
    n->get_expression()->accept(this);
    n->get_subscript()->accept(this);
    decrease_indent();
    print_line("</subscript_expression_node>");
}

void vanilla::gen::xml_generator::visit(element_selection_expression_node* n)
{
    print_line("<equality_expression_node>");
    increase_indent();
    n->get_left()->accept(this);
    indent();
    _o << n->get_element_name() << '\n';
    decrease_indent();
    print_line("</equality_expression_node>");
}

void vanilla::gen::xml_generator::visit(return_statement_node* n)
{
    print_line("<return_statement_node>");
    increase_indent();
    n->get_expression()->accept(this);
    decrease_indent();
    print_line("</return_statement_node>");
}

void vanilla::gen::xml_generator::visit(statement_sequence_node* n)
{
    for(statement_node::ptr const& cur : n->get_code())
        cur->accept(this);
}

void vanilla::gen::xml_generator::visit(assignment_statement_node* n)
{
    print_line("<assignment_statement_node>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</assignment_statement_node>");
}

void vanilla::gen::xml_generator::visit(if_statement_node* n)
{
    print_line("<if_statement_node>");
    increase_indent();

    auto const& ifs = n->get_ifs();
    print_line("<condition>");
    increase_indent();
    ifs[0].first->accept(this);
    decrease_indent();
    print_line("</condition>");
    print_line("<sequence>");
    increase_indent();
    ifs[0].second->accept(this);
    decrease_indent();
    print_line("</sequence>");
    
    for(unsigned i = 1; i < ifs.size(); ++i)
    {
        print_line("<elseif>");
        increase_indent();
        
        print_line("<condition>");
        increase_indent();
        ifs[i].first->accept(this);
        decrease_indent();
        print_line("</condition>");
        print_line("<sequence>");
        increase_indent();
        ifs[i].second->accept(this);
        decrease_indent();
        print_line("</sequence>");
        
        decrease_indent();
        print_line("</elseif>");
    }
    
    
    if(n->get_else())
    {
        print_line("<else>");
        increase_indent();
        n->get_else()->accept(this);
        decrease_indent();
        print_line("</else>");
    }
    
    decrease_indent();
    print_line("</if_statement_node>");
}

void vanilla::gen::xml_generator::visit(while_statement_node* n)
{
    print_line("<while_statement_node>");
    increase_indent();
    
    print_line("<condition>");
    increase_indent();
    n->get_condition()->accept(this);
    decrease_indent();
    print_line("</condition>");
    
    print_line("<code>");
    increase_indent();
    n->get_code()->accept(this);
    decrease_indent();
    print_line("</code>");
    
    decrease_indent();
    print_line("</while_statement_node>");
}

void vanilla::gen::xml_generator::visit(function_definition_statement_node* n)
{
    print_line("<function_definition_statement_node>");
    increase_indent();
    
    indent();
    _o << "<name>" << n->get_name() << "</name>\n";
    
    print_line("<arguments>");
    increase_indent();
    for(auto const& cur : n->get_arguments())
    {
        indent();
        _o << "<name>" << n->get_name() << "</name>\n";
        
        if(cur.second)
        {
            print_line("<default>");
            increase_indent();
            cur.second->accept(this);
            decrease_indent();
            print_line("</default>");
        }
    }
    decrease_indent();
    print_line("</arguments>");
    
    print_line("<body>");
    increase_indent();
    n->get_body()->accept(this);
    decrease_indent();
    print_line("</body>");
    
    decrease_indent();
    print_line("</function_definition_statement_node>");
}

void vanilla::gen::emit_xml(ast_node* ast, std::ostream& o, unsigned indent_spaces)
{
    xml_generator gen((o), indent_spaces);
    ast->accept(&gen);
}
