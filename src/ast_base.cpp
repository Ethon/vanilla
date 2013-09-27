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
#include <cassert>

// Vanilla:
#include <vanilla/ast_base.hpp>

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::ast_node
///////////////////////////////////////////////////////////////////////////

vanilla::ast_node::ast_node(unsigned line, unsigned pos)
    : _line(line), _pos(pos)
{ }

vanilla::ast_node::~ast_node()
{ }

unsigned vanilla::ast_node::get_line() const
{
    return _line;
}

unsigned vanilla::ast_node::get_pos() const
{
    return _pos;
}

///////////////////////////////////////////////////////////////////////////
/////////// vanilla::ast_visitor
///////////////////////////////////////////////////////////////////////////

vanilla::ast_visitor::~ast_visitor()
{ }