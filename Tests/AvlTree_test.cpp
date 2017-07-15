/**
 * Main function to test AvlTree class.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Defines which main() function to use.
#include "MainControl.cpp"
#ifdef AVLTREE_TEST_

// Libraries:
#include <iostream>
#include <forward_list>
#include <string>
#include <stack>
#include "../Classes/AvlTree.h"

using namespace std;


// Print AVL tree using left-right-center traversal.
void printAvlTree( AvlTreeNode *node )
{
	if( node->getDescendants().first != nullptr )
		printAvlTree( node->getDescendants().first );

	if( node->getDescendants().second != nullptr )
		printAvlTree( node->getDescendants().second );

	cout << node->getKey() << " ";
}


int main()
{
	cout << "> AvlTree : main()" << endl;

	// Data to be inserted.
	StringHashData d0( "d0", 3.52 );
	StringHashData d1( "d1", 3.24 );
	StringHashData d2( "d2", 3.52 );
	StringHashData d3( "d3", 3.24 );
	StringHashData d4( "d4", 3.521 );

	// Increase d1 and d3 frequencies.
	//d1.recalculateScore( 3.00 );
	//d3.recalculateScore( 3.00 );

	// Test constructor.
	AvlTree tree( SCORE );

	// insert() method.
	tree.insert( &d0 );
	tree.insert( &d1 );
	tree.insert( &d2 );
	tree.insert( &d3 );
	tree.insert( &d4 );

	// Print keys using left-right-center traversal.
	cout << "tree.insert(...):\n";
	printAvlTree( tree.getRoot() );

	// search() method.
	cout << "\n\ntree.search(...):\n"
			<< boolalpha
			<< "d0 = " << tree.search( &d0 )
			<< "\nd1 = " << tree.search( &d1 )
			<< "\nd2 = " << tree.search( &d2 )
			<< "\nd3 = " << tree.search( &d3 )
			<< "\nd4 = " << tree.search( &d4 )
			<< endl;

	// printGreatestKeys() method.
	cout << "\n-- Greatest keys:\n";
	tree.printGreatestKeys( 5 );

	// printLowestKeys() method.
	cout << "\n-- Lowest keys:\n";
	tree.printLowestKeys( 5 );

	// remove() method.
	tree.remove( &d4 );
	tree.remove( &d1 );
	tree.remove( &d0 );
	tree.remove( &d2 );
	tree.remove( &d3 );

	cout << "\n\ntree.remove(...):\n";

	// Check if keys were removed.
	if( tree.isEmpty() )
		cout << "All data removed." << endl;
	else
	{
		cout << "\n\ntree.search(...):\n"
				<< "d0 = " << tree.search( &d0 )
				<< "\nd1 = " << tree.search( &d1 )
				<< "\nd2 = " << tree.search( &d2 )
				<< "\nd3 = " << tree.search( &d3 )
				<< "\nd4 = " << tree.search( &d4 )
				<< endl;
	}


	cout << "> AvlTree : end of main()" << endl;
}

#endif /* AVLTREE_TEST_ */
