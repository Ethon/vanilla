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

#ifndef HEADER_UUID_D46F169E01F944C2880A480F866B6746
#define HEADER_UUID_D46F169E01F944C2880A480F866B6746

// C++ Standard Library:
#include <memory>
#include <functional>
#include <string>

// Vanilla:
#include <vanilla/ast_base.hpp>
#include <vanilla/context.hpp>
#include <vanilla/object.hpp>
#include <vanilla/none_object.hpp>
#include <vanilla/int_object.hpp>
#include <vanilla/float_object.hpp>
#include <vanilla/bool_object.hpp>
#include <vanilla/string_object.hpp>

namespace vanilla
{
    ///////////////////////////////////////////////////////////////////////////
    /////////// BASE CLASSES FOR EXPRESSION NODES 
    ///////////////////////////////////////////////////////////////////////////
    
    class expression_node : public ast_node
    {
    public:
        expression_node(    unsigned line,
                            unsigned pos );
        
        typedef std::unique_ptr<expression_node> ptr;
        virtual object::ptr eval(context&) = 0;
    };
    
    class nullary_expression_node : public expression_node
    {
    public:
        nullary_expression_node(    unsigned line,
                                    unsigned pos );
    };
    
    template<typename Type, typename VanillaType>
    class value_expression_node : public nullary_expression_node
    {
    protected:
        Type _v;
        
    public:
        value_expression_node(unsigned line, unsigned pos, Type v)
            : nullary_expression_node(line, pos), _v(std::move(v))
        { }
        
        virtual object::ptr eval(context&) override
        {
            return allocate_object<VanillaType>(_v);
        }
        
        Type& get_value() const
        {
            return _v;
        }
    };
    
    class unary_expression_node : public expression_node
    {
    protected:
        expression_node::ptr _child;
        
    public:
        unary_expression_node(  unsigned line,
                                unsigned pos,
                                expression_node::ptr child );
        
        expression_node* get_child();
    };
    
    class binary_expression_node : public expression_node
    {
    protected:
        expression_node::ptr _left;
        expression_node::ptr _right;
        
    public:
        binary_expression_node( unsigned line,
                                unsigned pos,
                                expression_node::ptr left,
                                expression_node::ptr right );
        
        expression_node* get_left();
        
        expression_node* get_right();
    };
    
    ///////////////////////////////////////////////////////////////////////////
    /////////// NULLARY EXPRESSIONS 
    ///////////////////////////////////////////////////////////////////////////
    
    class variable_expression_node : public nullary_expression_node
    {  
    private:
        std::string _name;
        
    public:
        variable_expression_node(   unsigned line,
                                    unsigned pos,
                                    std::string name );
        
        virtual object::ptr eval(context&) override;
        
        std::string const& get_name() const;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class int_expression_node :
        public value_expression_node<int_object::int_type, int_object>
    {
    public:
        int_expression_node(    unsigned line,
                                unsigned pos,
                                int_object::int_type v );
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class float_expression_node :
        public value_expression_node<float_object::float_type, float_object>
    {
    public:
        float_expression_node(  unsigned line,
                                unsigned pos,
                                float_object::float_type v );
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class string_expression_node :
        public value_expression_node<string_object::string_type, string_object>
    {
    public:
        string_expression_node( unsigned line,
                                unsigned pos,
                                string_object::string_type v );
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class bool_expression_node :
        public value_expression_node<bool_object::bool_type, bool_object>
    {
    public:
        bool_expression_node(   unsigned line,
                                unsigned pos,
                                bool_object::bool_type v );
        
        virtual void accept(ast_visitor* v) override;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    /////////// UNARY OPERATOR EXPRESSIONS 
    ///////////////////////////////////////////////////////////////////////////

    class negation_expression_node : public unary_expression_node
    {
    public:
        negation_expression_node(   unsigned line,
                                    unsigned pos,
                                    expression_node::ptr child );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class abs_expression_node : public unary_expression_node
    {
    public:
        abs_expression_node(    unsigned line,
                                unsigned pos,
                                expression_node::ptr child );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    /////////// BINARY OPERATOR EXPRESSIONS 
    ///////////////////////////////////////////////////////////////////////////
    
    // Arithmetic.
    class addition_expression_node : public binary_expression_node
    {
    public:
        addition_expression_node(   unsigned line,
                                    unsigned pos,
                                    expression_node::ptr left,
                                    expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class subtraction_expression_node : public binary_expression_node
    {
    public:
        subtraction_expression_node(    unsigned line,
                                        unsigned pos,
                                        expression_node::ptr left,
                                        expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class multiplication_expression_node : public binary_expression_node
    {
    public:
        multiplication_expression_node( unsigned line,
                                        unsigned pos,
                                        expression_node::ptr left,
                                        expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class division_expression_node : public binary_expression_node
    {
    public:
        division_expression_node(   unsigned line,
                                    unsigned pos,
                                    expression_node::ptr left,
                                    expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    // Relational.
    class lessthan_expression_node : public binary_expression_node
    {
    public:
        lessthan_expression_node(   unsigned line,
                                    unsigned pos,
                                    expression_node::ptr left,
                                    expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class lessequal_expression_node : public binary_expression_node
    {
    public:
        lessequal_expression_node(  unsigned line,
                                    unsigned pos,
                                    expression_node::ptr left,
                                    expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class greaterthan_expression_node : public binary_expression_node
    {
    public:
        greaterthan_expression_node(    unsigned line,
                                        unsigned pos,
                                        expression_node::ptr left,
                                        expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class greaterequal_expression_node : public binary_expression_node
    {
    public:
        greaterequal_expression_node(   unsigned line,
                                        unsigned pos,
                                        expression_node::ptr left,
                                        expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };

    // Equality.
    class equality_expression_node : public binary_expression_node
    {
    public:
        equality_expression_node(   unsigned line,
                                    unsigned pos,
                                    expression_node::ptr left,
                                    expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class inequality_expression_node : public binary_expression_node
    {
    public:
        inequality_expression_node( unsigned line,
                                    unsigned pos,
                                    expression_node::ptr left,
                                    expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    // Other.
    class concatenation_expression_node : public binary_expression_node
    {
    public:
        concatenation_expression_node(  unsigned line,
                                        unsigned pos,
                                        expression_node::ptr left,
                                        expression_node::ptr right );
        
        virtual object::ptr eval(context&) override;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    /////////// FUNCTION EXPRESSIONS 
    ///////////////////////////////////////////////////////////////////////////
    
    class function_call_expression_node : public expression_node
    {
    private:
        expression_node::ptr _function;
        std::vector<expression_node::ptr> _args;
        
    public:
        function_call_expression_node(  unsigned line,
                                        unsigned pos,
                                        expression_node::ptr function,
                                        std::vector<expression_node::ptr> args );

        virtual object::ptr eval(context&) override;
        
        expression_node* get_function();
        
        std::vector<expression_node::ptr> const& get_args();
        
        virtual void accept(ast_visitor* v) override;
    };
    
    struct statement_node;
    class function_definition_expression_node : public expression_node
    {
    private:
        std::string _name;
        std::vector<std::pair<std::string, expression_node::ptr>> _arguments;
        std::shared_ptr<statement_node> _body;
        
    public:
        function_definition_expression_node(
            unsigned line,
            unsigned pos,
            std::string name,
            std::vector<std::pair<std::string, expression_node::ptr>> arguments,
            std::shared_ptr<statement_node> body );
        
        virtual object::ptr eval(context&) override;
        
        std::string const& get_name() const;
        
        std::vector<std::pair<std::string, expression_node::ptr>> const& get_arguments();
        
        statement_node* get_body();
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class native_function_definition_expression_node  : public expression_node
    {
    private:
        std::string _library;
        std::string _name;
        std::string _return_type;
        std::vector<std::string> _argument_types;
        
    public:
        native_function_definition_expression_node( unsigned line,
                                                    unsigned pos,
                                                    std::string library,
                                                    std::string name,
                                                    std::string return_type,
                                                    std::vector<std::string> argument_types );
        
        virtual object::ptr eval(context&) override;
        
        std::string const& get_library() const;
        
        std::string const& get_name() const;
        
        std::string const& get_return_type() const;
        
        std::vector<std::string> const& get_argument_types() const;
        
        virtual void accept(ast_visitor* v) override;
    };
    
    ///////////////////////////////////////////////////////////////////////////
    /////////// CLASS EXPRESSIONS 
    ///////////////////////////////////////////////////////////////////////////
    
    
    
    ///////////////////////////////////////////////////////////////////////////
    /////////// OTHER EXPRESSIONS 
    ///////////////////////////////////////////////////////////////////////////

    class conditional_expression_node : public expression_node
    {
    private:
        expression_node::ptr _condition;
        expression_node::ptr _expr;
        expression_node::ptr _else;
        
    public:
        conditional_expression_node(    unsigned line,
                                        unsigned pos,
                                        expression_node::ptr condition,
                                        expression_node::ptr expr,
                                        expression_node::ptr else_ );
        
        virtual object::ptr eval(context&) override;
        
        expression_node* get_condition();
        
        expression_node* get_expression();
        
        expression_node* get_else();
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class subscript_expression_node : public expression_node
    {
    private:
        expression_node::ptr _expr;
        expression_node::ptr _subscript;
        
    public:
        subscript_expression_node(  unsigned line,
                                    unsigned pos,
                                    expression_node::ptr expr,
                                    expression_node::ptr subscript );
        
        virtual object::ptr eval(context&) override;
        
        expression_node* get_expression();
        
        expression_node* get_subscript();
        
        virtual void accept(ast_visitor* v) override;
    };
    
    class element_selection_expression_node : public expression_node
    {
    private:
        expression_node::ptr _left;
        std::string _element_name;
        
    public:
        element_selection_expression_node(  unsigned line,
                                            unsigned pos,
                                            expression_node::ptr left,
                                            std::string element_name );
        
        virtual object::ptr eval(context&) override;
        
        expression_node* get_left();
        
        std::string const& get_element_name();
        
        virtual void accept(ast_visitor* v) override;
    };
}

#endif // HEADER_UUID_D46F169E01F944C2880A480F866B6746