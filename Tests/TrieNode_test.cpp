/**
 * Main function to test Trie class.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Defines which main() function to use.
#include "MainControl.cpp"
#ifdef TRIENODE_TEST_

// Libraries:
#include <iostream>
#include <stack>
#include "../Classes/TrieNode.h"

using namespace std;

int main()
{
	cout << "> TrieNode : main()" << endl;

	// Test constructor.
	TrieNode* root = new TrieNode;

	// insert() method.
	root->insert( "be" );
	root->insert( "bear" );
	root->insert( "bell" );
	root->insert( "build" );
	root->insert( "sell" );
	root->insert( "stock" );
	root->insert( "stop" );

	// prefixSearch() method.
	cout << "\n\nroot->prefixSearch(...):\n";
	root->printAll( "", root );
	cout << endl;

}

#endif /* TRIENODE_TEST_ */
