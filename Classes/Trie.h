/**
 * Definitions of Trie class - implements a Trie data structure.
 *
 *  Created on: July 19th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

#ifndef TRIE_H_
#define TRIE_H_

// Libraries:
#include <string>
#include "TrieNode.h"


class Trie {
public:
	/**
	 * Constructor: initializes Trie.
	 */
	Trie();

	/**
	 * Insert data into the Trie tree.
	 *  @param word - string to be inserted at the Trie.
	 */
	void insert( std::string word );

	/**
	 * Search for a given node in AVL tree.
	 *  @param data Hash table's satellite data to be searched for in AVL tree of rankings.
	 *  @return True if found, false otherwise.
	 */
	bool search( std::string prefix );

	/**
	 * Check if Trie is empty.
	 *  @return True if is empty, false otherwise.
	 */
	bool isEmpty();

	/**
	 * Get Trie tree's root.
	 *  @return Trie tree's root node.
	 *  @attention This method breaks encapsulation, use it carefully.
	 */
	TrieNode* getRoot();

	/**
	 * Set Trie tree's root.
	 *  @param new_root New root node.
	 */
	void setRoot( TrieNode *new_root );

private:
	TrieNode *root_; // Trie tree's root.
};

#endif /* AVLTREE_H_ */
