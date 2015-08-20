/*
* Copyright (c) 2015 André Tupinambá (andrelrt@gmail.com)
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
* AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
* LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
* OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
* THE SOFTWARE.
*/

#include <map>
#include <iostream>

#include <boost/timer/timer.hpp>
#include <boost/container/flat_map.hpp>

#include "soa_map.h"

template< class MapType >
void forwardFill( MapType& ret_map, size_t size )
{
    for( size_t i = 0; i < size; ++i )
    {
        ret_map.insert( std::make_pair( i, i ) );
    }
}

template< class MapType >
void reverseFill( MapType& ret_map, size_t size )
{
    for( size_t i = 0; i < size; ++i )
    {
        ret_map.insert( std::make_pair( size - i, i ) );
    }
}

template< class MapType >
void forwardFind( MapType& ret_map, size_t size )
{
    auto itEnd = ret_map.end();
    for( size_t i = 0; i < size; ++i )
    {
        auto it = ret_map.lower_bound( i );
        if( itEnd == it )
        {
            std::cout << "ff Oops! " << i << std::endl;
        }
    }
}

template< class MapType >
void reverseFind( MapType& ret_map, size_t size )
{
    auto itEnd = ret_map.end();
    for( size_t i = 0; i < size; ++i )
    {
        auto it = ret_map.lower_bound( size - i );
        if( itEnd == it )
        {
            std::cout << "rew Oops! " << i << std::endl;
        }
    }
}

template< class T_XY >
void least_square( const std::string& name )
{
    T_XY xy;

    srand( 1 );

    size_t size = 512 * 1024 * 1024;
    xy.reserve( size );
    for( size_t i = 0; i < size; ++i )
    {
        xy.push_back( i + ( 2.f * rand() - RAND_MAX / 2.f ) / RAND_MAX,
            i + ( 4.f * rand() - RAND_MAX / 2.f ) / RAND_MAX );
    }

    boost::timer::cpu_timer timer;

    timer.start();
    float meanx = 0;
    float meany = 0;
    for( size_t i = 0; i < size; ++i )
    {
        meanx += xy.get_x( i );
        meany += xy.get_y( i );
    }

    meanx /= size;
    meany /= size;

    float numerator = 0;
    float denominator = 0;
    for( size_t i = 0; i < size; ++i )
    {
        float diffx = ( xy.get_x( i ) - meanx );
        numerator += diffx * ( xy.get_y( i ) - meany );
        denominator += diffx * diffx;
    }

    float b = numerator / denominator;
    float a = meany - b * meanx;
    timer.stop();

    std::cout << "Least Square linear regression " << name << ": y = " << a << " + " << b << "x + e : "
        << timer.format();
}

struct XYZ
{
    float x;
    float y;
    XYZ( float x_, float y_ ) : x( x_ ), y( y_ ) {}
};
void scale_aos()
{
    std::vector< XYZ > xyz_a;
    std::vector< XYZ > xyz_b;
    std::vector< XYZ > ret_xyz;

    srand( 1 );

    size_t size = 512 * 1024;
    xyz_a.reserve( size );
    xyz_b.reserve( size );
    for( size_t i = 0; i < size; ++i )
    {
        xyz_a.emplace_back(  static_cast<float>( i ),  static_cast<float>( i ) );
        xyz_b.emplace_back( -static_cast<float>( i ), -static_cast<float>( i ) );
    }
    ret_xyz.resize( size, { 0.f, 0.f } );

    boost::timer::cpu_timer timer;

    XYZ* pxyza = xyz_a.data();
    XYZ* pxyzb = xyz_b.data();
    XYZ* prxyz = ret_xyz.data();
    {
        XYZ* mypxyza = pxyza;
        XYZ* mypxyzb = pxyzb;
        XYZ* myprxyz = prxyz;
        for( size_t i = 0; i < size; ++i )
        {
            myprxyz->x = mypxyza->x + mypxyzb->x;
            myprxyz->y = mypxyza->y + mypxyzb->y;
            ++mypxyza; ++mypxyzb; ++myprxyz;
        }
    }

    timer.start();
    for( size_t j = 0; j < 20000; ++j )
    {
        XYZ* mypxyza = pxyza;
        XYZ* mypxyzb = pxyzb;
        XYZ* myprxyz = prxyz;
        for( size_t i = 0; i < size; ++i )
        {
            myprxyz->x = mypxyza->x + mypxyzb->x;
            myprxyz->y = mypxyza->y + mypxyzb->y;
            ++mypxyza; ++mypxyzb; ++myprxyz;
        }
    }
    timer.stop();

    std::cout << "scale " << size << " points aos: " << timer.format();
}

void scale_soa()
{
    std::vector< float > x_a;
    std::vector< float > y_a;
    std::vector< float > x_b;
    std::vector< float > y_b;
    std::vector< float > ret_x;
    std::vector< float > ret_y;

    srand( 1 );

    size_t size = 512 * 1024;
    x_a.reserve( size );
    y_a.reserve( size );
    x_b.reserve( size );
    y_b.reserve( size );
    for( size_t i = 0; i < size; ++i )
    {
        x_a.push_back(  static_cast<float>( i ) );
        y_a.push_back(  static_cast<float>( i ) );
        x_b.push_back( -static_cast<float>( i ) );
        y_b.push_back( -static_cast<float>( i ) );
    }
    ret_x.resize( size, 0.f );
    ret_y.resize( size, 0. );

    boost::timer::cpu_timer timer;

    float *pxa = x_a.data();
    float *pya = y_a.data();
    float *pxb = x_b.data();
    float *pyb = y_b.data();
    float *prx = ret_x.data();
    float *pry = ret_y.data();
    {
        float *mypxa = pxa;
        float *mypya = pya;
        float *mypxb = pxb;
        float *mypyb = pyb;
        float *myprx = prx;
        float *mypry = pry;
        for( size_t i = 0; i < size; ++i )
        {
            *myprx = *mypxa + *mypxb;
            *mypry = *mypya + *mypyb;
            ++mypxa; ++mypxb; ++myprx;
            ++mypya; ++mypyb; ++mypry;
        }
    }
    timer.start();
    for( size_t j = 0; j < 20000; ++j )
    {
        float *mypxa = pxa;
        float *mypya = pya;
        float *mypxb = pxb;
        float *mypyb = pyb;
        float *myprx = prx;
        float *mypry = pry;
        for( size_t i = 0; i < size; ++i )
        {
            *myprx = *mypxa + *mypxb;
            *mypry = *mypya + *mypyb;
            ++mypxa; ++mypxb; ++myprx;
            ++mypya; ++mypyb; ++mypry;
        }
    }
    timer.stop();

    std::cout << "scale " << size << " points soa: " << timer.format();
}

namespace ccppbrasil {

class XY_aos
{
public:
    void reserve( size_t size )
    {
        xy_.reserve( size );
    }

    void push_back( float x, float y )
    {
        xy_.push_back( std::make_pair( x, y ) );
    }

    float get_x( size_t idx )
    {
        return xy_[ idx ].first;
    }

    float get_y( size_t idx )
    {
        return xy_[ idx ].second;
    }

private:
    std::vector< std::pair< float, float > > xy_;
};

class XY_soa
{
public:
    void reserve( size_t size )
    {
        x_.reserve( size );
        y_.reserve( size );
    }

    void push_back( float x, float y )
    {
        x_.push_back( x );
        y_.push_back( y );
    }

    float get_x( size_t idx )
    {
        return x_[ idx ];
    }

    float get_y( size_t idx )
    {
        return y_[ idx ];
    }

private:
    std::vector< float > x_;
    std::vector< float > y_;
};

}

int main( int argc, char* argv[] )
{
    boost::timer::cpu_timer timer;

    size_t ffsize = 100000000;
    size_t rewsize = 100000;


    // std::map
    for( int i = 0; i < 10; ++i )
    {
        std::map<size_t, size_t> std_map1;
        timer.start();
        forwardFill( std_map1, ffsize );
        timer.stop();
        std::cout << "forward fill std::map: " << timer.format();

        timer.start();
        forwardFind( std_map1, ffsize );
        timer.stop();
        std::cout << "forward find std::map: " << timer.format();
    }

    for( int i = 0; i < 10; ++i )
    {
        std::map<size_t, size_t> std_map2;
        timer.start();
		reverseFill( std_map2, rewsize );
        timer.stop();
        std::cout << "reverse fill std::map: " << timer.format();

        timer.start();
		reverseFind( std_map2, rewsize );
        timer.stop();
        std::cout << "reserve find std::map: " << timer.format();
    }


    // boost::container::flat_map
	for( int i = 0; i < 10; ++i )
	{
        boost::container::flat_map<size_t, size_t> flat_map1;
        flat_map1.reserve( ffsize );
        timer.start();
        forwardFill( flat_map1, ffsize );
        timer.stop();
        std::cout << "forward fill boost::container::flat_map: " << timer.format();

        timer.start();
        forwardFind( flat_map1, ffsize );
        timer.stop();
        std::cout << "forward find boost::container::flat_map: " << timer.format();
    }

	for( int i = 0; i < 10; ++i )
	{
        boost::container::flat_map<size_t, size_t> flat_map2;
        flat_map2.reserve( rewsize );
        timer.start();
        reverseFill( flat_map2, rewsize );
        timer.stop();
        std::cout << "reverse fill boost::container::flat_map: " << timer.format();

        timer.start();
        reverseFind( flat_map2, rewsize );
        timer.stop();
        std::cout << "reverse find boost::container::flat_map: " << timer.format();
    }

    // ccppbrasil::soa_map
	for( int i = 0; i < 10; ++i )
	{
        ccppbrasil::soa_map<size_t, size_t> soa_map1;
        soa_map1.reserve( ffsize );
        timer.start();
        forwardFill( soa_map1, ffsize );
        timer.stop();
        std::cout << "forward fill ccppbrasil::soa_map: " << timer.format();

        timer.start();
        forwardFind( soa_map1, ffsize );
        timer.stop();
        std::cout << "forward find ccppbrasil::soa_map: " << timer.format();
    }

	for( int i = 0; i < 10; ++i )
	{
        ccppbrasil::soa_map<size_t, size_t> soa_map2;
        soa_map2.reserve( rewsize );
        timer.start();
        reverseFill( soa_map2, rewsize );
        timer.stop();
        std::cout << "reverse fill ccppbrasil::soa_map: " << timer.format();

        timer.start();
        reverseFind( soa_map2, rewsize );
        timer.stop();
        std::cout << "reverse find ccppbrasil::soa_map: " << timer.format();
    }

	for( int i = 0; i < 10; ++i )
		least_square<ccppbrasil::XY_aos>( "aos" );
	for( int i = 0; i < 10; ++i )
		least_square<ccppbrasil::XY_soa>( "soa" );
    for( int i = 0; i < 10; ++i )
        scale_aos();
    for( int i = 0; i < 10; ++i )
        scale_soa();
    return 0;
}

