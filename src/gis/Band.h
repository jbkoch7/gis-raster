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

#include <safe_compare.h>

#include <algorithm>
#include <numeric>
#include <vector>

namespace gis
{

///Band Class
template< typename T, typename A = std::allocator< T >, typename =
    typename std::enable_if< std::is_arithmetic< T >::value >::type >
class Band
{
public:
    ///Self friendship
    template< typename U, typename B, typename >
    friend class Band;

    ///Forward declaration
    class RowAccessor;
    class ConstRowAccessor;

    using Data = typename std::vector< T, A >;
    using Iterator = typename Data::iterator;
    using ConstIterator = typename Data::const_iterator;

    static constexpr T Lowest = std::numeric_limits< T >::lowest();
    static constexpr T Max = std::numeric_limits< T >::max();

    ///constructor
    template< typename N, typename V >
    Band(
        int width,
        int height,
        N nodataval,
        V initval )
        :
        m_upperleftx( 0.0 ),
        m_upperlefty( 0.0 ),
        m_width( width ),
        m_height( height ),
        m_srid( 0 ),
        m_scalex( 0.0 ),
        m_scaley( 0.0 ),
        m_nodatavalue( limits( nodataval ) ),
        m_data( m_height * m_width, limits( initval ) )
    {
        static_assert( std::is_arithmetic< N >::value,
            "Nodata value must be an arithmetic type" );
        static_assert( std::is_arithmetic< V >::value,
            "Initial value must be an arithmetic type" );
    }

    ///constructor
    template< typename N >
    Band(
        int width,
        int height,
        N nodataval )
        :
        Band( width, height, nodataval, nodataval )
    {
        ;
    }

    ///constructor
    Band(
        int width,
        int height )
        :
        Band( width, height, Lowest )
    {
        ;
    }

    ///constructor
    Band()
        :
        Band( 0, 0 )
    {
        ;
    }

    ///copy constructor
    Band( Band const& ) = default;

    ///template copy constructor
    template< typename U >
    Band(
        Band< U > const& o,
        bool clamp = false )
        :
        m_upperleftx( o.m_upperleftx ),
        m_upperlefty( o.m_upperlefty ),
        m_width( o.m_width ),
        m_height( o.m_height ),
        m_srid( o.m_srid ),
        m_scalex( o.m_scalex ),
        m_scaley( o.m_scaley ),
        m_nodatavalue( limits( o.m_nodatavalue ) ),
        m_data()
    {
        std::transform(
            o.m_data.begin(), o.m_data.end(),
            std::back_inserter( m_data ),
            [ clamp ]( U const val ) -> T
            {
                return limits( val, clamp );
            } );
    }

    ///copy assignment operator
    Band& operator =( Band const& ) = default;

    ///template copy assignment operator
    template< typename U >
    Band< T >& operator =(
        Band< U > const& o )
    {
        m_upperleftx = o.m_upperleftx;
        m_upperlefty = o.m_upperlefty;
        m_width = o.m_width;
        m_height = o.m_height;
        m_srid = o.m_srid;
        m_scalex = o.m_scalex;
        m_scaley = o.m_scaley;
        m_nodatavalue = limits( o.m_nodatavalue );
        std::transform(
            o.m_data.begin(), o.m_data.end(),
            std::back_inserter( m_data ),
            []( U const val ) -> T
            {
                return limits( val );
            } );
        return *this;
    }

    ///move constructor
    Band( Band&& ) = default;

    ///move assignment operator
    Band& operator =( Band&& ) = default;

    ///destructor
    ~Band() = default;

    ///
    bool oor_r(
        int r ) const
    {
        return !( r >= 0 && r < m_height );
    }

    ///
    bool oor_c(
        int c ) const
    {
        return !( c >= 0 && c < m_width );
    }

    ///
    bool oor(
        int r,
        int c ) const
    {
        return oor_r( r ) && oor_c( c );
    }

    ///
    std::size_t idx(
        int r,
        int c ) const
    {
        if( oor( r, c ) ) throw std::out_of_range(
            "Band::idx: out of range" );
        return ( r * m_width + c );
    }

    ///
    std::pair< int, int > rc(
        std::size_t const i ) const
    {
        if( i >= m_data.size() ) throw std::out_of_range(
            "Band::rc: out of range" );
        std::div_t res = std::div( i, m_width );
        return { res.quot, res.rem };
    }

    ///
    template< typename U >
    void set_at(
        std::size_t i,
        U val )
    {
        m_data.at( i ) = limits( val );
    }

    ///
    RowAccessor operator [](
        int r )
    {
        if( oor_r( r ) ) throw std::out_of_range(
            "Band::operator[]: out of range" );
        return RowAccessor( *this, r );
    }

    ///
    ConstRowAccessor operator [](
        int r ) const
    {
        if( oor_r( r ) ) throw std::out_of_range(
            "Band::operator[]: out of range" );
        return ConstRowAccessor( *this, r );
    }

    ///
    T& operator ()(
        int r,
        int c )
    {
        return m_data[ idx( r, c ) ];
    }

    ///
    T const& operator ()(
        int r,
        int c ) const
    {
        return m_data[ idx( r, c ) ];
    }

    ///
    T& at(
        std::size_t i )
    {
        return m_data.at( i );
    }

    ///
    T const& at(
        std::size_t i ) const
    {
        return m_data.at( i );
    }

    ///
    T& back()
    {
        return m_data.back();
    }

    ///
    T const& back() const
    {
        return m_data.back();
    }

    ///
    T& front()
    {
        return m_data.front();
    }

    ///
    T const& front() const
    {
        return m_data.front();
    }

    ///
    void insert(
        int r,
        int c,
        T const val )
    {
        m_data.insert( m_data.begin() + ( r * c ), val );
    }

    ///
    std::size_t size() const
    {
        return m_data.size();
    }

    ///
    Iterator begin()
    {
        return m_data.begin();
    }

    ///
    ConstIterator begin() const
    {
        return m_data.begin();
    }

    ///
    Iterator end()
    {
        return m_data.end();
    }

    ///
    ConstIterator end() const
    {
        return m_data.end();
    }

    ///
    double upperleftx() const
    {
        return m_upperleftx;
    }

    ///
    double lowerleftx() const
    {
        return m_upperleftx;
    }

    ///
    double upperrightx() const
    {
        return m_upperleftx + ( m_scalex * m_width );
    }

    ///
    double upperlefty() const
    {
        return m_upperlefty;
    }

    ///
    double upperrighty() const
    {
        return m_upperlefty;
    }

    ///
    double lowerlefty() const
    {
        return m_upperlefty + ( m_scaley * m_height );
    }

    ///
    int width() const
    {
        return m_width;
    }

    ///
    int height() const
    {
        return m_height;
    }

    ///
    int srid() const
    {
        return m_srid;
    }
    void srid(
        int val )
    {
        m_srid = val;
    }

    ///
    double scalex() const
    {
        return m_scalex;
    }
    void scalex(
        double val )
    {
        m_scalex = val;
    }

    ///
    double scaley() const
    {
        return ( m_scaley > 0 ) ? m_scaley * -1.0 : m_scaley;
    }
    void scaley(
        double val )
    {
        m_scaley = ( val > 0 ) ? val * -1.0 : val;
    }

    ///
    void scale(
        double val )
    {
        scalex( val );
        scaley( val );
    }
    void scale(
        double xval,
        double yval )
    {
        scalex( xval );
        scaley( yval );
    }

    ///
    T nodatavalue() const
    {
        return m_nodatavalue;
    }
    void nodatavalue(
        T nodataval )
    {
        m_nodatavalue = nodataval;
    }
    template< typename N >
    void nodatavalue(
        N nodataval )
    {
        static_assert( std::is_arithmetic< N >::value,
            "Nodata value must be an arithmetic type" );
        m_nodatavalue = limits( nodataval );
    }

    ///
    std::size_t count() const
    {
        return std::count_if( m_data.begin(), m_data.end(),
            [ this ]( T val )
            {
                return ( val != m_nodatavalue );
            } );
    }

    ///Need to double check these...
    T min() const
    {
        return *std::min_element( m_data.begin(), m_data.end(),
            [ this ]( T lhs, T rhs )
            {
                if( lhs == m_nodatavalue ) return false;
                if( rhs == m_nodatavalue ) return true;
                return ( lhs < rhs );
            } );
    }

    ///Need to double check these...
    T max() const
    {
        return *std::max_element( m_data.begin(), m_data.end(),
            [ this ]( T lhs, T rhs )
            {
                if( rhs == m_nodatavalue ) return false;
                if( lhs == m_nodatavalue ) return true;
                return ( lhs < rhs );
            } );
    }

    ///
    T sum() const
    {
        return std::accumulate( m_data.begin(), m_data.end(), 0,
            [ this ]( T lhs, T rhs )
            {
                return ( rhs == m_nodatavalue ) ? lhs : lhs + rhs;
            } );
    }

    ///
    double avg() const
    {
        return sum() / static_cast< double >( count() );
    }

    ///
    template< typename U >
    Band< T >& operator *=(
        U rhs )
    {
        static_assert( std::is_arithmetic< U >::value,
            "Multiplication requires arithmetic type" );
        for( auto& v : m_data )
        {
            if( v == m_nodatavalue ) continue;
            v *= limits( rhs );
        }
        return *this;
    }

    ///
    Band transpose()
    {
        Band result = *this;
        result.m_height = m_width;
        result.m_width = m_height;
        for( int r = 0; r < m_height; ++r )
        {
            for( int c = 0; c < m_width; ++j )
            {
                result( c, r ) = operator ()( r, c );
            }
        }
        return result;
    }

    ///
    template< typename U, typename N, typename V >
    void copy_props(
        Band< U > const& o,
        N nodataval,
        V initval )
    {
        static_assert( std::is_arithmetic< N >::value,
            "Nodata value must be an arithmetic type" );
        static_assert( std::is_arithmetic< V >::value,
            "Initial value must be an arithmetic type" );
        m_upperleftx = o.m_upperleftx;
        m_upperlefty = o.m_upperlefty;
        m_width = o.m_width;
        m_height = o.m_height;
        m_srid = o.m_srid;
        m_scalex = o.m_scalex;
        m_scaley = o.m_scaley;
        m_nodatavalue = limits( nodataval );
        m_data.assign( o.size(), limits( initval ) );
    }

    ///
    template< typename U, typename N >
    void copy_props(
        Band< U > const& o,
        N nodataval )
    {
        copy_props( o, nodataval, nodataval );
    }

    ///
    template< typename U >
    void copy_props(
        Band< U > const& o )
    {
        copy_props( o, o.m_nodatavalue );
    }

    ///
    void init(
        T initval )
    {
        for( auto& v : m_data ) v = initval;
    }

private:
    ///
    class RowAccessor
    {
    public:
        ///
        RowAccessor(
            Band& band,
            int row )
            :
            m_band( band ),
            m_row( row )
        {
            ;
        }

        ///
        T& operator [](
            int col )
        {
            return m_band( m_row, col );
        }

    private:
        ///
        Band& m_band;

        ///
        int m_row;
    };

    ///
    class ConstRowAccessor
    {
    public:
        ///
        ConstRowAccessor(
            Band const& band,
            int row )
            :
            m_band( band ),
            m_row( row )
        {
            ;
        }

        ///
        T const& operator [](
            int col ) const
        {
            return m_band( m_row, col );
        }

    private:
        ///
        Band const& m_band;

        ///
        int m_row;
    };

    ///
    static constexpr T limits(
        T val,
        bool clamp = false )
    {
        return val;
    }

    ///
    template< typename U >
    static constexpr T limits(
        U val,
        bool clamp = false )
    {
        static constexpr bool is_ftoi_v =
            std::is_integral< T >::value && std::is_floating_point< U >::value;
        if( is_ftoi_v ) val = static_cast< U >( std::round( val ) );

        static safe_compare::less< U, T > sless;
        static safe_compare::greater< U, T > sgreater;
        if( sless( val, Lowest ) )
        {
            if( clamp ) return Lowest;
            throw std::out_of_range(
                "Band::limits: value too small " + std::to_string( val ) );
        }
        else if( sgreater( val, Max ) )
        {
            if( clamp ) return Max;
            throw std::out_of_range(
                "Band::limits: value too large " + std::to_string( val ) );
        }
        return static_cast< T >( val );
    }

    ///
    double m_upperleftx;

    ///
    double m_upperlefty;

    ///
    int m_width;

    ///
    int m_height;

    ///
    int m_srid;

    ///
    double m_scalex;

    ///
    double m_scaley;

    ///
    static constexpr double m_skewx = 0.0;

    ///
    static constexpr double m_skewy = 0.0;

    ///
    T m_nodatavalue;

    ///
    Data m_data;
};

///
template< typename T >
inline auto begin(
    Band< T >& o )
{
    return o.begin();
}

///
template< typename T >
inline auto begin(
    Band< T > const& o )
{
    return o.begin();
}

///
template< typename T >
inline auto end(
    Band< T >& o )
{
    return o.end();
}

///
template< typename T >
inline auto end(
    Band< T > const& o )
{
    return o.end();
}

} //end gis
