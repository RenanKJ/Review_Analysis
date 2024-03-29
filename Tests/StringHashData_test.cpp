/**
 * Main function to test StringHashData class.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Defines which main() function to use.
#include "MainControl.cpp"
#ifdef STRINGHASHDATA_TEST_

// Libraries:
#include <iostream>
#include <iomanip>
#include "../Classes/StringHashData.h"

using namespace std;


int main()
{
	// Print header line.
	cout << "> StringHashData : main()" << endl;

	// Score's string.
	string score_range;

	// Test constructors.
	StringHashData D0;
	StringHashData D1( "test", 2 );

	// Print attributes.
	cout << "D0:\n"
			<< "Data      = " << D0.getWord()
			<< fixed << setprecision( 2 ) << boolalpha
			<< "\nScore     = " << D0.getScore( true, &score_range ) << " : " << score_range
			<< "\nFrequency = " << D0.getFrequency()
			<< endl;

	cout << "\nD1:\n"
			<< "Data      = " << D1.getWord()
			<< "\nScore     = " << D1.getScore( true, &score_range ) << " : " << score_range
			<< "\nFrequency = " << D1.getFrequency()
			<< endl;

	// insert() method.
	D0.insert( "semester", 0.0 );

	// Print attributes.
	cout << "\nD0.insert( \"semester\", 0.0 ):\n"
			<< "Data      = " << D0.getWord()
			<< "\nScore     = " << D0.getScore( true, &score_range ) << " : " << score_range
			<< "\nFrequency = " << D0.getFrequency()
			<< endl;

	// remove() method.
	D0.remove();

	// Print attributes.
	cout << "\nD0.remove():\n"
			<< "Data      = " << D0.getWord()
			<< "\nScore     = " << D0.getScore( true, &score_range ) << " : " << score_range
			<< "\nFrequency = " << D0.getFrequency()
			<< endl;

	// recalculate() method.
	D0.insert( "A", 4.0 );
	D0.recalculateScore( 3.0 );
	D0.recalculateScore( 4.0 );

	// Print attributes.
	cout << "\nD0.recalculateScore(...):\n"
			<< "Data      = " << D0.getWord()
			<< "\nScore     = " << D0.getScore( true, &score_range ) << " : " << score_range
			<< "\nFrequency = " << D0.getFrequency()
			<< endl;

	cout << "> StringHashData : end of main()" << endl;
}

#endif /* STRINGHASHDATA_TEST_*/
