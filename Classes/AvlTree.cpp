/**
 * Class AvlTree member-function definitions.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include "AvlTree.h"

using namespace std;


AvlTree::AvlTree( SortBy order )
{
	// Set root pointer as null.
	this->root_ = nullptr;
	// Set sorting order of keys.
	this->key_order_ = order;			// TODO: exception handling for invalid order.
}

AvlTree::~AvlTree()
{
	// Call clear() method to remove all nodes.
	clear();
}

void AvlTree::insert( StringHashData *data )
{
	// If AVL tree is empty, allocate memory for root node.
	if( isEmpty() )
		this->root_ = new AvlTreeNode;

	// Insert node with data and maintain correct root (may change if rotation occurs).
	setRoot( this->root_->insert( data, getKeyOrder() ) );
}

void AvlTree::remove( StringHashData *data )
{
	// If AVL tree isn't empty and data is in the tree, remove it while maintaining
	// correct root (may change if root is removed or rotation occurs).
	if( !isEmpty() && search( data ) )
		setRoot( this->root_->remove( data, getKeyOrder() ) );
}

void AvlTree::clear()
{
	// Call remove() method for all nodes.
	while( !isEmpty() )
		remove( getRoot()->getData() );
}

bool AvlTree::search( StringHashData *data )
{
	// If AVL tree isn't empty, search for node with data.
	if( !isEmpty() )
		return getRoot()->search( data, getKeyOrder() );

	// Otherwise, return false to indicate tree is empty.
	return false;
}

bool AvlTree::isEmpty()
{
	// If root is a null pointer, the tree is empty.
	return ( getRoot() == nullptr );
}

void AvlTree::setRoot( AvlTreeNode *new_root )
{
	// Assign new_root as tree's root.
		this->root_ = new_root;
}

AvlTreeNode* AvlTree::getRoot()
{
	// Return root node.
	return this->root_;
}

SortBy AvlTree::getKeyOrder()
{
	// Return sorting order.
	return this->key_order_;
}
