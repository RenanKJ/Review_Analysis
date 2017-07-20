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
#include "Trie.h"

using namespace std;


Trie::Trie()
{
	// Set root pointer as null.
	this->root_ = new TrieNode;
}

void Trie::insert( string word )
{
	// If Trie is empty, allocate memory for root node.
	if( isEmpty() )
		this->root_ = new TrieNode;

	// Insert node with data
	this->root_->insert( word );
}

bool Trie::search( string prefix )
{
	TrieNode* starting_node;
	// If Trie isn't empty
	if( !isEmpty() ){
		// Search the node where the prefix ends
		if(prefix == ""){
			starting_node->printAll(prefix, this->root_);
			return true;
		}
		else{
			starting_node = this->root_->nodeSearch(prefix, this->root_);
			if(starting_node != nullptr){
				// Starting printing all the words above that node
				starting_node->printAll(prefix, starting_node);
				return true;
			}
		}
	}

	// Otherwise, return false to indicate tree is empty.
	return false;
}

bool Trie::isEmpty()
{
	// If root is a null pointer, the tree is empty.
	return ( getRoot() == nullptr );
}

TrieNode* Trie::getRoot()
{
	// Return root node.
	return this->root_;
}

void Trie::setRoot( TrieNode *new_root )
{
	// Assign new_root as tree's root.
		this->root_ = new_root;
}
