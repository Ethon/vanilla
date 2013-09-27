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

#ifndef HEADER_UUID_70C283C65D004A2992D861CFE3ACF1D1
#define HEADER_UUID_70C283C65D004A2992D861CFE3ACF1D1

// C++ Standard Library:
#include <ostream>

// Vanilla:
#include <vanilla/expression_ast.hpp>
#include <vanilla/statement_ast.hpp>

namespace vanilla
{
    namespace gen
    {
        class xml_generator : public vanilla::ast_visitor
        {
        private:
            std::ostream& _o;
            unsigned _indent_spaces;
            unsigned _indent_level;
            
            void indent();
            void increase_indent();
            void decrease_indent();
            void print_line(char const* s);
            
        public:
            xml_generator(std::ostream& o, unsigned indent_spaces = 4);
            
            // Nullary expressions.
            virtual void visit(variable_expression_node* n) override;
            virtual void visit(integer_expression_node* n) override;
            virtual void visit(real_expression_node* n) override;
            virtual void visit(string_expression_node* n) override;
            virtual void visit(boolean_expression_node* n) override;
            
            // Unary expressions.
            virtual void visit(negation_expression_node* n) override;
            virtual void visit(abs_expression_node* n) override;
            
            // Binary expressions.
            virtual void visit(addition_expression_node* n) override;
            virtual void visit(subtraction_expression_node* n) override;
            virtual void visit(multiplication_expression_node* n) override;
            virtual void visit(division_expression_node* n) override;
            virtual void visit(concatenation_expression_node* n) override;
            virtual void visit(lessthan_expression_node* n) override;
            virtual void visit(lessequal_expression_node* n) override;
            virtual void visit(greater_expression_node* n) override;
            virtual void visit(greaterequal_expression_node* n) override;
            virtual void visit(equality_expression_node* n) override;
            virtual void visit(inequality_expression_node* n) override;
            
            // Function expressions.
            virtual void visit(function_call_expression_node* n) override;
            virtual void visit(function_definition_expression_node* n) override;
            
            // Other expressions.
            virtual void visit(conditional_expression_node* n) override;
            
            // Statements.
            virtual void visit(return_statement_node* n) override;
            virtual void visit(statement_sequence_node* n) override;
            virtual void visit(if_statement_node* n) override;
            virtual void visit(while_statement_node* n) override;
            virtual void visit(assignment_statement_node* n) override;
        };
        
        void emit_xml(ast_node* ast, std::ostream& o, unsigned indent_spaces = 4);
    }
}


#endif // HEADER_UUID_70C283C65D004A2992D861CFE3ACF1D1