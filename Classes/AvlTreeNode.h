/**
 * Definitions of AvlTreeNode class - implements a node of an AVL Tree data structure
 * specific for StringHashData score and frequency rankings.
 *
 *  Created on: July 7th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

#ifndef AVLTREENODE_H_
#define AVLTREENODE_H_

// Libraries:
#include "StringHashData.h"


/**
 * Enumerator to be used as argument to define AVL tree keys.
 */
enum SortBy { SCORE, FREQUENCY };


class AvlTreeNode {
public:
	/**
	 * Constructor: initializes pointers as null pointers.
	 */
	AvlTreeNode();

	/**
	 * Insert data into the AVL tree, returning pointer to tree's root.
	 *  @param data Hash table's satellite data to be inserted in AVL tree of rankings.
	 *  @return Pointer to AVL tree's root.
	 */
	AvlTreeNode* insert( StringHashData *data, SortBy key_order );

	/**
	 * Remove data from AVL tree, returning pointer to tree's root.
	 *  @param data Hash table's satellite data to be removed from AVL tree of rankings.
	 *  @return Pointer to AVL tree's root.
	 */
	AvlTreeNode* remove( StringHashData *data, SortBy key_order );

	/**
	 * Search for a given node in AVL tree of rankings.
	 *  @param data Hash table's satellite data to be searched for in AVL tree of rankings.
	 *  @return True if found, false otherwise.
	 */
	bool search( StringHashData *data, SortBy key_order );

	/**
	 * Set node's key.
	 *  @param key Node's key.
	 *  @return True if key is valid, false otherwise.
	 */
	bool setKey( double key );

	/**
	 * Get node's key.
	 *  @return Node's key.
	 */
	double getKey();

	/**
	 * Set node's data.
	 *  @param data Node's data.
	 *  @return True if data was set, false if pointer was null.
	 */
	bool setData( StringHashData *data );

	/**
	 * Get node's data.
	 *  @return Node's data.
	 */
	StringHashData* getData();

	/**
	 * Set node's left descendant.
	 *  @param Node's left descendant.
	 *  @return True if node was set, false if there's already a left descendant.
	 */
	void setLeftDescendant( AvlTreeNode *left_node );

	/**
	 * Set node's right descendant.
	 *  @param Node's right descendant.
	 *  @return True if node was set, false if there's already a right descendant.
	 */
	void setRightDescendant( AvlTreeNode *right_node );

	/**
	 * Get node's left and right descendants.
	 *  @return Pair of node's left and right descendants.
	 */
	std::pair< AvlTreeNode*, AvlTreeNode* > getDescendants();

	/**
	 * Set node's height.
	 *  @param height Node's height.
	 *  @return True if height is valid, false otherwise.
	 */
	bool setHeight( int height );

	/**
	 * Get node's height.
	 *  @return Node's height.
	 */
	int getHeight();

	/**
	 * Set node's balancing factor.
	 *  @param factor Node's balancing factor.
	 */
	void setFactor( int factor );

	/**
	 * Get node's balancing factor.
	 *  @return Node's balancing factor.
	 */
	int getFactor();

private:
	/**
	 * Update node's height (greatest height between descendants plus one).
	 */
	void updateHeight();

	/**
	 * Update node's factor (difference between descendants's heights).
	 */
	void updateFactor();

	/**
	 * Return greatest key among descendants taking this node as root.
	 *  @return Node with greatest key among descendants.
	 */
	AvlTreeNode* findGreatestKey();

	/**
	 * Manage rotations to maintain AVL tree balance.
	 *  @return Pointer to AVL tree's root.
	 */
	AvlTreeNode* rotate();

	/**
	 * Make a left rotation.
	 *  @return Pointer to AVL tree's root.
	 */
	AvlTreeNode* rotateLeft();

	/**
	 * Make a right rotation.
	 *  @return Pointer to AVL tree's root.
	 */
	AvlTreeNode* rotateRight();


	// Attributes:
	double key_;            // Node's key (determines position in AVL tree).
	StringHashData *data_;  // Hash table's satellite data.

	AvlTreeNode *left_;   // Descendant to the left (lower key).
	AvlTreeNode *right_;  // Descendant to the right (greater key).

	int height_;  // Node's height.
	int factor_;  // Node's balancing factor.

};


#endif /* AVLTREENODE_H_ */
