/**
 * Definitions of AvlTree class - implements an AVL Tree data structure specific for
 * StringHashData score and frequency rankings.
 *
 *  Created on: July 7th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

#ifndef AVLTREE_H_
#define AVLTREE_H_

// Libraries:
#include "AvlTreeNode.h"


class AvlTree {
public:
	/**
	 * Constructor: initializes AVL tree.
	 *  @param order Sorting order for this tree's keys.
	 *  @attention Possible sorting orders are SCORE and FREQUENCY.
	 */
	AvlTree( SortBy order );

	/**
	 * Destructor: deallocates dynamic memory used.
	 */
	~AvlTree();

	/**
	 * Insert data into the AVL tree.
	 *  @param data Hash table's satellite data to be inserted in AVL tree of rankings.
	 */
	void insert( StringHashData *data );

	/**
	 * Remove data from AVL tree.
	 *  @param data Hash table's satellite data to be removed from AVL tree of rankings.
	 */
	void remove( StringHashData *data );

	/**
	 * Clear all data from AVL tree.
	 *  @attention Sorting order is kept.
	 */
	void clear();

	/**
	 * Search for a given node in AVL tree.
	 *  @param data Hash table's satellite data to be searched for in AVL tree of rankings.
	 *  @return True if found, false otherwise.
	 */
	bool search( StringHashData *data );

	/**
	 * Check if AVL tree is empty.
	 *  @return True if is empty, false otherwise.
	 */
	bool isEmpty();

	/**
	 * Get AVL tree's root.
	 *  @return AVL tree's root node.
	 */
	AvlTreeNode* getRoot();

	/**
	 * Get sorting order of keys.
	 */
	SortBy getKeyOrder();

	/**
	 * Print rank_size words with the greatest keys.
	 *  @param rank_size Amount of words in ranking list.
	 */
	void printGreatestKeys( unsigned rank_size );

	/**
	 * Print rank_size words with the lowest keys.
	 *  @param rank_size Amount of words in ranking list.
	 */
	void printLowestKeys( unsigned rank_size );

private:
	/**
	 * Set AVL tree's root.
	 *  @param new_root New root node.
	 */
	void setRoot( AvlTreeNode *new_root );

	AvlTreeNode *root_; // AVL tree's root.
	SortBy key_order_;  // Sorting order of keys.

	//int height_;  // TODO: get tree's height.
	//int factor_;  // TODO: get tree's balancing factor.
};


#endif /* AVLTREE_H_ */
