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

#include <gis/Band.h>

#include <tuple>
#include <utility>

namespace gis
{

///
template< typename... Ts >
class Raster
{
public:
    ///
    typedef std::tuple< Band< Ts >... > Data;

    ///
    Raster(
        Band< Ts >&&... ts )
        :
        m_width(),
        m_height(),
        m_data( std::forward< Band< Ts > >( ts )... )
    {
        if( !same_size() )
        {
            throw std::runtime_error( "Raster::Raster: "
                "input bands do not have the same width and height" );
        }

        m_width = band().width();
        m_height = band().height();
    }

    ///
    Raster(
        int width,
        int height,
        Ts... nodatas,
        Ts... initvals )
        :
        m_width( width ),
        m_height( height ),
        m_data( Band< Ts >( width, height, nodatas, initvals )... )
    {
        ;
    }

    ///
    Raster(
        int width,
        int height,
        std::tuple< Ts... > nodatas,
        std::tuple< Ts... > initvals )
        :
        Raster(
            width,
            height,
            nodatas,
            initvals,
            std::make_index_sequence< sizeof...( Ts ) >{} )
    {
        ;
    }

    ///
    Raster(
        int width,
        int height )
        :
        m_width( width ),
        m_height( height ),
        m_data( Band< Ts >( width, height )... )
    {
        ;
    }

    ///
    Raster()
        :
        Raster( 0, 0 )
    {
        ;
    }

    ///
    int width()
    {
        return m_width;
    }

    ///
    int height()
    {
        return m_height;
    }

    ///
    constexpr std::size_t size()
    {
        return std::tuple_size< Data >::value;
    }

    ///
    template< std::size_t N = 0 >
    typename std::tuple_element< N, Data >::type& band()
    {
        return std::get< N >( m_data );
    }

    ///
    template< std::size_t N = 0 >
    typename std::tuple_element< N, Data >::type const& band() const
    {
        return std::get< N >( m_data );
    }

    ///
    template< typename U >
    auto append(
        Band< U >&& band )
    {
        //return Raster( std::tuple_cat( m_data, std::forward< U >( band ) ) );
    }

    ///
    constexpr bool same_size()
    {
        return same_size(
            std::make_index_sequence< std::tuple_size< Data >::value >{} );
    }

protected:

private:
    ///
    template< std::size_t... Ns >
    Raster(
        int width,
        int height,
        std::tuple< Ts... > nodatas,
        std::tuple< Ts... > initvals,
        std::index_sequence< Ns... > )
        :
        Raster(
            width,
            height,
            std::get< Ns >( nodatas )...,
            std::get< Ns >( initvals )... )
    {
        ;
    }

    ///
    template< typename U >
    constexpr bool same_size(
        U&& a )
    {
        return true;
    }

    ///
    template< typename U1, typename U2, typename... Us >
    constexpr bool same_size(
        U1&& a,
        U2&& b,
        Us&&... us )
    {
        return ( a.width() == b.width() ) && ( a.height() == b.height() ) &&
            same_size( a, std::forward< Us >( us )... );
    }

    ///
    template< std::size_t... Ns >
    constexpr bool same_size(
        std::index_sequence< Ns... > )
    {
        return same_size( std::get< Ns >( m_data )... );
    }

    ///
    int m_width;

    ///
    int m_height;

    ///
    Data m_data;
};

} //end gis