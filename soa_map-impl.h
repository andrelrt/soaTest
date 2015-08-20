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
#ifndef CCPPBRASIL_SOAMAP_IMPL_H
#define CCPPBRASIL_SOAMAP_IMPL_H

#include <algorithm>

namespace ccppbrasil {

//-------------------------------------------------------------------------------------------------
// soa_pair
//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
soa_pair<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::soa_pair( value_type& obj, size_t pos ) :
    soa_map_( obj ), pos_(pos)
{
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
bool soa_pair<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::operator<( const my_type& other ) const
{
    return( KeyCompare()( key(), other.key ) );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const KeyType& soa_pair<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::key() const
{
    return soa_map_.keyAtIndex( pos_ );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
ValueType& soa_pair<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::value()
{
    return soa_map_.atIndex( pos_ );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const ValueType& soa_pair<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::value() const
{
    return soa_map_.atIndex( pos_ );
}

//-------------------------------------------------------------------------------------------------
// soa_iterator
//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::soa_iterator( value_type& obj, size_t pos ) :
    boost::counting_iterator<size_t>( pos ),
    soa_map_( obj )
{
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
void soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::swap( soa_iterator& other )
{
    std::swap( soa_map_, other.soa_map_ );
    std::swap( base(), other.base() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const KeyType& soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::key() const
{
    return soa_map_.keyAtIndex( base() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
ValueType& soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::value()
{
    return soa_map_.atIndex( base() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const ValueType& soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::value() const
{
    return soa_map_.atIndex( base() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator > soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::operator*()
{
    return soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >( soa_map_, base() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator > soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::operator*() const
{
    return soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >( soa_map_, base() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator > soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::operator->()
{
    return soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >( soa_map_, base() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator > soa_iterator<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::operator->() const
{
    return soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >( soa_map_, base() );
}


//-------------------------------------------------------------------------------------------------
// soa_map
//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
void soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::reserve( size_t capacity )
{
    key_container_.reserve( capacity );
    value_container_.reserve( capacity );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
size_t soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::size() const
{
    return key_container_.size();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
bool soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::empty() const
{
    return key_container_.empty();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
void soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::clear()
{
    key_container_.clear();
    value_container_.clear();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
bool soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::insert( const std::pair< KeyType, ValueType > &keyValuePair )
{
    typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator pos = lower_bound( keyValuePair.first );

    if( end() != pos && keyValuePair.first == pos.key() )
    {
        return false;
    }

    size_t idx = pos.base();
    auto keyPos = key_container_.begin();
    std::advance( keyPos, idx );
    key_container_.insert( keyPos, keyValuePair.first );
    
    auto valPos = value_container_.begin();
    std::advance( valPos, idx );
    value_container_.insert( valPos, keyValuePair.second );
    
    return true;
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
bool soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::emplace( KeyType && refKey, ValueType && value )
{
    KeyType key( std::forward<KeyType>(refKey) );
    typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator pos = lower_bound( key );

    if( end() != pos && key == pos.key() )
    {
        return false;
    }

    size_t idx = pos.base();
    auto keyPos = key_container_.begin();
    std::advance( keyPos, idx );
    key_container_.insert( keyPos, key );

    auto valPos = value_container_.begin();
    std::advance( valPos, idx );
    value_container_.insert( valPos, std::forward<ValueType>( value ) );

    return true;
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator 
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::erase( const KeyType &key )
{
    auto pos = find( key );

    if( end() != pos )
    {
        size_t idx = pos.base();

        auto keyPos = key_container_.begin();
        std::advance( keyPos, idx );
        key_container_.erase( keyPos );
        
        auto valPos = value_container_.begin();
        std::advance( valPos, idx );
        value_container_.erase( valPos );
        
        return iterator( *this, idx );
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
void soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::swap( soa_map &other )
{
    key_container_.swap( other.key_container_ );
    value_container_.swap( other.value_container_ );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const KeyType& soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::keyAtIndex( size_t index ) const
{
    return key_container_.at( index );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
ValueType& soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::atIndex( size_t index )
{
    return value_container_.at( index );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const ValueType& soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::atIndex( size_t index ) const
{
    return value_container_.at( index );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
ValueType& soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::at( const KeyType & key )
{
    auto pos = find( key );

    if( end() != pos )
    {
        return pos.value();
    }
    throw std::out_of_range( "" );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const ValueType& soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::at( const KeyType & key ) const
{
    return const_cast< ValueType & >( const_cast< const soa_map* >( this )->at( key ) );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
ValueType& soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::operator[]( const KeyType &key )
{
    auto pos = find( key );

    if( end() != pos )
    {
        return pos.value();
    }
    
    insert( { key, ValueType() } );
    pos = find( key );
    return pos.value();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::begin()
{
    return iterator( *this, 0 );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::const_iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::begin() const
{
    return const_iterator( *this, 0 );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::end()
{
    return iterator( *this, size() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::const_iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::end() const
{
    return const_iterator( *this, size() );
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::lower_bound( const KeyType &key )
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );
    if( key_container_.end() != pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return iterator( *this, idx );
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::const_iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::lower_bound( const KeyType &key ) const
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );
    if( key_container_.end() != pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return const_iterator( *this, idx );
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::upper_bound( const KeyType &key )
{
    auto pos = std::upper_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );
    if( key_container_.end() != pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return iterator( *this, idx );
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::const_iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::upper_bound( const KeyType &key ) const
{
    auto pos = std::upper_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );
    if( key_container_.end() != pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return const_iterator( *this, idx );
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::find( const KeyType &key )
{
    auto pos = lower_bound( key );
    if( end() != pos && key == pos.key() )
    {
        return pos;
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
const typename soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::const_iterator
soa_map<KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >::find( const KeyType &key ) const
{
    auto pos = lower_bound( key );
    if( end() != pos && key == pos.key() )
    {
        return pos;
    }
    return end();
}

} //namespace ccppbrasil

namespace std {

template< class KeyType, class ValueType, class KeyCompare, class KeyAllocator, class ValueAllocator >
void swap( ::ccppbrasil::soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >& lhs,
           ::ccppbrasil::soa_pair< KeyType, ValueType, KeyCompare, KeyAllocator, ValueAllocator >& rhs )
{
    std::swap( lhs.key(), rhs.key() );
    std::swap( lhs.value(), rhs.value() );
}

} // namespace std


#endif // CCPPBRASIL_SOAMAP_IMPL_H
