/**
 * Class AvlTreeNode member-function definitions.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include <stack>
#include <utility>
#include "../Classes/AvlTreeNode.h"


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

AvlTreeNode* AvlTreeNode::insert( StringHashData *data, SortBy key_order )
{
	// New node's key.
	double new_key = ( key_order == SortBy::SCORE ) ?
			data->getScore() : static_cast< double >( data->getFrequency() );

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

	// If new node's key is lower than its ascendant's key:
	else if( new_key < getKey() )
	{
		// If its ascendant has left descendants, search among them.
		if( getLeftDescendant() != nullptr )
			setLeftDescendant( getLeftDescendant()->insert( data, key_order ) );
		// Otherwise, insert it as left descendant.
		else
		{
			// Node to be inserted as leaf:
			AvlTreeNode *leaf = new AvlTreeNode;
			leaf->setKey( new_key );  // Set new node's key.
			leaf->setHeight( 1 );     // Initial height.
			leaf->setData( data );    // Node's data.

			// Set leaf as left descendant.
			setLeftDescendant( leaf );
		}
	}
	// If new node's key is greater than or equal to its ascendant's key:
	else
	{
		// If its ascendant has right descendants, search among them.
		if( getRightDescendant() != nullptr )
			setRightDescendant( getRightDescendant()->insert( data, key_order ) );
		// Otherwise, insert it as right descendant.
		else
		{
			// Node to be inserted as leaf:
			AvlTreeNode *leaf = new AvlTreeNode;
			leaf->setKey( new_key );  // Set new node's key.
			leaf->setHeight( 1 );     // Initial height.
			leaf->setData( data );    // Node's data.

			// Set leaf as right descendant.
			setRightDescendant( leaf );
		}
	}

	// Backtrack through traversed tree maintaining balancing factor of -1, 0 or 1.

	// Update node's height and balancing factor.
	updateHeight();
	updateFactor();

	AvlTreeNode *root = this;
	// If node's unbalanced, rotate it.
	if( getFactor() > 1 || getFactor() < -1 )
		root = rotate();

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
		if( getLeftDescendant() != nullptr &&
				getRightDescendant() != nullptr )
		{
			// Node with greatest key among left descendants.
			AvlTreeNode *greatest = getLeftDescendant()->findGreatestKey();
			setKey( greatest->getKey() );    // Node's new key. This removes node's old key.
			setData( greatest->getData() );  // Node's new data. This removes node's old data.

			// Remove node with greatest key among left descendants while linking modified nodes.
			setLeftDescendant( getLeftDescendant()->remove( greatest->getData(), key_order ) );

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
		else if( getLeftDescendant() != nullptr )
		{
			// Tree's root is left descendant node.
			AvlTreeNode *new_root = getLeftDescendant();

			// Deallocate memory used for node to be removed.
			delete this;

			// Return tree's new root.
			return new_root;
		}

		// If only right descendant exists:
		else if( getRightDescendant() != nullptr )
		{
			// Tree's root is right descendant node.
			AvlTreeNode *new_root = getRightDescendant();

			// Deallocate memory used for node to be removed.
			delete this;

			// Return tree's new root.
			return new_root;
		}

		// If no descendants exist:
		else
		{
			// Deallocate memory used for node to be removed.
			delete this;

			// Return null pointer (empty node).
			return nullptr;
		}
	}

	// If remove_key is lower than current node, search among left descendants.
	else if( getLeftDescendant() != nullptr && remove_key < getKey() )
		setLeftDescendant( getLeftDescendant()->remove( data, key_order ) );
	// If remove_key is greater than current node, search among right descendants.
	else if( getRightDescendant() != nullptr && remove_key > getKey() )
		setRightDescendant( getRightDescendant()->remove( data, key_order ) );
	// If remove_key is equal to current node, search among existing descendants.
	else if( ( getLeftDescendant() != nullptr || getRightDescendant() != nullptr ) &&
				remove_key == getKey() )
	{
		if( getLeftDescendant() != nullptr )
			setLeftDescendant( getLeftDescendant()->remove( data, key_order ) );
		if( getRightDescendant() != nullptr )
			setRightDescendant( getRightDescendant()->remove( data, key_order ) );
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
			current_node = current_node->getLeftDescendant();
		// If find_key is greater than current_node's key, search among right descendants.
		else if( find_key > current_node->getKey() )
			current_node = current_node->getRightDescendant();
		// If find_key is equal to current_node's key, but has different data, search among
		// existing descendants.
		else
		{
			// Search among left descendants.
			if( current_node->getLeftDescendant() != nullptr &&
					current_node->getLeftDescendant()->search( data, key_order ) )
				return true;
			// Search among right descendants if data wasn't found in left descendants.
			else if( current_node->getRightDescendant() != nullptr &&
					current_node->getRightDescendant()->search( data, key_order ) )
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
	// If key is valid (non-negative):
	if( key >= 0.0 )
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

void AvlTreeNode::setLeftDescendant( AvlTreeNode *left_node )
{

	// Assign left_node argument to node's left descendant.
	this->left_ = left_node;
}

void AvlTreeNode::setRightDescendant( AvlTreeNode *right_node )
{
	// Assign right_node argument to node's right descendant.
	this->right_ = right_node;

}

AvlTreeNode* AvlTreeNode::getLeftDescendant()
{
	// Return node's left descendant.
	return this->left_;
}

AvlTreeNode* AvlTreeNode::getRightDescendant()
{
	// Return node's right descendant.
	return this->right_;
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
	AvlTreeNode *left_node = getLeftDescendant();
	// Right descendant node.
	AvlTreeNode *right_node = getRightDescendant();

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
	AvlTreeNode *left_node = getLeftDescendant();
	// Right descendant node.
	AvlTreeNode *right_node = getRightDescendant();

	// If both descendants exist:
	if( left_node != nullptr && right_node != nullptr )
	{
		// Calculate this node's balancing factor (difference between descendants's heights).
		setFactor( left_node->getHeight() - right_node->getHeight() );
	}
	// If only left descendant exists:
	else if( left_node != nullptr )
		setFactor( left_node->getHeight() );
	// If only right descendant exists:
	else if( right_node != nullptr )
		setFactor( 0 - right_node->getHeight() );
}

AvlTreeNode* AvlTreeNode::findGreatestKey()
{
	// Pointer to search for node with greatest key.
	AvlTreeNode *greatest_node = this;

	// Indicate if greatest node has been found.
	bool is_found = false;

	// Search for descendant node with greatest key.
	while( !is_found )
	{
		if( greatest_node->getRightDescendant() != nullptr )
			greatest_node = greatest_node->getRightDescendant();
		else
			is_found = true;
	}

	// Return descendant nodes's greatest key.
	return greatest_node;
}

AvlTreeNode* AvlTreeNode::rotate()
{
	// If node lacks a descendant, return unaltered node.
	if( getLeftDescendant() == nullptr && getRightDescendant() == nullptr )
	{
		AvlTreeNode *node = this;
		return node;
	}

	// Root node after rotation.
	AvlTreeNode *root = nullptr;

	// Descendants.
	AvlTreeNode *left_node = getLeftDescendant();
	AvlTreeNode *right_node = getRightDescendant();

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
	AvlTreeNode *new_root = getRightDescendant();

	// Tree's original root.
	AvlTreeNode *old_root = this;

	// Left rotation linking adjustments.
	setRightDescendant( new_root->getLeftDescendant() );
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
	AvlTreeNode *new_root = getLeftDescendant();

	// Tree's original root.
	AvlTreeNode *old_root = this;

	// Right rotation linking adjustments.
	setLeftDescendant( new_root->getRightDescendant() );
	new_root->setRightDescendant( old_root );

	// Updates modified node's height and balancing factor.
	updateHeight();
	updateFactor();

	// Return tree's new root.
	return new_root;
}
