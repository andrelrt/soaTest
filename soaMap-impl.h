/*
 * Copyright (c) 2014 André Tupinambá (andrelrt@gmail.com)
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

namespace ccppbrasil {

//-------------------------------------------------------------------------------------------------
// soa_iterator
//-------------------------------------------------------------------------------------------------
soa_iterator::soa_iterator( value_type& obj, size_t pos ) : 
    boost::counting_iterator<size_t>( pos ),
    soa_map_( obj )
{
}

//-------------------------------------------------------------------------------------------------
void soa_iterator::swap( soa_iterator& other )
{
    std::swap( soa_map_, other.soa_map_ );
    std::swap( base(), other.base() );
}

//-------------------------------------------------------------------------------------------------
KeyType& soa_iterator::key()
{
    return soa_map_.keyAtIndex( base() );
}

//-------------------------------------------------------------------------------------------------
const KeyType& soa_iterator::key() const
{
    return soa_map_.keyAtIndex( base() );
}

//-------------------------------------------------------------------------------------------------
ValueType& soa_iterator::value()
{
    return soa_map_.atIndex( base() );
}

//-------------------------------------------------------------------------------------------------
const ValueType& soa_iterator::value() const
{
    return soa_map_.atIndex( base() );
}

//-------------------------------------------------------------------------------------------------
std::pair< KeyType&, ValueType& > operator*()
{
    return std::make_pair( key(), value() );
}

//-------------------------------------------------------------------------------------------------
std::pair< const KeyType&, const ValueType& > operator*() const
{
    return std::make_pair( key(), value() );
}

//-------------------------------------------------------------------------------------------------
std::pair< KeyType&, ValueType& > operator->()
{
    return std::make_pair( key(), value() );
}

//-------------------------------------------------------------------------------------------------
std::pair< const KeyType&, const ValueType& > operator->() const
{
    return std::make_pair( key(), value() );
}


//-------------------------------------------------------------------------------------------------
// soa_map
//-------------------------------------------------------------------------------------------------
void soa_map::reserve( size_t capacity )
{
    key_container_.reserve( capacity );
    value_container_.reserve( capacity );
}

//-------------------------------------------------------------------------------------------------
size_t soa_map::size() const
{
    return key_container_.size();
}

//-------------------------------------------------------------------------------------------------
bool soa_map::empty() const
{
    return key_container_.empty();
}

//-------------------------------------------------------------------------------------------------
void soa_map::clear()
{
    key_container_.clear();
    value_container_.clear();
}

//-------------------------------------------------------------------------------------------------
std::pair< iterator, bool > soa_map::insert( const std::pair< KeyType, ValueType > &keyValuePair )
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), keyValuePair.first, KeyCompare() );

    if( key_container_.end() != pos && keyValuePair.first == *pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return std::make_pair( iterator( *this, idx ), false );
    }

    pos = key_container_.insert( pos, keyValuePair.first );
    size_t idx = std::distance( key_container_.begin(), pos );
    
    auto valPos = value_container_.begin();
    std::advance( valPos, idx );
    value_container_.insert( valPos, keyValuePair.second );
    
    return std::make_pair( iterator( *this, idx ), true );
}

//-------------------------------------------------------------------------------------------------
template<class K, class V> 
std::pair< iterator, bool > soa_map::emplace( K && _key, V && value )
{
    KeyType key( std::forward<K>(_key) );
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );

    if( key_container_.end() != pos && key == *pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return std::make_pair( iterator( *this, idx ), false );
    }

    pos = key_container_.insert( pos, std::move( key ) );
    size_t idx = std::distance( key_container_.begin(), pos );
    
    auto valPos = value_container_.begin();
    std::advance( valPos, idx );
    value_container_.insert( valPos, std::forward<V>( value ) );
    
    return std::make_pair( iterator( *this, idx ), true );
}

//-------------------------------------------------------------------------------------------------
iterator soa_map::erase( const KeyType &key )
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );

    if( key_container_.end() != pos && key == *pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        key_container_.erase( pos );
        
        auto valPos = value_container_.begin();
        std::advance( valPos, idx );
        value_container_.erase( valPos );
        
        return iterator( *this, idx );
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
void soa_map::swap( soa_map &other )
{
    key_container_.swap( other.key_container_ );
    value_container_.swap( other.value_container_ );
}

//-------------------------------------------------------------------------------------------------
const KeyType& soa_map::keyAtIndex( size_t index ) const
{
    return key_container_.at( index );
}

//-------------------------------------------------------------------------------------------------
ValueType& soa_map::atIndex( size_t index )
{
    return value_container_.at( index );
}

//-------------------------------------------------------------------------------------------------
const ValueType& soa_map::atIndex( size_t index ) const
{
    return value_container_.at( index );
}

//-------------------------------------------------------------------------------------------------
ValueType& soa_map::at( const KeyType & key )
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );

    if( key_container_.end() != pos && key == *pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return value_container_[ idx ];
    }
    throw std::out_of_range( "" );
}

//-------------------------------------------------------------------------------------------------
const ValueType& soa_map::at( const KeyType & key ) const
{
    return const_cast< ValueType & >( const_cast< const soa_map* >( this )->at( key ) );
}

//-------------------------------------------------------------------------------------------------
ValueType& soa_map::operator[]( const KeyType &key )
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );

    if( key_container_.end() != pos && key == *pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return value_container_[ idx ];
    }
    
    auto pos = emplace( key, ValueType() );
    return pos->first;
}

//-------------------------------------------------------------------------------------------------
iterator soa_map::begin()
{
    return iterator( *this, 0 );
}

//-------------------------------------------------------------------------------------------------
const_iterator soa_map::begin() const
{
    return const_iterator( *this, 0 );
}

//-------------------------------------------------------------------------------------------------
iterator soa_map::end()
{
    return iterator( *this, size() );
}

//-------------------------------------------------------------------------------------------------
const_iterator soa_map::end() const
{
    return const_iterator( *this, size() );
}

//-------------------------------------------------------------------------------------------------
iterator soa_map::lower_bound( const KeyType &key )
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
const_iterator soa_map::lower_bound( const KeyType &key ) const
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
iterator soa_map::upper_bound( const KeyType &key )
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
const_iterator soa_map::upper_bound( const KeyType &key ) const
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
iterator soa_map::find( const KeyType &key )
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );
    if( key_container_.end() != pos && key == *pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return iterator( *this, idx );
    }
    return end();
}

//-------------------------------------------------------------------------------------------------
const_iterator soa_map::find( const KeyType &key ) const
{
    auto pos = std::lower_bound( key_container_.begin(), key_container_.end(), key, KeyCompare() );
    if( key_container_.end() != pos && key == *pos )
    {
        size_t idx = std::distance( key_container_.begin(), pos );
        return const_iterator( *this, idx );
    }
    return end();
}

} //namespace ccppbrasil

#endif // CCPPBRASIL_SOAMAP_IMPL_H
