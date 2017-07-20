/**
 * Main function to test Trie class.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Defines which main() function to use.
#include "MainControl.cpp"
#ifdef TRIE_TEST_

// Libraries:
#include <iostream>
#include <stack>
#include "../Classes/Trie.h"

using namespace std;

int main()
{
	cout << "> Trie : main()" << endl;

	// Test constructor.
	Trie* tree = new Trie;

	// insert() method.
	tree->insert( "be" );
	tree->insert( "bear" );
	tree->insert( "bell" );
	tree->insert( "build" );
	tree->insert( "sell" );
	tree->insert( "stock" );
	tree->insert( "stop" );

	// prefixSearch() method.
	cout << "search all:\n";
	tree->search( "" );
	cout << endl;

	cout << "search be:\n";
	tree->search( "bea" );
	cout << endl;

	cout << "search s:\n";
	tree->search( "s" );
	cout << endl;
}

#endif /* TRIE_TEST_ */
