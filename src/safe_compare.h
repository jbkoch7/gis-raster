/*******************************************************************************
 *
 * MIT License
 *
 * Copyright (c) 2018 Joshua Koch
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 ******************************************************************************/

#pragma once

#include <type_traits>

namespace safe_compare
{

///
template< bool B, typename T = void >
using enable_if_t = typename std::enable_if< B, T >::type;

///
template< typename T >
using make_unsigned_t = typename std::make_unsigned< T >::type;

///
template< typename T, typename U >
struct both_arithmetic
{
    static constexpr bool value =
        std::is_arithmetic< T >::value && std::is_arithmetic< U >::value;
};

///
template< typename T, typename U >
struct both_integral
{
    static constexpr bool value =
        std::is_integral< T >::value && std::is_integral< U >::value;
};

///
template< typename T, typename U, typename =
    enable_if_t< both_arithmetic< T, U >::value > >
struct is_first_variant
{
    static constexpr bool value = both_integral< T, U >::value &&
        std::is_signed< T >::value && std::is_unsigned< U >::value;
};

///
template< typename T, typename U, typename =
    enable_if_t< both_arithmetic< T, U >::value > >
struct is_second_variant
{
    static constexpr bool value = both_integral< T, U >::value &&
        std::is_unsigned< T >::value && std::is_signed< U >::value;
};

///
template< typename T, typename U, typename =
    enable_if_t< both_arithmetic< T, U >::value > >
struct is_not_variant
{
    static constexpr bool value =
        !is_first_variant< T, U >::value && !is_second_variant< T, U >::value;
};

///
template< typename T, typename U,
    enable_if_t< is_not_variant< T, U >::value, int > = 0 >
constexpr bool is_greater( T lhs, U rhs )
{
    return ( lhs > rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_first_variant< T, U >::value, int > = 0 >
constexpr bool is_greater( T lhs, U rhs )
{
    return ( lhs >= 0 ) && ( static_cast< make_unsigned_t< T > >( lhs ) > rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_second_variant< T, U >::value, int > = 0 >
constexpr bool is_greater( T lhs, U rhs )
{
    return ( rhs < 0 ) || ( lhs > static_cast< make_unsigned_t< U > >( rhs ) );
}

///
template< typename T, typename U,
    enable_if_t< is_not_variant< T, U >::value, int > = 0 >
constexpr bool is_less( T lhs, U rhs )
{
    return ( lhs < rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_first_variant< T, U >::value, int > = 0 >
constexpr bool is_less( T lhs, U rhs )
{
    return ( lhs < 0 ) || ( static_cast< make_unsigned_t< T > >( lhs ) < rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_second_variant< T, U >::value, int > = 0 >
constexpr bool is_less( T lhs, U rhs )
{
    return ( rhs >= 0 ) && ( lhs < static_cast< make_unsigned_t< U > >( rhs ) );
}

///
template< typename T, typename U,
    enable_if_t< is_not_variant< T, U >::value, int > = 0 >
constexpr bool is_greater_equal( T lhs, U rhs )
{
    return ( lhs >= rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_first_variant< T, U >::value, int > = 0 >
constexpr bool is_greater_equal( T lhs, U rhs )
{
    return ( lhs >= 0 ) && ( static_cast< make_unsigned_t< T > >( lhs ) >= rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_second_variant< T, U >::value, int > = 0 >
constexpr bool is_greater_equal( T lhs, U rhs )
{
    return ( rhs < 0 ) || ( lhs >= static_cast< make_unsigned_t< U > >( rhs ) );
}

///
template< typename T, typename U,
    enable_if_t< is_not_variant< T, U >::value, int > = 0 >
constexpr bool is_less_equal( T lhs, U rhs )
{
    return ( lhs <= rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_first_variant< T, U >::value, int > = 0 >
constexpr bool is_less_equal( T lhs, U rhs )
{
    return ( lhs < 0 ) || ( static_cast< make_unsigned_t< T > >( lhs ) <= rhs );
}

///
template< typename T, typename U,
    enable_if_t< is_second_variant< T, U >::value, int > = 0 >
constexpr bool is_less_equal( T lhs, U rhs )
{
    return ( rhs >= 0 ) && ( lhs <= static_cast< make_unsigned_t< U > >( rhs ) );
}

///
template< typename T, typename U >
struct greater
{
    ///
    constexpr bool operator()( T lhs, U rhs ) const
    {
        return is_greater< T, U >( lhs, rhs );
    }
};

///
template< typename T, typename U >
struct less
{
    ///
    constexpr bool operator()( T lhs, U rhs ) const
    {
        return is_less< T, U >( lhs, rhs );
    }
};

///
template< typename T, typename U >
struct greater_equal
{
    ///
    constexpr bool operator()( T lhs, U rhs ) const
    {
        return is_greater_equal< T, U >( lhs, rhs );
    }
};

///
template< typename T, typename U >
struct less_equal
{
    ///
    constexpr bool operator()( T lhs, U rhs ) const
    {
        return is_less_equal< T, U >( lhs, rhs );
    }
};

} //end safe_compare
