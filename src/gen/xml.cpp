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
    _o << "<variable>" << n->get_name() << "</variable>\n";
}

void vanilla::gen::xml_generator::visit(integer_expression_node* n)
{
    indent();
    _o << "<integer>" << n->get_value() << "</integer>\n";
}

void vanilla::gen::xml_generator::visit(real_expression_node* n)
{
    indent();
    _o << "<real>" << n->get_value() << "</real>\n";
}

void vanilla::gen::xml_generator::visit(string_expression_node* n)
{
    indent();
    _o << "<string>" << *n->get_value() << "</string>\n";
}

void vanilla::gen::xml_generator::visit(boolean_expression_node* n)
{
    indent();
    _o << "<boolean>" << n->get_value() << "</boolean>\n";
}
    
// Unary expressions.
void vanilla::gen::xml_generator::visit(negation_expression_node* n)
{
    print_line("<negation>");
    increase_indent();
    n->get_child()->accept(this);
    decrease_indent();
    print_line("</negation>");
}

void vanilla::gen::xml_generator::visit(abs_expression_node* n)
{
    print_line("<abs>");
    increase_indent();
    n->get_child()->accept(this);
    decrease_indent();
    print_line("</abs>");
}
    
    
// Binary expressions.
void vanilla::gen::xml_generator::visit(addition_expression_node* n)
{
    print_line("<addition>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</addition>");
}
    
void vanilla::gen::xml_generator::visit(subtraction_expression_node* n)
{
    print_line("<subtraction>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</subtraction>");
}

void vanilla::gen::xml_generator::visit(multiplication_expression_node* n)
{
    print_line("<multiplication>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</multiplication>");
}
    
void vanilla::gen::xml_generator::visit(division_expression_node* n)
{
    print_line("<division>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</division>");
}

void vanilla::gen::xml_generator::visit(concatenation_expression_node* n)
{
    print_line("<concatenation>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</concatenation>");
}

void vanilla::gen::xml_generator::visit(lessthan_expression_node* n)
{
    print_line("<less>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</less>");
}

void vanilla::gen::xml_generator::visit(lessequal_expression_node* n)
{
    print_line("<lessequal>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</lessequal>");
}

void vanilla::gen::xml_generator::visit(greater_expression_node* n)
{
    print_line("<greater>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</greater>");
}
    
void vanilla::gen::xml_generator::visit(greaterequal_expression_node* n)
{
    print_line("<greaterequal>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</greaterequal>");
}

void vanilla::gen::xml_generator::visit(equality_expression_node* n)
{
    print_line("<equality>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</equality>");
}

void vanilla::gen::xml_generator::visit(inequality_expression_node* n)
{
    print_line("<inequality>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</inequality>");
}

void vanilla::gen::xml_generator::visit(function_call_expression_node* n)
{
    print_line("<call>");
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
    print_line("</call>");
}

void vanilla::gen::xml_generator::visit(function_definition_expression_node* n)
{
    print_line("<function>");
    increase_indent();
    
    for(auto const& cur : n->get_argnames())
    {
        indent();
        _o << "<argname>" << cur << "</argname>\n";
    }
    
    print_line("<body>");
    increase_indent();
    n->get_body()->accept(this);
    decrease_indent();
    print_line("</body>");
    
    decrease_indent();
    print_line("</function>");
}

void vanilla::gen::xml_generator::visit(conditional_expression_node* n)
{
    print_line("<conditional>");
    increase_indent();
    
    n->get_condition()->accept(this);
    n->get_expression()->accept(this);
    n->get_else()->accept(this);
    
    decrease_indent();
    print_line("</conditional>");
}

void vanilla::gen::xml_generator::visit(return_statement_node* n)
{
    print_line("<return>");
    increase_indent();
    n->get_expression()->accept(this);
    decrease_indent();
    print_line("</return>");
}

void vanilla::gen::xml_generator::visit(statement_sequence_node* n)
{
    for(statement_node::ptr const& cur : n->get_code())
        cur->accept(this);
}

void vanilla::gen::xml_generator::visit(assignment_statement_node* n)
{
    print_line("<assignment>");
    increase_indent();
    n->get_left()->accept(this);
    n->get_right()->accept(this);
    decrease_indent();
    print_line("</assignment>");
}

void vanilla::gen::xml_generator::visit(if_statement_node* n)
{
    print_line("<if>");
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
    print_line("</if>");
}

void vanilla::gen::xml_generator::visit(while_statement_node* n)
{
    print_line("<while>");
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
    print_line("</while>");
}

void vanilla::gen::emit_xml(ast_node* ast, std::ostream& o, unsigned indent_spaces)
{
    xml_generator gen((o), indent_spaces);
    ast->accept(&gen);
}
