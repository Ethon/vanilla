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

#ifndef HEADER_UUID_B0C5E9BC65B94BBC95987A600973B9EF
#define HEADER_UUID_B0C5E9BC65B94BBC95987A600973B9EF

// C++ Standard Library:
#include <cassert>
#include <stdexcept>
#include <ostream>
#include <cstring>
#include <type_traits>

namespace vanilla
{
    template<typename Char>
    class basic_str_range
    {
    private:
        Char* _begin;
        Char* _end;
        
    public:
        basic_str_range()
            : _begin(nullptr), _end(nullptr)
        { }
        
        basic_str_range(Char* begin, Char* end)
            : _begin(begin), _end(end)
        {
            assert(begin <= end);
        }
        
        basic_str_range(Char* begin)
            : _begin(begin), _end(begin + std::strlen(begin))
        { }
        
        template<std::size_t N>
        basic_str_range(Char (&s)[N])
            : _begin(s), _end(s + N)
        { }
        
        Char* begin()
        {
            return _begin;
        }
        
        Char const* begin() const
        {
            return _begin;
        }
        
        Char* end()
        {
            return _end;
        }
        
        Char const* end() const
        {
            return _end;
        }
        
        bool empty() const
        {
            return _begin == _end;
        }
        
        std::size_t length() const
        {
            return std::size_t(_end - _begin);
        }
        
        Char& at(std::size_t index)
        {
            Char* pc = _begin + index;
            if(pc >= _end)
                throw std::out_of_range("basic_str_range::at");
            return *pc;
        }
        
        Char const& at(std::size_t index) const
        {
            Char* pc = _begin + index;
            if(pc >= _end)
                throw std::out_of_range("basic_str_range::at");
            return *pc;
        }
        
        Char& operator[](std::size_t index)
        {
            return _begin[index];
        }
        
        Char const& operator[](std::size_t index) const
        {
            return _begin[index];
        }
        
        int compare(basic_str_range<Char> const& other) const
        {
            int r = std::memcmp(begin(), other.begin(), std::min(length(), other.length()) * sizeof(Char));
            if(r != 0)
                return r;
            if(length() == other.length())
                return 0;
            return length() > other.length() ? 1 : -1;
        }
        
        int compare(Char const* s) const
        {
            Char const* p = begin();
            for(; p != end() && *s; ++p, ++s)
            {
                if(*p != *s)
                    return *p - *s;
            }
            
            if(p == end() && *s)
                return -*s;
            if(p != end() && !*s)
                return *p;
            return 0;
        }
    };
    
    template<typename Char>
    bool operator==(basic_str_range<Char> const& lhs, basic_str_range<Char> const& rhs)
    {
        return lhs.compare(rhs) == 0;
    }
    
    template<typename Char>
    bool operator==(basic_str_range<Char> const& lhs, Char const* rhs)
    {
        return lhs.compare(rhs) == 0;
    }
    
    template<typename Char>
    bool operator==(Char const* lhs, basic_str_range<Char> const& rhs)
    {
        return rhs.compare(lhs) == 0;
    }
    
    template<typename Char>
    std::basic_ostream<typename std::remove_cv<Char>::type>& operator<<(
        std::basic_ostream<typename std::remove_cv<Char>::type>& o,
        basic_str_range<Char> const& s)
    {
        o.write(s.begin(), s.length());
        return o;
    }
    
    typedef basic_str_range<char> str_range;
    typedef basic_str_range<char const> cstr_range;
}

#endif // HEADER_UUID_B0C5E9BC65B94BBC95987A600973B9EF