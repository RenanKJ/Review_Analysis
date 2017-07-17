/**
 * Class ReviewAnalyzer member-function definitions.
 *
 *  Created on: July 16th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include <locale>
#include <stack>
#include "ReviewAnalyzer.h"

using namespace std;


ReviewAnalyzer::ReviewAnalyzer( unsigned hash_size )
	: database_( hash_size ),   // Initialize hash table with given size.
	  score_ranking_( SortBy::SCORE ),        // Sort AVL Tree by score.
	  frequency_ranking_( SortBy::FREQUENCY ) // Sort AVL Tree by frequency.
{
	// Empty body.
}

int ReviewAnalyzer::run()
{
	// Print header line.
	cout << "-- REVIEWS ANALYSIS --\n"
			<< "Aline Weber & Renan Kummer\n"
			<< endl;

	// Explain commands for the user.
	printCommands();

	string command; // User's command.
	while( command != "exit" )
	{
		// Print symbol to prompt user.
		cout << "\n> ";
		// Read command.
		command = readCommand();
		// Change string to lowercase.
		changeToLowercase( command );

		// If command is "analyze":
		if( command == "analyze" )
		{
			// Read argument (review text).
			string review = readArgument();

			// Call "analyze" command.
			analyze( review );
		}

		// If command is "help":
		else if( command == "help" )
		{
			// Call "help" command.
			printCommands();
		}

		// If command is "read":
		else if( command == "read" )
		{
			// Read argument (file name).
			string file_name = readArgument();

			// Call "read" command.
			if( !readFile( file_name ) )
				cout << "Couldn't open file. Wrong file path or format." << endl;
		}

		// If command is "print+":
		else if( command == "print+" )
		{
			// Read argument (ranking size).
			unsigned rank_size = stoi( readArgument() );			// TODO: exception handling for wrong arguments.

			if( !this->score_ranking_.isEmpty() )
			{
				// Call "print+" command.
				printMostPositive( rank_size );
			}
			else
				cout << "Database is empty." << endl;
		}

		// If command is "print-":
		else if( command == "print-" )
		{
			// Read argument (ranking size).
			unsigned rank_size = stoi( readArgument() );			// TODO: exception handling for wrong arguments.

			if( !this->score_ranking_.isEmpty() )
			{
				// Call "print-" command.
				printMostNegative( rank_size );
			}
			else
				cout << "Database is empty." << endl;
		}

		// If command is "printf":
		else if( command == "printf" )
		{
			// Read argument (ranking size).
			unsigned rank_size = stoi( readArgument() );			// TODO: exception handling for wrong arguments.

			if( !this->frequency_ranking_.isEmpty() )
			{
				// Call "printf" command.
				printMostFrequent( rank_size );
			}
			else
				cout << "Database is empty." << endl;
		}

		// If command is "exit" or "quit":
		else if( command == "exit" || command == "quit" )
		{
			// Print message.
			cout << "\nProgram ended successfully." << endl;
		}

		// If command is invalid, print message.
		else if( command.empty() )
			cout << "Invalid command. Enter \"help\" to list commands." << endl;
	}

	// Program ended successfully.
	return 0;
}

std::string ReviewAnalyzer::readCommand()
{
	// Command from the user.
	string command;

	// Read command.
	cin >> command;

	// Change command to lowercase.
	changeToLowercase( command );

	// Validate command.
	if( command == "analyze" || command == "help" || command == "read" ||
			command == "print+" || command == "print-" || command == "printf" ||
			command == "exit" || command == "quit" )
	{
		// Command is valid.
		return command;
	}

	// Command is invalid, return empty string.
	else
	{
		return "";
	}
}

std::string ReviewAnalyzer::readArgument()		// TODO: exception handling for invalid arguments.
{
	// Ignore characters in buffer (e.g. whitespace).
	cin.ignore();

	// Argument from the user.
	string argument;

	// Read argument.
	getline( std::cin, argument );

	// Return command's argument.
	return argument;
}

void ReviewAnalyzer::printCommands()
{
	// Print header line.
	cout << "-- Commands list --\n";

	// Print command and description.
	cout << "analyze <text>   :  analyzes the overall emotion of <text>\n"
			<< "read <filename>  :  read file and store words in database\n"
			<< "print+ <amount>  :  print <amount> most positive words in database\n"
			<< "print- <amount>  :  print <amount> most negative words in database\n"
			<< "printf <amount>  :  print <amount> most frequent words in database\n"
			<< "help             :  print list of commands\n"
			<< "exit             :  ends program\n"
			<< "--  End of list  --"
			<< endl;
}

bool ReviewAnalyzer::readFile( std::string file_name )		// TODO: exception handling for wrong format.
{
	// File to be read.
	ifstream file;

	// Attempt to open file.
	file.open( file_name );
	// Validate file path.
	if( !file.is_open() )
		return false;

	// Review's overall feeling.
	double feeling;
	// Review's text.
	string review;

	// DEBUG: counter.
	unsigned counter = 1;

	// Read entire file, line by line.
	while( !file.eof() )
	{
		// Read feeling's number.
		file >> feeling;
		// Read review.
		getline( file, review );

		// Words from review.
		string word;
		// Key for word in database.
		unsigned key;
		// Stream to get each word.
		istringstream line( review );

		// DEBUG: print line.
		cout << "Line " << counter++ << endl;

		// Insert each word in database and update scores accordingly.
		while( !line.eof() )
		{
			// Get word.
			line >> word;

			// Filter punctuation characters and concatenated words.
			if( !word.empty() && filterWord( word ) )
			{
				// Insert word and get its key.
				insert( word, feeling, &key );
				cout << " Word " << word << " Key " << key << " Rate " << this->database_.getUsedRate() << endl;
				// Update word's score.
				//updateScore( getSatelliteData( key ), feeling );
			}
		}
	}

	// File was successfully read.
	return true;
}

bool ReviewAnalyzer::analyze( std::string review )
{
	// Stream to get each word from the text.
	istringstream text( review );

	string word; 	 // Words from the text.
	unsigned key;   // Key to word in database.
	unsigned words_amount = 0;  // Amount of words for the score.
	double score = 0;				 // Score of review.
	bool has_word = false;		 // Indicate a word has been found in database.

	// Read the entire text word by word.
	while( !text.eof() )
	{
		// Get a word.
		text >> word;

		// If it passes by the filter and it is in the database:
		if( filterWord( word ) && search( word, &key ) )			// TODO: filterWord() may return two words in next version.
		{
			// Indicate so.
			has_word = true;

			// Increment amount of words.
			++words_amount;
			// Add word's score to total so far.
			score += getSatelliteData( key )->getScore();
		}
	}

	// If at least one word has been found in database:
	if( has_word )
	{
		// Calculate review's score.
		score = score / words_amount;

		// Print results.
		cout << fixed << setprecision( 2 )
				<< "--     Score    --\n";

		if( score < 0.95 )
			cout << "You better avoid it!\n" << score << " - Awful";
		else if( score < 1.95 )
			cout << "Maybe when you've got nothing better to watch?\n" << score << " - Bad";
		else if( score < 2.95 )
			cout << "Nothing remarkable, but not bad either.\n" << score << " - OK";
		else if( score < 3.95 )
			cout << "Recommended! Nice film!\n" << score << " - Good";
		else
			cout << "This deserves an Oscar!\n" << score << " - Excellent";

		cout << "\n-- End of score --" << endl;
	}
	// Otherwise, print message.
	else
	{
		cout << "--    Result     --\n"
				<< "Oops, our database lacks enough information to calculate your review's score.\n"
				<< "-- End of result --" << endl;
	}

	return has_word;
}

bool ReviewAnalyzer::insert( std::string word, double phrase_score, unsigned *key )
{
	// Insert word in database.
	return this->database_.insert( word, phrase_score, key );
}

bool ReviewAnalyzer::search( std::string word, unsigned *key )
{
	// Search for word in database.
	return this->database_.search( word, key );
}

void ReviewAnalyzer::remove( std::string word )
{
	// Remove word from database.
	this->database_.remove( word );
}

void ReviewAnalyzer::printMostPositive( unsigned rank_size )
{
	// Print rank_size most positive words.
	this->score_ranking_.printGreatestKeys( rank_size );
	cout << "--  End of ranking   --" << endl;
}

void ReviewAnalyzer::printMostNegative( unsigned rank_size )
{
	// Print rank_size most positive words.
	this->score_ranking_.printLowestKeys( rank_size );
	cout << "--  End of ranking   --" << endl;
}

void ReviewAnalyzer::printMostFrequent( unsigned rank_size )
{
	// Print rank_size most frequent words.
	this->frequency_ranking_.printGreatestKeys( rank_size );
	cout << "--  End of ranking   --" << endl;
}

StringHashData* ReviewAnalyzer::getSatelliteData( unsigned key )
{
	// Return data at position key.
	return this->database_.getData( key );
}

bool ReviewAnalyzer::filterWord( std::string &word )
{
	// If word is a punctuation mark, it can't be inserted.
	if( word.at( 0 ) == '.' || word.at( 0 ) == ',' || word.at( 0 ) == ';' ||
			word.at( 0 ) == ':' || word.at( 0 ) == '\"' || word.at( 0 ) == '?' ||
			word.at( 0 ) == '!' || word.at( 0 ) == '-' || word.at( 0 ) == '\'' ||
			word.at( 0 ) == '\t' || word.at( 0 ) == '\n' || word.at( 0 ) == ' ' )	// TODO: any more punctuation marks?
	{
		return false;
	}

	// Otherwise, it can be inserted.
	return true;
}

void ReviewAnalyzer::updateScore( StringHashData *satellite_data, double new_score )
{
	// If new_score is negative (standard argument), satellite_data isn't in the tree
	// and must be inserted.
	if( new_score < 0.0 )
		this->score_ranking_.insert( satellite_data );

	// If satellite_data is in the tree, remove it and reinsert it.
	else if( this->score_ranking_.search( satellite_data ) )
	{
		this->score_ranking_.remove( satellite_data );
		satellite_data->recalculateScore( new_score );
		this->score_ranking_.insert( satellite_data );
	}

	// Otherwise, client entered a valid score although satellite_data isn't in the tree,
	// so insert it.
	else
		this->score_ranking_.insert( satellite_data );
}

bool ReviewAnalyzer::rankFrequency()
{
	// Insert each word in frequency tree.
	AvlTreeNode *root = this->score_ranking_.getRoot();

	// Stack to traverse tree.
	stack< AvlTreeNode* > traverse;

	// Push root to stack.
	traverse.push( root );

	// Last node checked.
	AvlTreeNode *last = root;

	// Traverse tree using left-right-center route:
	while( !traverse.empty() )
	{
		// Node's left descendant.
		AvlTreeNode *left = traverse.top()->getDescendants().first;
		// Node's right descendant.
		AvlTreeNode *right = traverse.top()->getDescendants().second;

		// If there's a left descendant that wasn't visited,
		// push it to the stack.
		if( left != nullptr && left != last && right != last )
			traverse.push( left );

		// If there's a right descendant that wasn't visited,
		// push it to the stack.
		else if( right != nullptr && right != last )
			traverse.push( right );

		// Otherwise, insert node in frequency tree and pop it from the stack.
		else
		{
			// Insert in frequency ranking tree.
			this->frequency_ranking_.insert( traverse.top()->getData() );
			last = traverse.top();  // Indicate node has been check last.
			traverse.pop();			// Pop node from the stack.
		}
	}

	// All data was inserted in frequency ranking tree.
	return true;
}

void ReviewAnalyzer::changeToLowercase( std::string &word )
{
	// Global locale.
	locale global;

	// Turn each letter in word into lowercase.
	for( auto i = word.begin(); i != word.end(); ++i )
		tolower( *i, global );
}
