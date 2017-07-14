/**
 * Class AvlTreeNode member-function definitions.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include "../Classes/AvlTreeNode.h"
#include <stack>
// DEBUG //
#include <iostream>

using namespace std;


AvlTreeNode::AvlTreeNode()
{
	// Initialize node's key to indicate there's no data.
	this->key_ = -1.0;

	// Set data as empty.
	this->data_ = nullptr;

	// Set descendants as empty.
	this->left_ = nullptr;
	this->right_ = nullptr;

	this->height_ = 0;  // Initial height.
	this->factor_ = 0;  // Initial balancing factor.
}

AvlTreeNode::~AvlTreeNode()
{
	// If both descendants have been removed, it's safe to deallocate memory.
	if( this->left_ == nullptr && this->right_ == nullptr )
		delete this;  // TODO: make sure this releases memory allocated for this object.
	// Otherwise, deallocate memory for descendants first.
	else
	{
		if( this->left_ != nullptr )
			this->left_->~AvlTreeNode();

		if( this->right_ != nullptr )
			this->right_->~AvlTreeNode();

		delete this;  // TODO: make sure this releases memory allocated for this object.
	}
}

AvlTreeNode* AvlTreeNode::insert( StringHashData *data, SortBy key_order )
{
	// New node's key.
	double new_key = ( key_order == SortBy::SCORE ) ?
			data->getScore() : static_cast< double >( data->getFrequency() );

	// Stack to traverse tree and adjust nodes's heights and balancing factors.
	stack< AvlTreeNode* > tree_traversal;

	// Indicate if node's insertion position has been found.
	bool is_found = false;

	// If tree is empty, insert node as root.
	if( getKey() == -1.0 )
	{
		setKey( new_key );  // Root's key.
		setHeight( 1 );     // Initial height.

		setData( data );    // Node's data.

		// Return this node as root.
		return this;
	}

	// Otherwise, search for insertion position:

	// Tree's root node.
	AvlTreeNode *root = this;
	// Push original root to tree_traversal stack.
	tree_traversal.push( root );

	// Search for insertion's position:
	while( !is_found )
	{
		if( tree_traversal.top() == nullptr )
			is_found = true;
		// If new node's key is lower than its ascendant's key, search for insertion position
		// among left descendants.
		else if( new_key < tree_traversal.top()->getKey() )
			tree_traversal.push( tree_traversal.top()->getDescendants().first );
		// If new node's key is greater than or equal to its ascendant's key, search for
		// insertion position among right descendants.
		else
			tree_traversal.push( tree_traversal.top()->getDescendants().second );
	}

	// Insert new node, if it wasn't already inserted as root.
	if( is_found )
	{
		// Pop insertion position from the stack.
		tree_traversal.pop();

		// Node to be inserted as leaf:
		AvlTreeNode *leaf = new AvlTreeNode();
		leaf->setKey( new_key );  // Set new node's key.
		leaf->setHeight( 1 );     // Initial height.
		leaf->setData( data );    // Node's data.

		// Link with ascendant as left descendant if leaf's key is lower than ascendant's key.
		if( new_key < tree_traversal.top()->getKey() )
		{
			tree_traversal.top()->setLeftDescendant( leaf );
		}
		// Link with ascendant as right descendant if leaf's key is greater than or equal to
		// ascendant's key.
		else
			tree_traversal.top()->setRightDescendant( leaf );
	}

	// Backtrack through traversed tree maintaining balancing factor of -1, 0 or 1.
	while( !tree_traversal.empty() )
	{
		// Pop root node from the stack.
		root = tree_traversal.top();
		tree_traversal.pop();

		// Update its height and balancing factor.
		root->updateHeight();
		root->updateFactor();

		// If it's unbalanced, rotate node and get new root.
		if( root->getFactor() > 1 || root->getFactor() < -1 )
			root = root->rotate();

		// Correct left and right descendants after possible rotations.
		if( !tree_traversal.empty() &&
				root->getKey() < tree_traversal.top()->getKey() )
		{
			tree_traversal.top()->setLeftDescendant( root );
		}
		else if( !tree_traversal.empty() )
		{
			tree_traversal.top()->setRightDescendant( root );
		}
	}

	// Return tree's root recursively.
	return root;
}

AvlTreeNode* AvlTreeNode::remove( StringHashData *data, SortBy key_order )
{
	// Key of node to be removed.
	double remove_key = ( key_order == SortBy::SCORE ) ?
			data->getScore() : static_cast< double >( data->getFrequency() );

	// If tree is empty:
	if( getKey() == -1.0 )
	{
		// Return empty_node as root.
		return this;
	}

	// Otherwise, search for node with remove_key:

	// If node with remove_key has been found:
	if( getKey() == remove_key && getData() == data )
	{
		// If both descendants exist:
		if( getDescendants().first != nullptr &&
				getDescendants().second != nullptr )
		{
			// Node with greatest key among left descendants.
			AvlTreeNode *greatest = getDescendants().first->findGreatestKey();
			setKey( greatest->getKey() );    // Node's new key. This removes node's old key.
			setData( greatest->getData() );  // Node's new data. This removes node's old data.

			// Remove node with greatest key among left descendants while linking modified nodes.
			setLeftDescendant( getDescendants().first->remove( greatest->getData(), key_order ) );

			// Update node's height and balancing factor.
			updateHeight();
			updateFactor();

			// Tree's root up to this point.
			AvlTreeNode *root = this;
			// If tree is unbalanced, rotate to adjust it.
			if( getFactor() < -1 || getFactor() > 1 )
				root = rotate();

			// Return tree's root.
			return root;
		}

		// If only left descendant exists:
		else if( getDescendants().first != nullptr )
		{
			// Tree's root is left descendant node.
			AvlTreeNode *new_root = getDescendants().first;
			delete this;  // Deallocate memory used for node to be removed.

			// Return tree's new root.
			return new_root;
		}

		// If only right descendant exists:
		else if( getDescendants().second != nullptr )
		{
			// Tree's root is right descendant node.
			AvlTreeNode *new_root = getDescendants().second;
			delete this;  // Deallocate memory used for node to be removed.

			// Return tree's new root.
			return new_root;
		}

		// If no descendants exist:
		else
		{
			delete this;  // Deallocate memory used for node to be removed.

			// Return null pointer (empty node).
			return nullptr;
		}
	}

	// If remove_key is lower than current node, search among left descendants.
	else if( getDescendants().first != nullptr && remove_key < getKey() )
		setLeftDescendant( getDescendants().first->remove( data, key_order ) );
	// If remove_key is greater than current node, search among right descendants.
	else if( getDescendants().second != nullptr && remove_key > getKey() )
		setRightDescendant( getDescendants().second->remove( data, key_order ) );
	// If remove_key is equal to current node, search among existing descendants.
	else if( ( getDescendants().first != nullptr || getDescendants().second != nullptr ) &&
				remove_key == getKey() )
	{
		if( getDescendants().first != nullptr )
			setLeftDescendant( getDescendants().first->remove( data, key_order ) );
		if( getDescendants().second != nullptr )
			setRightDescendant( getDescendants().second->remove( data, key_order ) );
	}
	// Otherwise, there are no descendants to search and remove_key isn't in the tree.
	else
	{
		// Return this node.
		return this;
	}

	// Update this node's height and balancing factor.
	updateHeight();
	updateFactor();

	// Tree's root up to this point.
	AvlTreeNode *root = this;
	// If tree is unbalanced, rotate node to adjust it.
	if( getFactor() < -1 || getFactor() > 1 )
		root = rotate();

	// Return tree's root.
	return root;
}

bool AvlTreeNode::search( StringHashData *data, SortBy key_order )
{
	// If tree is empty, return FALSE (not found).
	if( getKey() < 0.0 )
		return false;

	// Key of node to be found
	double find_key = ( key_order == SortBy::SCORE ) ?
			data->getScore() : static_cast< double >( data->getFrequency() );

	// Pointer to traverse AVL tree.
	AvlTreeNode *current_node = this;
	// Search for node with find_key.
	while( current_node != nullptr )
	{
		// If find_key has been found, return TRUE.
		if( current_node->getKey() == find_key && current_node->getData() == data )
			return true;
		// If find_key is lower than current_node's key, search among left descendants.
		else if( find_key < current_node->getKey() )
			current_node = current_node->getDescendants().first;
		// If find_key is greater than current_node's key, search among right descendants.
		else if( find_key > current_node->getKey() )
			current_node = current_node->getDescendants().second;
		// If find_key is equal to current_node's key, but has different data, search among
		// existing descendants.
		else
		{
			// Search among left descendants.
			if( current_node->getDescendants().first != nullptr &&
					current_node->getDescendants().first->search( data, key_order ) )
				return true;
			// Search among right descendants if data wasn't found in left descendants.
			else if( current_node->getDescendants().second != nullptr &&
					current_node->getDescendants().second->search( data, key_order ) )
				return true;
			// Node wasn't found in the tree.
			else
				return false;
		}
	}

	// Since find_key wasn't found, return FALSE.
	return false;
}

bool AvlTreeNode::setKey( double key )
{
	// If key is valid (0.0 ~ 4.0 ):
	if( key >= 0.0 && key <= 4.0 )
	{
		// Assign key argument to node's key.
		this->key_ = key;

		// Indicate that key was set.
		return true;
	}
	else
		return false;  // Indicate that key is invalid.
}

double AvlTreeNode::getKey()
{
	// Return node's key.
	return this->key_;
}

bool AvlTreeNode::setData( StringHashData *data )
{
	// If data is valid:
	if( data != nullptr )
	{
		// Assign data argument to node's data.
		this->data_ = data;

		// Indicate that data was set.
		return true;
	}
	else
		// Indicate that data is invalid (null pointer).
		return false;
}

StringHashData* AvlTreeNode::getData()
{
	// Return node's data. Can be null pointer if there's no data.
	return this->data_;
}

bool AvlTreeNode::setLeftDescendant( AvlTreeNode *left_node )
{
	// If left descendant isn't set:
	if( getDescendants().first == nullptr )
	{
		// Assign left_node argument to node's left descendant.
		this->left_ = left_node;

		// Indicate that left descendant was set.
		return true;
	}
	else
		// Indicate that left descendant was already set.
		return false;
}

bool AvlTreeNode::setRightDescendant( AvlTreeNode *right_node )
{
	// If right descendant isn't set:
	if( getDescendants().second == nullptr )
	{
		// Assign right_node argument to node's right descendant.
		this->right_ = right_node;

		// Indicate that right descendant was set.
		return true;
	}
	else
		// Indicate that right descendant was already set.
		return false;
}

std::pair< AvlTreeNode*, AvlTreeNode* > AvlTreeNode::getDescendants()
{
	// Return pair of left and right descendants.
	return pair< AvlTreeNode*, AvlTreeNode* >( this->left_, this->right_ );
}

bool AvlTreeNode::setHeight( int height )
{
	// If height is valid (non-negative):
	if( height >= 0 )
	{
		// Assign height argument to node's height.
		this->height_ = height;

		// Indicate that height was set.
		return true;
	}
	else
		// Indicate that height is invalid (negative).
		return false;
}

int AvlTreeNode::getHeight()
{
	// Return node's height.
	return this->height_;
}

void AvlTreeNode::setFactor( int factor )
{
	// Assign factor argument to node's balancing factor.
	this->factor_ = factor;
}

int AvlTreeNode::getFactor()
{
	// Return node's balancing factor.
	return this->factor_;
}

void AvlTreeNode::updateHeight()
{
	// Left descendant node.
	AvlTreeNode *left_node = getDescendants().first;
	// Right descendant node.
	AvlTreeNode *right_node = getDescendants().second;

	// If both descendants exist:
	if( left_node != nullptr && right_node != nullptr )
	{
		// Set ascendant's height as the greatest height between descendants plus one.
		if( left_node->getHeight() >= right_node->getHeight() )
			setHeight( left_node->getHeight() + 1 );
		else
			setHeight( right_node->getHeight() + 1 );
	}
	// If only left descendant exists, set ascendant's height as left's height plus one.
	else if( left_node != nullptr )
		setHeight( left_node->getHeight() + 1 );
	// If only right descendant exists, set ascendant's height as right's height plus one.
	else if( right_node != nullptr )
		setHeight( right_node->getHeight() + 1 );
	// Otherwise, it's a leaf with height one (1).
	else
		setHeight( 1 );
}

void AvlTreeNode::updateFactor()
{
	// Left descendant node.
	AvlTreeNode *left_node = getDescendants().first;
	// Right descendant node.
	AvlTreeNode *right_node = getDescendants().second;

	// If both descendants exist:
	if( left_node != nullptr && right_node != nullptr )
	{
		// Calculate this node's balancing factor (difference between descendants's heights).
		setFactor( left_node->getHeight() - right_node->getHeight() );
	}
	// If only left descendant exists:
	if( left_node != nullptr )
		setFactor( left_node->getHeight() );
	// If only right descendant exists:
	if( right_node != nullptr )
		setFactor( 0 - right_node->getHeight() );
}

AvlTreeNode* AvlTreeNode::findGreatestKey()
{
	// Pointer to search for node with greatest key.
	AvlTreeNode *current_node = ( getDescendants().second != nullptr ) ?
										getDescendants().second : getDescendants().first;

	// Descendant node with the greatest key.
	AvlTreeNode *greatest_node;

	// Search for descendant node with greatest key.
	while( current_node != nullptr )
	{
		// Greatest key so far.
		greatest_node = current_node;
		// Assign either a node with greater key or null pointer to current_node.
		current_node = current_node->getDescendants().second;
	}

	// Return descendant nodes's greatest key.
	return greatest_node;
}

AvlTreeNode* AvlTreeNode::rotate()
{
	// If node lacks a descendant, return unaltered node.
	if( getDescendants().first == nullptr && getDescendants().second == nullptr )
	{
		AvlTreeNode *node = this;
		return node;
	}

	// Root node after rotation.
	AvlTreeNode *root = nullptr;

	// Descendants.
	AvlTreeNode *left_node = getDescendants().first;
	AvlTreeNode *right_node = getDescendants().second;

	// Rotate according to balancing factors of root and descendants.
	// Simple rotation to the left:
	if( right_node != nullptr && getFactor() < 0 && right_node->getFactor() <= 0 )
		root = rotateLeft();
	// Simple rotation to the right:
	else if( left_node != nullptr && getFactor() > 0 && left_node->getFactor() >= 0 )
		root = rotateRight();
	// Double rotation to the left:
	else if( right_node != nullptr && getFactor() < 0 && right_node->getFactor() > 0 )
	{
		setRightDescendant( right_node->rotateRight() );
		root = rotateLeft();
	}
	// Double rotation to the right:
	else if( left_node != nullptr && getFactor() > 0 && left_node->getFactor() < 0 )
	{
		setLeftDescendant( left_node->rotateLeft() );
		root = rotateRight();
	}

	// Update root's height and balancing factor.
	root->updateHeight();
	root->updateFactor();

	// Return tree root.
	return root;
}

AvlTreeNode* AvlTreeNode::rotateLeft()
{
	// Tree's root after rotating.
	AvlTreeNode *new_root = getDescendants().second;

	// Tree's original root.
	AvlTreeNode *old_root = this;

	// Left rotation linking adjustments.
	setRightDescendant( new_root->getDescendants().first );
	new_root->setLeftDescendant( old_root );

	// Updates modified node's height and balancing factor.
	updateHeight();
	updateFactor();

	// Return tree's new root.
	return new_root;
}

AvlTreeNode* AvlTreeNode::rotateRight()
{
	// Tree's root after rotating.
	AvlTreeNode *new_root = getDescendants().first;

	// Tree's original root.
	AvlTreeNode *old_root = this;

	// Right rotation linking adjustments.
	setLeftDescendant( new_root->getDescendants().second );
	new_root->setRightDescendant( old_root );

	// Updates modified node's height and balancing factor.
	updateHeight();
	updateFactor();

	// Return tree's new root.
	return new_root;
}
