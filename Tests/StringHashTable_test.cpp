/**
 * Main function to test StringHashTable class.
 *
 *  Created on: July 15th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Defines which main() function to use.
#include "MainControl.cpp"
#ifdef STRINGHASHTABLE_TEST_

// Libraries:
#include <iostream>
#include <iomanip>
#include <utility>
#include "../Classes/StringHashTable.h"

using namespace std;


int main()
{
	cout << "> StringHashTable : main()" << endl;

	// Testing constructor.
	StringHashTable h( 3 );

	// Testing insert() and rehash() methods.
	cout << "h.insert(...): [4 elements]\n"
			<< boolalpha
			<< h.insert( "semester", 0 ) << '\n'
			<< h.insert( "gaming", 4 ) << '\n'
			<< h.insert( "budweiser", 4 ) << '\n'
			<< h.insert( "studying", 2 )
			<< endl;

	// Testing getSize(), getUsedRate() and getCollisionsNum() methods.
	cout << "\nh.getSize()          = " << h.getSize()
			<< fixed << setprecision( 2 )
			<< "\nh.getUsedRate()      = " << h.getUsedRate()
			<< "\nh.getCollisionsNum() = " << h.getCollisionsNum()
			<< endl;

	// Testing search() method.
	unsigned k0, k1, k2, k3;
	cout << "\nh.search(...):\n"
			<< "semester  = " << h.search( "semester", &k0 ) << '\n'
			<< "gaming    = " << h.search( "gaming", &k1 ) << '\n'
			<< "budweiser = " << h.search( "budweiser", &k2 ) << '\n'
			<< "studying  = " << h.search( "studying", &k3 ) << '\n'
			<< "missing   = " << h.search( "missing" )
			<< endl;

	// Testing remove() method.
	h.remove( h.getData( k0 )->getWord() );
	h.remove( h.getData( k1 )->getWord() );
	h.remove( h.getData( k2 )->getWord() );
	h.remove( h.getData( k3 )->getWord() );
	h.remove( "missing" );

	cout << "\nh.remove(...):\n"
			<< "semester  = " << h.search( "semester", &k0 ) << '\n'
			<< "gaming    = " << h.search( "gaming", &k1 ) << '\n'
			<< "budweiser = " << h.search( "budweiser", &k2 ) << '\n'
			<< "studying  = " << h.search( "studying", &k3 )
			<< endl;

	cout << "> StringHashTable : end of main()" << endl;
}

#endif /* STRINGHASHTABLE_TEST_*/
