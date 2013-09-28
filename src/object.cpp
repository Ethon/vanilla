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
#include <sstream>

// Vanilla:
#include <vanilla/object.hpp>
#include <vanilla/string_object.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::object
///////////////////////////////////////////////////////////////////////////

vanilla::object::~object()
{ }

vanilla::object::ptr vanilla::object::clone() const
{
    vanilla::object* this_ = const_cast<vanilla::object*>(this);
    BOOST_THROW_EXCEPTION(error::unsupported_operation_error()
        << error::first_operand(this_->shared_from_this())
        << error::operation_name("clone"));
}

vanilla::object::ptr vanilla::object::to_string() const
{
    std::ostringstream ss;
    ss  << "<" << static_cast<string_object*>(type_name().get())->value()
        << " object @ " << (void const*)this << ">";
    return allocate_object<string_object>(ss.str());
}

vanilla::object::ptr vanilla::object::to_int() const
{
    BOOST_THROW_EXCEPTION(error::bad_cast_error()
        << error::first_operand(const_cast<object*>(this)->shared_from_this())
        << error::cast_target_name("int"));
}

vanilla::object::ptr vanilla::object::to_float() const
{
    BOOST_THROW_EXCEPTION(error::bad_cast_error()
        << error::first_operand(const_cast<object*>(this)->shared_from_this())
        << error::cast_target_name("float"));
}

vanilla::object::ptr vanilla::object::to_bool() const
{
    BOOST_THROW_EXCEPTION(error::bad_cast_error()
        << error::first_operand(const_cast<object*>(this)->shared_from_this())
        << error::cast_target_name("bool"));
}

vanilla::object::ptr vanilla::object::call(context&, ptr*, unsigned)
{
    BOOST_THROW_EXCEPTION(error::value_not_callable_error()
        << error::first_operand(shared_from_this()));
}

vanilla::object::ptr vanilla::object::neg()
{
    BOOST_THROW_EXCEPTION(error::bad_unary_operation_error()
        << error::operation_name("-")
        << error::first_operand(shared_from_this()));
}

vanilla::object::ptr vanilla::object::abs()
{
    BOOST_THROW_EXCEPTION(error::bad_unary_operation_error()
        << error::operation_name("+")
        << error::first_operand(shared_from_this()));
}

vanilla::object::ptr vanilla::object::add(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("+")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::sub(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("-")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::mul(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("*")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::div(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("/")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::lt(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("<")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::le(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("<=")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::gt(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name(">")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::ge(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name(">=")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::eq(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("==")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::neq(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("!=")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::concat(object::ptr const& other)
{
    BOOST_THROW_EXCEPTION(error::bad_binary_operation_error()
        << error::operation_name("~")
        << error::first_operand(shared_from_this())
        << error::second_operand(other));
}

vanilla::object::ptr vanilla::object::sget(object::ptr const&)
{
    BOOST_THROW_EXCEPTION(error::unsupported_operation_error()
        << error::first_operand(shared_from_this())
        << error::operation_name("subscript"));
}

void vanilla::object::sset(object::ptr const&, ptr)
{
    BOOST_THROW_EXCEPTION(error::unsupported_operation_error()
        << error::first_operand(shared_from_this())
        << error::operation_name("subscript assign"));
}

vanilla::object::ptr vanilla::object::eget(std::string const& name)
{
    BOOST_THROW_EXCEPTION(error::unsupported_operation_error()
        << error::first_operand(shared_from_this())
        << error::cast_target_name(name)
        << error::operation_name("element selection"));
}

void vanilla::object::eset(std::string const& name, ptr)
{
    BOOST_THROW_EXCEPTION(error::unsupported_operation_error()
        << error::first_operand(shared_from_this())
        << error::cast_target_name(name)
        << error::operation_name("element assign"));
}