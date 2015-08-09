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

#include <boost/timer/timer.hpp>
#include <boost/container/flat_map.hpp>

#include "soa_map.h"
#include "XY.h"

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

    srand( static_cast<unsigned>( time(NULL) ) );

    size_t size = 500'000'000;
    xy.reserve( size );
    for( size_t i = 0; i < size; ++i )
    {
        xy.push_back( i + ( 2.f * rand() - RAND_MAX / 2.f ) / RAND_MAX ,
                      i + ( 4.f * rand() - RAND_MAX / 2.f ) / RAND_MAX );
    }

    boost::timer::cpu_timer timer;

    timer.start();
    double meanx = 0;
    double meany = 0;
    for( size_t i = 0; i < size; ++i )
    {
        meanx += xy.get_x( i );
        meany += xy.get_y( i );
    }

    meanx /= size;
    meany /= size;

    double numerator = 0;
    double denominator = 0;
    for( size_t i = 0; i < size; ++i )
    {
        double diffx = ( xy.get_x( i ) - meanx );
        numerator += diffx * ( xy.get_y( i ) - meany );
        denominator += diffx * diffx;
    }

    double b = numerator / denominator;
    double a = meany - b * meanx;
    timer.stop();

    std::cout << "Least Square linear regression " << name << ": y = " << a << " + " << b << "x + e : "
              << timer.format();
}

int main(int argc, char* argv[])
{
    boost::timer::cpu_timer timer;

    size_t ffsize = 100'000'000;
    size_t rewsize = 100'000;


    // std::map
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


    least_square<ccppbrasil::XY_aos>( "aos" );
    least_square<ccppbrasil::XY_soa>( "soa" );
    return 0;
}

