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

void least_square_aos()
{
    struct XY
    {
        float x;
        float y;
    };

    std::vector< XY > xy;

    srand( static_cast<unsigned>( time(NULL) ) );

    size_t size = 500'000'000;
    xy.reserve( size );
    for( size_t i = 0; i < size; ++i )
    {
        xy.push_back( { i + ( 2.f * rand() - RAND_MAX / 2.f ) / RAND_MAX ,
                        i + ( 4.f * rand() - RAND_MAX / 2.f ) / RAND_MAX } );
    }

    boost::timer::cpu_timer timer;

    timer.start();
    float meanx = 0;
    float meany = 0;
    for( size_t i = 0; i < size; ++i )
    {
        meanx += xy[ i ].x;
        meany += xy[ i ].y;
    }

    meanx /= size;
    meany /= size;

    float numerator = 0;
    float denominator = 0;
    for( size_t i = 0; i < size; ++i )
    {
        float diffx = ( xy[ i ].x - meanx );
        numerator += diffx * ( xy[ i ].y - meany );
        denominator += diffx * diffx;
    }

    float b = numerator / denominator;
    float a = meany - b * meanx;
    timer.stop();

    std::cout << "Least Square linear regression aos: y = " << a << " + " << b << "x + e : "
              << timer.format();

}


void least_square_soa()
{
    std::vector< float > x;
    std::vector< float > y;

    srand( static_cast<unsigned>( time( NULL ) ) );

    size_t size = 500'000'000;
    x.reserve( size );
    y.reserve( size );
    for( size_t i = 0; i < size; ++i )
    {
        x.push_back( i + ( 2.f * rand() - RAND_MAX / 2.f ) / RAND_MAX );
        y.push_back( i + ( 4.f * rand() - RAND_MAX / 2.f ) / RAND_MAX );
    }

    boost::timer::cpu_timer timer;

    timer.start();
    float meanx = 0;
    float meany = 0;
    for( size_t i = 0; i < size; ++i )
    {
        meanx += x[i];
        meany += y[i];
    }

    meanx /= size;
    meany /= size;

    float numerator = 0;
    float denominator = 0;
    for( size_t i = 0; i < size; ++i )
    {
        float diffx = ( x[i] - meanx );
        numerator += diffx * ( y[i] - meany );
        denominator += diffx * diffx;
    }

    float b = numerator / denominator;
    float a = meany - b * meanx;
    timer.stop();

    std::cout << "Least Square linear regression soa: y = " << a << " + " << b << "x + e : "
        << timer.format();

}


int main(int argc, char* argv[])
{
    boost::timer::cpu_timer timer;

    size_t size = 100'000;


    // std::map
    {
        std::map<size_t, size_t> std_map1;
        timer.start();
        forwardFill( std_map1, size );
        timer.stop();
        std::cout << "forward std::map: " << timer.format();
    }

    {
        std::map<size_t, size_t> std_map2;
        timer.start();
        reverseFill( std_map2, size );
        timer.stop();
        std::cout << "reverse std::map: " << timer.format();
    }


    // boost::container::flat_map
    {
        boost::container::flat_map<size_t, size_t> flat_map1;
        flat_map1.reserve( size );
        timer.start();
        forwardFill( flat_map1, size );
        timer.stop();
        std::cout << "forward boost::container::flat_map: " << timer.format();
    }

    {
        boost::container::flat_map<size_t, size_t> flat_map2;
        flat_map2.reserve( size );
        timer.start();
        reverseFill( flat_map2, size );
        timer.stop();
        std::cout << "reverse boost::container::flat_map: " << timer.format();
    }

    // ccppbrasil::soa_map
    {
        ccppbrasil::soa_map<size_t, size_t> soa_map1;
        soa_map1.reserve( size );
        timer.start();
        forwardFill( soa_map1, size );
        timer.stop();
        std::cout << "forward ccppbrasil::soa_map: " << timer.format();
    }

    {
        ccppbrasil::soa_map<size_t, size_t> soa_map2;
        soa_map2.reserve( size );
        timer.start();
        reverseFill( soa_map2, size );
        timer.stop();
        std::cout << "reverse ccppbrasil::soa_map: " << timer.format();
    }


    least_square_aos();
    least_square_soa();
    return 0;
}

