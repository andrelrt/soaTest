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

int main(int argc, char* argv[])
{
    boost::timer::cpu_timer timer;

    size_t size = 100'000;


    // std::map
    std::map<size_t, size_t> std_map1;
    timer.start();
    forwardFill( std_map1, size );
    timer.stop();
    std::cout << "forward std::map: " << timer.format() << std::endl;

    std::map<size_t, size_t> std_map2;
    timer.start();
    reverseFill( std_map2, size );
    timer.stop();
    std::cout << "reverse std::map: " << timer.format() << std::endl;


    // boost::container::flat_map
    boost::container::flat_map<size_t, size_t> flat_map1;
    flat_map1.reserve( size );
    timer.start();
    forwardFill( flat_map1, size );
    timer.stop();
    std::cout << "forward boost::container::flat_map: " << timer.format() << std::endl;

    boost::container::flat_map<size_t, size_t> flat_map2;
    flat_map2.reserve( size );
    timer.start();
    reverseFill( flat_map2, size );
    timer.stop();
    std::cout << "reverse boost::container::flat_map: " << timer.format() << std::endl;

    // ccppbrasil::soa_map
    ccppbrasil::soa_map<size_t, size_t> soa_map1;
    soa_map1.reserve( size );
    timer.start();
    forwardFill( soa_map1, size );
    timer.stop();
    std::cout << "forward ccppbrasil::soa_map: " << timer.format() << std::endl;

    ccppbrasil::soa_map<size_t, size_t> soa_map2;
    soa_map2.reserve( size );
    timer.start();
    reverseFill( soa_map2, size );
    timer.stop();
    std::cout << "reverse ccppbrasil::soa_map: " << timer.format() << std::endl;


    ccppbrasil::soa_map<size_t, size_t> soa_map_test;
    soa_map_test.reserve( 100 );
    reverseFill( soa_map_test, 100 );
    for( auto& pt : soa_map_test )
    {
        std::cout << "(" << pt.key() << "," << pt.value() << ")-";
    }
    
	return 0;
}

