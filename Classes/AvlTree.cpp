/**
 * Class AvlTree member-function definitions.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include <iostream>
#include <iomanip>
#include <stack>
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

void AvlTree::printGreatestKeys( unsigned rank_size )
{
	// Copy of rank_size.
	unsigned size = rank_size;
	// Amount of digits in rank_size.
	unsigned digits_amount = 0;
	// Calculate the amount of digits in rank_size.
	while( size > 0 )
	{
		size /= 10;
		++digits_amount;
	}
	// Set size to 1.
	size = 1;

	// Stack to traverse tree.
	stack< AvlTreeNode* > tree_traversal;
	// Stack of printed words (to avoid printing again when backtracking).
	stack< string > printed_words;
	// Last printed or checked word.
	string last_word;

	// Push tree's root to the stack.
	tree_traversal.push( getRoot() );

	// Print header line.
	( getKeyOrder() == SortBy::SCORE ) ? cout << "--   Score Ranking   --\n" : cout << "-- Frequency Ranking --\n";

	// Search for rank_size words:
	while( !tree_traversal.empty() && size <= rank_size )
	{
		// If current word was already printed, its left descendants were already checked;
		// keep backtracking.
		if( !printed_words.empty() &&
				tree_traversal.top()->getData()->getWord() == printed_words.top() )
		{
			last_word = printed_words.top();

			printed_words.pop();
			tree_traversal.pop();
		}

		// If last word wasn't in the right descendant and there're right descendants,
		// search for greater key among right descendants.
		else if( tree_traversal.top()->getDescendants().second != nullptr &&
				tree_traversal.top()->getDescendants().second->getData()->getWord() != last_word )
		{
			tree_traversal.push( tree_traversal.top()->getDescendants().second );
		}

		// There're no more right descendants:
		else
		{
			// This is one of the words with the greatest keys.
			cout << setw( digits_amount ) << setfill( '0' ) << size
					<< ". " << tree_traversal.top()->getData()->getWord() << '\n';
			++size;  // Increment amount of found words.
			last_word = tree_traversal.top()->getData()->getWord();

			// If last word wasn't in the left descendant and there're left descendants,
			// search for next greater key among left descendants.
			if( tree_traversal.top()->getDescendants().first != nullptr &&
					tree_traversal.top()->getDescendants().first->getData()->getWord() != last_word )
			{
				printed_words.push( tree_traversal.top()->getData()->getWord() );
				tree_traversal.push( tree_traversal.top()->getDescendants().first );
			}
			// Left and right descendants have been checked or do not exist, keep backtracking.
			else
				tree_traversal.pop();
		}
	}
}

void AvlTree::printLowestKeys( unsigned rank_size )
{
	// Copy of rank_size.
	unsigned size = rank_size;
	// Amount of digits in rank_size.
	unsigned digits_amount = 0;
	// Calculate the amount of digits in rank_size.
	while( size > 0 )
	{
		size /= 10;
		++digits_amount;
	}
	// Set size to 1.
	size = 1;

	// Stack to traverse tree.
	stack< AvlTreeNode* > tree_traversal;
	// Stack of printed words (to avoid printing again when backtracking).
	stack< string > printed_words;
	// Last printed or checked word.
	string last_word;

	// Push tree's root to the stack.
	tree_traversal.push( getRoot() );

	// Print header line.
	( getKeyOrder() == SortBy::SCORE ) ? cout << "--   Score Ranking   --\n" : cout << "-- Frequency Ranking --\n";

	// Search for rank_size words:
	while( !tree_traversal.empty() && size <= rank_size )
	{
		// If current word was already printed, its right descendants were already checked;
		// keep backtracking.
		if( !printed_words.empty() &&
				tree_traversal.top()->getData()->getWord() == printed_words.top() )
		{
			last_word = printed_words.top();

			printed_words.pop();
			tree_traversal.pop();
		}

		// If last word wasn't in the left descendant and there're left descendants,
		// search for lower key among left descendants.
		else if( tree_traversal.top()->getDescendants().first != nullptr &&
				tree_traversal.top()->getDescendants().first->getData()->getWord() != last_word )
		{
			tree_traversal.push( tree_traversal.top()->getDescendants().first );
		}

		// There're no more left descendants:
		else
		{
			// This is one of the words with the lowest keys.
			cout << setw( digits_amount ) << setfill( '0' ) << size
					<< ". " << tree_traversal.top()->getData()->getWord() << '\n';
			++size;  // Increment amount of found words.
			last_word = tree_traversal.top()->getData()->getWord();

			// If last word wasn't in the right descendant and there're right descendants,
			// search for next lower key among right descendants.
			if( tree_traversal.top()->getDescendants().second != nullptr &&
					tree_traversal.top()->getDescendants().second->getData()->getWord() != last_word )
			{
				printed_words.push( tree_traversal.top()->getData()->getWord() );
				tree_traversal.push( tree_traversal.top()->getDescendants().second );
			}
			// Left and right descendants have been checked or do not exist, keep backtracking.
			else
				tree_traversal.pop();
		}
	}
}

void AvlTree::setRoot( AvlTreeNode *new_root )
{
	// Assign new_root as tree's root.
		this->root_ = new_root;
}
