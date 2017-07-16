/**
 * Class StringHashTable member-function definitions.
 *
 *  Created on: July 15th, 2017
 *      Author: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include <string>
#include <stack>
#include <cmath>
#include "StringHashTable.h"

using namespace std;


StringHashTable::StringHashTable( unsigned size )
	: table_( size )  // Create hash table with given size.
{
	// No data is inserted.
	this->amount_of_data_ = 0;

	// No collisions occurred.
	this->collisions_ = 0;

	// Calculate maximum rate of occupation.
	calculateMaxRate();
}

bool StringHashTable::insert( std::string word, double phrase_score )
{
	// Hash value for given satellite data.
	unsigned hash_num;

	// If word isn't in the hash table:
	if( !search( word ) )
	{
		// Calculate h( hash_num ).
		hash_num = hash( word );

		// If hash table at hash_num is free, insert data.
		if( isFree( hash_num ) )
		{
			// Insert data in hash table.
			getDataRef( hash_num )->insert( word, phrase_score );
		}

		// Otherwise, a collision occurred, search for empty position.
		else
		{
			// Obtain an empty position using Double Hashing.
			hash_num = solveCollision( word );

			// Insert data in hash table.
			getDataRef( hash_num )->insert( word, phrase_score );
		}

		// Increment amount of data stored.
		this->amount_of_data_++;

		// If hash table's occupied space is greater than or equal to maximum rate,
		// expand it and reinsert data in appropriate positions.
		if( getUsedRate() >= getMaxRate() )
			rehash();

		// Indicate new data has been inserted.
		return true;
	}

	// Indicate data was already inserted.
	return false;
}

bool StringHashTable::search( std::string word, unsigned *key )
{
	// Hash value for word = h( word ).
	unsigned hash_num;
	// Double hash's round.
	unsigned round = 1;

	// Calculate h( word ).
	hash_num = hash( word );

	// Indicate that word has been found.
	bool is_found = false;

	// Search for word until either it's found or a position that wasn't
	// previously occupied is found.
	while( !is_found && getData( hash_num ).wasOccupied() )
	{
		// If word has been found, indicate so.
		if( getData( hash_num ).getWord() == word )
		{
			is_found = true;

			if( key != nullptr )
				*key = hash_num;
		}

		// Otherwise, recalculate hash using Double Hashing.
		else
		{
			hash_num = doubleHash( word, round );
			++round;
		}
	}

	// Return if the word was found.
	return is_found;
}

void StringHashTable::remove( std::string word )
{
	// Hash value for word.
	unsigned hash_num;

	// If word is in the tree, remove it.
	if( search( word, &hash_num ) )
	{
		getDataRef( hash_num )->remove();
		this->amount_of_data_--;
	}
}

StringHashData StringHashTable::getData( unsigned key )
{
	// Return satellite data at key position.
	return this->table_.at( key );
}

unsigned StringHashTable::getSize()
{
	// Return hash table's current size.
	return this->table_.size();
}

unsigned StringHashTable::getUsedAmount()
{
	// Return hash table's amount of occupied space.
	return this->amount_of_data_;
}

double StringHashTable::getMaxRate()
{
	// Return maximum rate of occupation.
	return this->max_rate_;
}

double StringHashTable::getUsedRate()
{
	// Return rate of hash table's used space.
	return static_cast< double >( getUsedAmount() ) / getSize();
}

unsigned StringHashTable::getCollisionsNum()
{
	// Return number of collisions occurred.
	return this->collisions_;
}

void StringHashTable::setData( StringHashData data, unsigned key )
{
	// Assign data to hash table's position.
	this->table_.at( key ) = data;
}

StringHashData* StringHashTable::getDataRef( unsigned key )
{
	// Return pointer to satellite data at position key.
	return &( this->table_.at( key ) );
}

void StringHashTable::clear( unsigned key )
{
	// Clear hash table's position.
	this->table_.at( key ).clear();
}

unsigned StringHashTable::hash( std::string word )
{
	unsigned hash_num = 0;    // Hash result for word = h( word ).
	const unsigned prime = 5; // Prime number for polynomial hashing function.
	unsigned power = 0;       // Power to raise prime to.

	// Iterator to get each character from word.
	string::iterator letter = word.begin();
	// Calculate hash for word using each character.
	while( letter != word.end() )
	{
		// h( word ) = c * p^i  |  c is a character from word; p is a prime number; i is a counter starting at 0.
		hash_num += *letter * pow( prime, power );

		// If h( word ) is currently greater than hash's size, calculate modulus by size.
		if( hash_num > getSize() )
			hash_num %= getSize();

		// Increment iterator.
		++letter;
		// Increment power.
		++power;
	}

	// Return h( word ).
	return hash_num;
}

unsigned StringHashTable::doubleHash( std::string word, unsigned round )
{
	unsigned hash_num = round; // Hash result for word = h( word ).
	const unsigned prime = 13; // Prime number for polynomial hashing function.
	unsigned power = 0;        // Power to raise prime to.

	// Iterator to get each character from word.
	string::iterator letter = word.begin();
	// Calculate hash for word using each character.
	while( letter != word.end() )
	{
		// h( word ) = c * p^i  |  c is a character from word; p is a prime number; i is a counter starting at 0.
		hash_num += *letter * pow( prime, power );

		// If h( word ) is currently greater than hash's size, calculate modulus by size.
		if( hash_num > getSize() )
			hash_num %= getSize();

		// Increment iterator.
		++letter;
		// Increment power.
		++power;
	}

	// Return h( word ).
	return hash_num;
}

unsigned StringHashTable::solveCollision( std::string word )
{
	unsigned hash_num = 0;   // Hash result for word = h( word ).
	unsigned round = 1;      // Double hashing number of calls so far.

	// Recalculate hashing function for word until a free space is found.
	do
	{
		// Increment number of collisions.
		this->collisions_++;
		// Calculate hash for word using a second hashing function.
		hash_num = doubleHash( word, round );
		// Increment amount of times doubleHash() was called.
		++round;
	} while( !isFree( hash_num ) );

	// Return free space in hash table.
	return hash_num;
}

bool StringHashTable::isFree( unsigned key )
{
	// Return true if space is free or false otherwise.
	return getData( key ).getWord().empty();
}

void StringHashTable::rehash()
{
	// Hash table's original size.
	unsigned old_size = getSize();

	// Double hash table's size.
	this->table_.reserve( getSize() * 2 );
	this->table_.resize( getSize() * 2 );

	// AVL Tree to indicate already reinserted elements.
	AvlTree reinserted( SortBy::SCORE );

	// Recalculate hashing function for each satellite data.
	for( unsigned i = 0; i < old_size; ++i )
	{
		// If there's an element in this position and it wasn't reinserted, reinsert it.
		if( !isFree( i ) && !reinserted.search( getDataRef( i ) ) )
			reinsert( i, reinserted );

		// If there's no element in this position, but it was previously occupied, clear it.
		else if( isFree( i ) && getData( i ).wasOccupied() )
			clear( i );
	}
}

void StringHashTable::reinsert( unsigned key, AvlTree &reinserted )
{
	// Copy of data to be reinserted.
	StringHashData data_copy = getData( key );
	// Amount of times hashing function was called.
	unsigned round = 1;

	// Clear data from the hash table at key.
	clear( key );

	// Calculate hashing function for word at position key.
	unsigned hash_num = hash( data_copy.getWord() );

	// Indicate data was reinserted.
	bool was_reinserted = false;

	while( !was_reinserted )
	{
		// If calculated position is free, insert data.
		if( isFree( hash_num ) )
		{
			setData( data_copy, hash_num );

			// Indicate data has been reinserted.
			reinserted.insert( getDataRef( hash_num ) );
			was_reinserted = true;
		}

		// If it is occupied, check if data in position hasn't been reinserted.
		else if( !reinserted.search( getDataRef( hash_num ) ) )
			reinsert( hash_num, reinserted );

		// Otherwise, it is occupied and a collision occurred - recalculate hash value.
		else
		{
			hash_num = doubleHash( data_copy.getWord(), round );
			++round;
		}
	}
}

void StringHashTable::calculateMaxRate()
{
	// Calculate 70% of hash table's size rounded down.
	double num_of_elements = floor( getSize() * 0.7 );

	// Calculate hash table's maximum rate of occupation.
	this->max_rate_ = num_of_elements / getSize();
}
