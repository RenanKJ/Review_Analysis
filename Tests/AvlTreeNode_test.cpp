/**
 * Main function to test AvlTreeNode class.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Defines which main() function to use.
#include "MainControl.cpp"
#ifdef AVLTREENODE_TEST_

// Libraries:
#include <iostream>
#include <stack>
#include "../Classes/AvlTreeNode.h"
#include "../Classes/StringHashData.h"

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
	cout << "> AvlTreeNode : main()" << endl;

	// Data to be inserted.
	StringHashData d0( "d0", 3 );
	StringHashData d1( "d1", 4 );
	StringHashData d2( "d2", 0 );
	StringHashData d3( "d3", 1 );
	StringHashData d4( "d4", 2 );

	// Test constructor.
	AvlTreeNode *root = new AvlTreeNode;

	// insert() method.
	root = root->insert( &d0, SCORE );
	root = root->insert( &d1, SCORE );
	root = root->insert( &d2, SCORE );
	root = root->insert( &d3, SCORE );
	root = root->insert( &d4, SCORE );

	// Print keys using left-right-center traversal.
	cout << "root->insert(...):\n";
	//printAvlTree( root );

	// search() method.
	cout << "\n\nroot->search(...):\n"
			<< boolalpha
			<< "d0 = " << root->search( &d0, SCORE )
			<< "\nd1 = " << root->search( &d1, SCORE )
			<< "\nd2 = " << root->search( &d2, SCORE )
			<< "\nd3 = " << root->search( &d3, SCORE )
			<< "\nd4 = " << root->search( &d4, SCORE )
			<< endl;

	/*// remove() method.
	root = root->remove( &d4, SCORE );
	root = root->remove( &d1, SCORE );
	root = root->remove( &d0, SCORE );
	root = root->remove( &d2, SCORE );
	root = root->remove( &d3, SCORE );

	cout << "\n\nroot->remove(...):\n";

	// Check if keys were removed.
	if( root == nullptr )
		cout << "All data removed." << endl;
	else
		cout << "d0 = " << root->search( &d0, SCORE )
				<< "\nd1 = " << root->search( &d1, SCORE )
				<< "\nd2 = " << root->search( &d2, SCORE )
				<< "\nd3 = " << root->search( &d3, SCORE )
				<< "\nd4 = " << root->search( &d4, SCORE )
				<< endl;*/

	cout << "> AvlTreeNode : end of main()" << endl;
}

#endif /* AVLTREENODE_TEST_ */
