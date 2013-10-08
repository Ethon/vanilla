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

#ifndef HEADER_UUID_EBA2F1F013BF479D80BC82D224716693
#define HEADER_UUID_EBA2F1F013BF479D80BC82D224716693

// C++ Standard Library:
#include <vector>

// Vanilla:
#include <vanilla/ast_base.hpp>
#include <vanilla/expression_ast.hpp>

namespace vanilla
{
    class statement_node : public ast_node
    {
    public:
        statement_node( unsigned line,
                        unsigned pos );
        
        typedef std::unique_ptr<statement_node> ptr;
        virtual void eval(context&) = 0;
    };
    
    class expression_statement_node : public statement_node
    {
    private:
        expression_node::ptr _expression;
        
    public:
        expression_statement_node(  unsigned line,
                                    unsigned pos,
                                    expression_node::ptr expression );
        
        expression_node* get_expression();
        
        virtual void eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class statement_sequence_node : public statement_node
    {
    private:
        std::vector<statement_node::ptr> _code;
        
    public:
        statement_sequence_node(    unsigned line,
                                    unsigned pos,
                                    std::vector<statement_node::ptr> code );
        
        std::vector<statement_node::ptr>& get_code();
        
        virtual void eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class return_statement_node : public statement_node
    {
    private:
        expression_node::ptr _expression;
        
    public:
        return_statement_node(  unsigned line,
                                unsigned pos,
                                expression_node::ptr expression);
        
        expression_node* get_expression();
        
        virtual void eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class if_statement_node : public statement_node
    {
    private:
        std::vector<std::pair<expression_node::ptr, statement_node::ptr>> _ifs;
        statement_node::ptr _else;

    public:
        if_statement_node(  unsigned line,
                            unsigned pos,
                            std::vector<std::pair<expression_node::ptr, statement_node::ptr>> ifs,
                            statement_node::ptr else_ );
        
        std::vector<std::pair<expression_node::ptr, statement_node::ptr>> const& get_ifs();
        
        statement_node* get_else();
        
        virtual void eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class while_statement_node : public statement_node
    {
    private:
        expression_node::ptr _condition;
        statement_node::ptr _code;
        
    public:
        while_statement_node(   unsigned line,
                                unsigned pos,
                                expression_node::ptr condition,
                                statement_node::ptr code );
        
        expression_node* get_condition();
        
        statement_node* get_code();
        
        virtual void eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class function_definition_statement_node : public statement_node
    {
    private:
        std::string _name;
        std::vector<std::pair<std::string, expression_node::ptr>> _arguments;
        std::shared_ptr<statement_node> _body;
        
    public:
        function_definition_statement_node(
            unsigned line,
            unsigned pos,
            std::string name,
            std::vector<std::pair<std::string, expression_node::ptr>> arguments,
            std::shared_ptr<statement_node> body );
        
        std::string const& get_name() const;
        
        std::vector<std::pair<std::string, expression_node::ptr>> const& get_arguments();
        
        statement_node* get_body();
        
        virtual void eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class assignment_statement_node : public statement_node
    {
    private:
        expression_node::ptr _lhs;
        expression_node::ptr _rhs;

    public:
        assignment_statement_node(  unsigned line,
                                    unsigned pos,
                                    expression_node::ptr lhs,
                                    expression_node::ptr rhs );
        
        expression_node* get_left();
        
        expression_node* get_right();
        
        virtual void eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
}

#endif // HEADER_UUID_EBA2F1F013BF479D80BC82D224716693