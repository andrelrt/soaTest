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
#ifndef CCPPBRASIL_SOAMAP_H
#define CCPPBRASIL_SOAMAP_H

#include <vector>
#include <boost/iterator/counting_iterator.hpp>

namespace ccppbrasil {

template< class KeyType,
		  class ValueType,
		  class KeyCompare = std::less< KeyType >,
		  class KeyAllocator = std::allocator< KeyType >,
		  class ValueAllocator = std::allocator< ValueType > >
class soa_map;

template< class KeyType,
		  class ValueType,
		  class KeyCompare,
		  class KeyAllocator,
		  class ValueAllocator >
class soa_iterator : public boost::counting_iterator<size_t>
{
public:
    typedef soa_map< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator > value_type;
    
    soa_iterator( value_type& obj, size_t pos );
    
    void swap( soa_iterator& other );
    
    KeyType& key();
    const KeyType& key() const;

    ValueType& value();
    const ValueType& value() const;
    
    std::pair< KeyType&, ValueType& > operator*();
    std::pair< const KeyType&, const ValueType& > operator*() const;

    std::pair< KeyType&, ValueType& > operator->();
    std::pair< const KeyType&, const ValueType& > operator->() const;
    
private:
    value_type& soa_map_;
};

template< class KeyType,
		  class ValueType,
		  class KeyCompare,
		  class KeyAllocator,
		  class ValueAllocator >
class soa_map
{
public:
   	typedef soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >            iterator;
   	typedef const soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >      const_iterator;

	void reserve( size_t capacity );
	size_t size() const;
	bool empty() const;
	void clear();

	std::pair< iterator, bool > insert( const std::pair< KeyType, ValueType > &keyValuePair );

    std::pair< iterator, bool > emplace( KeyType && _key, ValueType && value );

	iterator erase( const KeyType &key );

	void swap( soa_map &other );

	const KeyType& keyAtIndex( size_t index ) const;

	ValueType& atIndex( size_t index );
	const ValueType& atIndex( size_t index ) const;

	ValueType& at( const KeyType & key );
	const ValueType& at( const KeyType & key ) const;

	ValueType& operator[]( const KeyType &key );

	iterator begin();
	const_iterator begin() const;

	iterator end();
	const_iterator end() const;

	iterator lower_bound( const KeyType &key );
	const_iterator lower_bound( const KeyType &key ) const;

	iterator upper_bound( const KeyType &key );
	const_iterator upper_bound( const KeyType &key ) const;

	iterator find( const KeyType &key );
	const_iterator find( const KeyType &key ) const;
    
private:
    typedef std::vector< KeyType, KeyAllocator > key_container_type;
    typedef std::vector< ValueType, ValueAllocator > value_container_type;

    friend class soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >;
    
    key_container_type key_container_;
    value_container_type value_container_;
};

}

#include "soa_map-impl.h"

#endif // CCPPBRASIL_SOAMAP_H
