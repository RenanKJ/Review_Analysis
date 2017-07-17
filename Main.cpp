/**
 * Analizes film reviews and classifies them according to expressed feelings.
 *
 *   Created on: July 16th, 2017
 *      Authors: Aline Weber
 *               Renan Kummer
 *
 * Final work of "Data Sorting and Searching" discipline.
 *   University: Universidade Federal do Rio Grande do Sul (UFRGS)
 *     Location: Porto Alegre, RS, Brazil
 */

// Defines which main() function to use.
#include "Tests/MainControl.cpp"
#ifdef MAIN_

// Libraries:
#include "Classes/ReviewAnalyzer.h"


int main()
{
	// Instance of analyzer class.
	ReviewAnalyzer analyzer;
	// Begin the program.
	analyzer.run();
}

#endif /* MAIN_ */
