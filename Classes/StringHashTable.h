/**
 * Definitions of StringHashTable class - a hash table for strings.
 *
 *  Created on: July 7th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

#ifndef STRINGHASHTABLE_H_
#define STRINGHASHTABLE_H_

// Libraries:
#include <string>
#include <vector>
#include "AvlTree.h"


class StringHashTable {
public:
	/**
	 * Constructor: create hash table with given size and initialize ranking trees.
	 *  @param size Hash table's initial size.
	 */
	StringHashTable( unsigned size );

	/**
	 * Insert string in hash table at given index.
	 *  @param word Word to be inserted.
	 *  @param phrase_score Word's score in the phrase.
	 *  @return True if inserted, false otherwise.
	 *  @attention May call solveCollision() or rehash() methods.
	 */
	bool insert( std::string word, double phrase_score );

	/**
	 * Search for given string in hash table.
	 *  @param word Word to find.
	 *  @param key Word's key in the hash table if found (optional).
	 *  @return True if found, false otherwise.
	 */
	bool search( std::string word, unsigned *key = nullptr );

	/**
	 * Remove string from hash table at given index.
	 *  @param word Word to be removed.
	 */
	void remove( std::string word );

	/**
	 * Get satellite data at key position.
	 *  @param key Position where data is in hash table.
	 *  @return Satellite data at key position.
	 */
	StringHashData getData( unsigned key );

	/**
	 * Get hash table's current size.
	 *  @return Hash table's current size.
	 */
	unsigned getSize();

	/**
	 * Get amount of occupied space in hash table.
	 *  @return Amount of occupied space.
	 */
	unsigned getUsedAmount();

	/**
	 * Get hash table's maximum rate of occupied space allowed before rehashing.
	 *  @return Maximum rate of occupation.
	 */
	double getMaxRate();

	/**
	 * Get rate of hash table's occupied space.
	 *  @return Rate of hash table's occupied space.
	 */
	double getUsedRate();

	/**
	 * Get number of collisions occurred.
	 *  @return Number of collisions.
	 */
	unsigned getCollisionsNum();

private:
	/**
	 * Set satellite data to key position.
	 *  @param data Satellite data to be inserted.
	 *  @param key Position in hash table.
	 */
	void setData( StringHashData data, unsigned key );

	/**
	 * Get satellite data at position key by reference.
	 *  @param key Position in hash table.
	 *  @return Pointer to satellite data.
	 *  @attention This method breaks encapsulation, use it carefully.
	 */
	StringHashData* getDataRef( unsigned key );

	/**
	 * Clear hash table at position key.
	 *  @param key Position to be cleared.
	 */
	void clear( unsigned key );

	/**
	 * Hashing function for a given string.
	 *  @param word String to be inserted in hash table.
	 *  @return Index for insertion in hash table.
	 */
	unsigned hash( std::string word );

	/**
	 * A second hashing function for a given string.
	 *  @param word Word to calculate hashing function.
	 *  @param round Amount of times hashing function was called so far.
	 *  @return Index for insertion in hash table.
	 */
	unsigned doubleHash( std::string word, unsigned round );

	/**
	 * Find a free space to insert data using Double Hashing.
	 *  @param word Word to recalculate hashing function.
	 *  @return Key to a free space.
	 */
	unsigned solveCollision( std::string word );

	/**
	 * Check if hash table at a given key is free.
	 *  @param key Hash table's key to check.
	 *  @return True if space is free, false otherwise.
	 */
	bool isFree( unsigned key );

	/**
	 * Reallocate the entire hash table. Operation has a high cost and should be used
	 * only if used_rate is ~0.7.
	 */
	void rehash();

	/**
	 * Reinsert satellite data at given key while rehashing.
	 *  @param key Hash table's key to data.
	 *  @param reinserted Tree of already reinserted data.
	 */
	void reinsert( unsigned key, AvlTree &reinserted );

	/**
	 * Calculate optimal percentage of occupation to rehash.
	 */
	void calculateMaxRate();


// Attributes:
	unsigned amount_of_data_;  // Amount of data stored in the hash table.
	unsigned collisions_;      // Number of collisions.
	double max_rate_;          // Maximum rate of occupation.

	std::vector< StringHashData > table_;
};


#endif /* STRINGHASHTABLE_H_ */
