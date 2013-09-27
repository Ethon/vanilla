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

#ifndef HEADER_UUID_44B55D4117A44116B8D2911BE4538950
#define HEADER_UUID_44B55D4117A44116B8D2911BE4538950

namespace vanilla
{
    // Nullary expression forwarding.
    class variable_expression_node;
    class integer_expression_node;
    class real_expression_node;
    class string_expression_node;
    class boolean_expression_node;
    
    // Unary expression forwarding.
    class negation_expression_node;
    class abs_expression_node;
    
    // Binary expression forwarding.
    class addition_expression_node;
    class subtraction_expression_node;
    class multiplication_expression_node;
    class division_expression_node;
    class concatenation_expression_node;
    class lessthan_expression_node;
    class lessequal_expression_node;
    class greater_expression_node;
    class greaterequal_expression_node;
    class equality_expression_node;
    class inequality_expression_node;
    
    // Function expressions.
    class function_call_expression_node;
    class function_definition_expression_node;
    
    // Other expressions.
    class conditional_expression_node;
    
    // Statements.
    class return_statement_node;
    class statement_sequence_node;
    class if_statement_node;
    class while_statement_node;
    class assignment_statement_node;
    
    class ast_visitor
    {
    public:
        virtual ~ast_visitor() = 0;
        
        // Nullary expressions.
        virtual void visit(variable_expression_node*) = 0;
        virtual void visit(integer_expression_node*) = 0;
        virtual void visit(real_expression_node*) = 0;
        virtual void visit(string_expression_node*) = 0;
        virtual void visit(boolean_expression_node*) = 0;
        
        // Unary expressions.
        virtual void visit(negation_expression_node*) = 0;
        virtual void visit(abs_expression_node*) = 0;
        
        // Binary expressions.
        virtual void visit(addition_expression_node*) = 0;
        virtual void visit(subtraction_expression_node*) = 0;
        virtual void visit(multiplication_expression_node*) = 0;
        virtual void visit(division_expression_node*) = 0;
        virtual void visit(concatenation_expression_node*) = 0;
        virtual void visit(lessthan_expression_node*) = 0;
        virtual void visit(lessequal_expression_node*) = 0;
        virtual void visit(greater_expression_node*) = 0;
        virtual void visit(greaterequal_expression_node*) = 0;
        virtual void visit(equality_expression_node*) = 0;
        virtual void visit(inequality_expression_node*) = 0;
        
        // Function expressions.
        virtual void visit(function_call_expression_node*) = 0;
        virtual void visit(function_definition_expression_node*) = 0;
        
        // Other expressions.
        virtual void visit(conditional_expression_node*) = 0;
        
        // Statements.
        virtual void visit(return_statement_node*) = 0;
        virtual void visit(statement_sequence_node*) = 0;
        virtual void visit(if_statement_node*) = 0;
        virtual void visit(while_statement_node*) = 0;
        virtual void visit(assignment_statement_node*) = 0;
    };
    
    class ast_node
    {
    private:
        unsigned _line;
        unsigned _pos;
        
    public:
        ast_node(unsigned line, unsigned pos);
        virtual ~ast_node();
        virtual void accept(ast_visitor*) = 0;
        
        unsigned get_line() const;
        unsigned get_pos() const;
    };
}

#endif // HEADER_UUID_44B55D4117A44116B8D2911BE4538950
