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
#include <cmath>
#include "ReviewAnalyzer.h"

using namespace std;


ReviewAnalyzer::ReviewAnalyzer( unsigned hash_size )
	: database_( hash_size ),   // Initialize hash table with given size.
	  score_ranking_( SortBy::SCORE ),         // Sort AVL Tree by score.
	  frequency_ranking_( SortBy::FREQUENCY ), // Sort AVL Tree by frequency.
     stopwords_( 1000 )        // Initialize stopwords' hash table.
{
	// Read stopwords.
	readStopwords( "stopwords.txt" );
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
	while( command != "exit" && command != "quit" )
	{
		// Print symbol to prompt user.
		cout << "\n > ";
		// Read command.
		command = readCommand();
		// Change string to lowercase.
		changeToLowercase( command );

		// If command is "analyze":
		if( command == "analyze" )
		{
			// Read argument (review text).
			string review = readArgument();

			// If no argument was provided, print message.
			if( review.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;
			else
			{
				// Call "analyze" command.
				analyze( review );
			}
		}

		// If command is "help":
		else if( command == "help" )
		{
			// Call "help" command.
			printCommands();
			// Ignore "any" input (up to 1 million characters) after "help".
			cin.ignore( 1000000, '\n' );
		}

		// If command is "read":
		else if( command == "read" )
		{
			// Read argument (file name).
			string file_name = readArgument();

			// If no argument was provided, print message.
			if( file_name.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;
			// Otherwise, call "read" command.
			else if( !readFile( file_name ) )
				cout << "Couldn't open file. Wrong file path or format." << endl;
			else
			{
				cout << "Generating ranking of words...\n";

				// Create frequency ranking tree.
				rankFrequency();

				cout << "Success." << endl;
			}
		}

		// If command is "analyzef".
		else if( command == "analyzef" )
		{
			// Read argument (file name).
			string file_name = readArgument();

			// If no argument was provided, print message.
			if( file_name.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;
			// Otherwise, call "analyzef" command.
			else if( !analyzeFile( file_name ) )
				cout << "Couldn't open file. Wrong file path or format." << endl;
			else
				cout << "Success." << endl;
		}

		// If command is "getr".
		else if( command == "getr" )
		{
			// Read argument (word).
			string word = readArgument();

			// Change word to lowercase.
			changeToLowercase( word );

			// If no argument was provided, print message.
			if( word.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;
			// Otherwise, call "getr" command.
			else if( !getReviews( word ) )
				cout << "There's no occurrence of \"" << word << "\" in our database." << endl;
			else
				cout << "Success." << endl;
		}

		// If command is "search".
		else if( command == "search" )
		{
			// Read argument (prefix).
			string prefix = readArgument();

			// Change prefix to lowercase.
			changeToLowercase( prefix );

			// If no argument was provided, print message.
			if( prefix.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;
			// Otherwise, call "search" command.
			else if( !searchWords( prefix ) )
				cout << "There's no word starting with \"" << prefix << "\" in our database." << endl;
		}

		// If command is "print+":
		else if( command == "print+" )
		{
			// Read argument (ranking size).
			string rank_size = readArgument();

			// If no argument was provided, print message.
			if( rank_size.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;

			// Otherwise, if argument is valid, call "print+" command.
			else
			{
				// Assure argument contains only digits (0-9).
				unsigned i = 0;  // Index.
				while( i < rank_size.size() &&
						rank_size.at( i ) >= '0' && rank_size.at( i ) <= '9' )
				{
					++i;
				}
				if( i < rank_size.size() )
					cout << "Invalid argument. Enter \"help\" to list commands." << endl;

				// Argument is valid:
				else if( !this->score_ranking_.isEmpty() )
				{
					// Call "print+" command.
					printMostPositive( stoi( rank_size ) );
				}
				else
					cout << "Database is empty." << endl;
			}
		}

		// If command is "print-":
		else if( command == "print-" )
		{
			// Read argument (ranking size).
			string rank_size = readArgument();

			// If no argument was provided, print message.
			if( rank_size.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;

			// Otherwise, if argument is valid, call "print-" command.
			else
			{
				// Assure argument contains only digits (0-9).
				unsigned i = 0;  // Index.
				while( i < rank_size.size() &&
						rank_size.at( i ) >= '0' && rank_size.at( i ) <= '9' )
				{
					++i;
				}
				if( i < rank_size.size() )
					cout << "Invalid argument. Enter \"help\" to list commands." << endl;

				// Argument is valid:
				else if( !this->score_ranking_.isEmpty() )
				{
					// Call "print-" command.
					printMostNegative( stoi( rank_size ) );
				}
				else
					cout << "Database is empty." << endl;
			}
		}

		// If command is "printf":
		else if( command == "printf" )
		{
			// Read argument (ranking size).
			string rank_size = readArgument();

			// If no argument was provided, print message.
			if( rank_size.empty() )
				cout << "Command requires argument. Enter \"help\" to list commands." << endl;

			// Otherwise, if argument is valid, call "printf" command.
			else
			{
				// Assure argument contains only digits (0-9).
				unsigned i = 0;  // Index.
				while( i < rank_size.size() &&
						rank_size.at( i ) >= '0' && rank_size.at( i ) <= '9' )
				{
					++i;
				}
				if( i < rank_size.size() )
					cout << "Invalid argument. Enter \"help\" to list commands." << endl;

				// Argument is valid:
				else if( !this->score_ranking_.isEmpty() )
				{
					// Call "printf" command.
					printMostFrequent( stoi( rank_size ) );
				}
				else
					cout << "Database is empty." << endl;
			}
		}

		// If command is "exit" or "quit":
		else if( command == "exit" || command == "quit" )
		{
			// Print message.
			cout << "Ending program..." << endl;
		}

		// If command is invalid, print message.
		else if( command.empty() )
		{
			cout << "Invalid command. Enter \"help\" to list commands." << endl;

			// Ignore "any" input (up to 1 million characters) after invalid command.
			cin.ignore( 1000000, '\n' );
		}
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
			command == "analyzef" || command == "getr" || command == "search" ||
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

std::string ReviewAnalyzer::readArgument()
{
	// Ignore whitespace character after command.
	cin.ignore();

	// Argument from the user.
	string argument;

	// Read argument.
	getline( std::cin, argument );

	// Ignore whitespace before argument if it isn't empty.
	if( !argument.empty() )
	{
		unsigned i = 0;  // Index for string.
		while( i < argument.size() &&	( argument.at( i ) == ' ' ||
				argument.at( i ) == '\t' || argument.at( i ) == '\n' ) )
		{
			++i;
		}
		// Get a substring of argument without whitespace before it.
		if( i < argument.size() )
			argument = argument.substr( i );
		else
			argument.erase();
	}

	// Return command's argument.
	return argument;
}

void ReviewAnalyzer::printCommands()
{
	// Print header line.
	cout << "-- Commands list --\n";

	// Print command and description.
	cout <<  "read      <filename>  : read file and store words in database\n"
			<< "analyze   <text>      : analyze the overall emotion of <text>\n"
			<< "analyzef  <filename>  : analyze reviews from <filename>\n"
			<< "search    <prefix>    : search for words starting with <prefix>\n"
			<< "print+    <amount>    : print <amount> most positive words in database\n"
			<< "print-    <amount>    : print <amount> most negative words in database\n"
			<< "printf    <amount>    : print <amount> most frequent words in database\n"
			<< "getr      <word>      : create file listing all reviews where <word> appears\n"
			<< "help                  : print list of commands\n"
			<< "exit                  : ends program\n"
			<< "--  End of list  --"
			<< endl;
}

bool ReviewAnalyzer::readFile( std::string file_name )
{
	// Add ".txt" to file_name if user didn't write it.
	string extension;
	if( file_name.size() >= 5 )
		extension = file_name.substr( file_name.size() - 4, 4 );
	if( extension != ".txt" && extension != ".tsv" )
	{
		extension = ".txt";
		file_name = file_name + extension;
	}

	// Print message.
	cout << "Attempting to open file \"" << file_name << "\".\n";

	// File to be read.
	ifstream file;

	// Attempt to open file.
	file.open( "Files/" + file_name );
	// Validate file path.
	if( !file.is_open() )
		return false;

	// Print message.
	cout << "Reading file..." << endl;

	// Review's overall feeling.
	double score;
	// Review's text.
	string review;

	// FOR KAGGLE ONLY: Ignore header line.
	//getline( file, review );

	// Read entire file, line by line.
	while( !file.eof() )
	{
		// FOR KAGGLE ONLY:
		//file >> feeling; // Ignore phrase id.
		//file >> feeling; // Ignore sentence id.
		//file.ignore();   // Ignore '\t'.

		// Read feeling's number.
		if( extension == ".txt" )
			file >> score;

		// Read review.
		getline( file, review );
		// Store review.
		unsigned index = storeReview( review, score );

		// FOR KAGGLE ONLY: Get score by converting ASCII to integer.
		//string score_num;
		//score_num.push_back( review.back() );
		//if( score_num == "s" )
		//	break;
		//feeling = stoi( score_num );
		//review.pop_back();	  // Remove score from string.
		//if( review.back() == '\t' )
		//	review.pop_back();  // Ignore '\t' in string.

		// Words from review.
		string word;
		// Key for word in database.
		unsigned key;
		// Stream to get each word.
		istringstream line( review );

		// Insert each word in database and update scores accordingly.
		while( !line.eof() )
		{
			// Get word.
			line >> word;

			// Change word to lowercase.
			changeToLowercase( word );

			// Filter punctuation characters and concatenated words.
			if( !word.empty() && filterWord( word ) )
			{
				// Insert word and get its key.
				insert( word, score, index, &key );
				// Update word's score.
				updateScore( getSatelliteData( key ), index, score );
			}
		}
	}

	// Close file.
	file.close();

	// File was successfully read.
	return true;
}

bool ReviewAnalyzer::analyze( std::string review, double *score, bool print )
{
	// Stream to get each word from the text.
	istringstream text( review );

	string word; 	// Words from the text.
	unsigned key;  // Key to word in database.
	unsigned words_amount = 0;  // Amount of words for the score.
	double review_score = 0;    // Score of review.
	bool has_word = false;		 // Indicate a word has been found in database.

	// Read the entire text word by word.
	while( !text.eof() )
	{
		// Get a word.
		text >> word;

		// Change word to lowercase.
		changeToLowercase( word );

		// If it passes by the filter and it is in the database:
		if( filterWord( word ) && search( word, &key ) )
		{
			// Indicate so.
			has_word = true;

			// Increment amount of words.
			++words_amount;
			// Add word's score to total so far.
			review_score += getSatelliteData( key )->getScore();
		}
	}

	// If at least one word has been found in database:
	if( has_word )
	{
		// Calculate review's score.
		review_score = review_score / words_amount;

        // Return score if requested.
        if( score != nullptr )
            *score = review_score;

        // Print results if requested.
		if( print )
		{
            cout << fixed << setprecision( 2 )
				<< "--     Score    --\n";

            if( review_score < 0.5 )
                cout << "You better avoid it!\n" << review_score << " / 4.00 - Awful";
            else if( review_score < 1.5 )
                cout << "Maybe when you've got nothing better to do?\n" << review_score << " / 4.00 - Bad";
            else if( review_score < 2.5 )
                cout << "Nothing remarkable, but not bad either.\n" << review_score << " / 4.00 - OK";
            else if( review_score < 3.5 )
                cout << "Recommended! Nice one!\n" << review_score << " / 4.00 - Good";
            else
                cout << "As perfect as it can be!\n" << review_score << " / 4.00 - Excellent";

            cout << "\n-- End of score --" << endl;
		}
	}

	else
	{
	    // Return score if requested.
	    if( score != nullptr )
            *score = 2;

        // Print results if requested.
	    if( print )
	    {
            cout << "--    Result     --\n"
                    << "Oops, our database lacks enough information to calculate your review's score.\n"
                    << "-- End of result --" << endl;
	    }
	}

	return has_word;
}

bool ReviewAnalyzer::analyzeFile( std::string file_name, bool print )
{
	// Add ".tsv" to file_name if user didn't write it.
	string extension;
	if( file_name.size() >= 5 )
		extension = file_name.substr( file_name.size() - 4, 4 );
	if( extension != ".tsv" )
		file_name = file_name + ".tsv";

	// Print message.
	cout << "Attempting to open file \"" << file_name << "\"." << endl;

	// File to be read.
	ifstream input_file;

	// Attempt to open file.
	input_file.open( "Files/" + file_name );
	// Validate file path.
	if( !input_file.is_open() )
		return false;

	// Print message.
	cout << "Analyzing reviews..." << endl;

	// Remove extension from file_name.
	file_name = file_name.substr( 0, file_name.size() - 4 );

	// ID of phrase.
	unsigned phrase_id;
	// ID of sentence.
	unsigned sentence_id;
	// ID of last phrase read.
	unsigned last_phrase;

	// Review's overall feeling.
	double score;
	// Review's text.
	string review;

	// Output file.
	ofstream output_file;
	// Open output file.
	output_file.open( "Files/" + file_name + "_analysis.csv" );
	// Output header line.
	output_file << "PhraseId,Sentiment" << endl;

	// Read first line (header), ignoring it.
	getline( input_file, review );

	// Read entire file, line by line.
	while( !input_file.eof() )
	{
		// Read phrase id.
		input_file >> phrase_id;
		// Read sentence id.
		input_file >> sentence_id;
		// Read review.
		getline( input_file, review );

		// Analyze line.
		analyze( review, &score, false );

		if( phrase_id != last_phrase )
		{
			// Output phrase_id.
			output_file << phrase_id << ",";
			// Output score.
			output_file << round( score );

			if( !input_file.eof() )
				output_file << endl;
		}

		// Assign last phrase's id.
		last_phrase = phrase_id;
	}

	// Close files.
	output_file.close();
	input_file.close();

	return true;
}

bool ReviewAnalyzer::insert( std::string word, double phrase_score, unsigned index, unsigned *key )
{
	// Insert word in Trie Tree to search for prefix.
	this->words_.insert( word );

	// Insert word in database.
	return this->database_.insert( word, phrase_score, index, key );
}

bool ReviewAnalyzer::search( std::string word, unsigned *key )
{
	// Change word to lowercase.
	changeToLowercase( word );

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

bool ReviewAnalyzer::getReviews( std::string word )
{
	// Word's index in hash table.
	unsigned key;

	// Change word to lowercase.
	changeToLowercase( word );

	if( search( word, &key ) )
	{
		// Print message.
		cout << "Listing reviews..." << endl;

		// Output file.
		ofstream file( "Files/" + word + "_reviews.csv" );

		// Data from hash table.
		StringHashData *word_data = getSatelliteData( key );

		// Output word and its score.
		file << "Word: " << word << '\n'
				<< fixed << setprecision( 2 )
				<< "Score: " << word_data->getScore() << '\n' << endl;

		// Word's reviews.
		vector< unsigned > indexes = word_data->getIndexes();

		// Output each review where word appears to file.
		for( auto index : indexes )
		{
			// Output review's score.
			file << this->reviews_.at( index ).first << ' ';
			// Output review's text.
			file << this->reviews_.at( index ).second << endl;
		}

		file.close();
		return true;
	}

	// Word not found.
	return false;
}

bool ReviewAnalyzer::searchWords( std::string word )
{
	// Search for word, printing words found.
	return this->words_.search( word );
}

StringHashData* ReviewAnalyzer::getSatelliteData( unsigned key )
{
	// Return data at position key.
	return this->database_.getData( key );
}

bool ReviewAnalyzer::filterWord( std::string &word )
{
	// If word is a punctuation mark, it can't be inserted.
	if( word.empty() || word.at( 0 ) == '.' || word.at( 0 ) == ',' || word.at( 0 ) == ';' ||
			word.at( 0 ) == ':' || word.at( 0 ) == '\"' || word.at( 0 ) == '?' ||
			word.at( 0 ) == '!' || word.at( 0 ) == '-' || word.at( 0 ) == '\'' ||
			word.at( 0 ) == '\t' || word.at( 0 ) == '\n' || word.at( 0 ) == ' ' ||
			word.at( 0 ) == '´' || word.at( 0 ) == '`' || word.at( 0 ) == '~' ||
			word.at( 0 ) == '^' || word.at( 0 ) == '/' || word.at( 0 ) == '0' ||
			word.at( 0 ) == '1' || word.at( 0 ) == '2' || word.at( 0 ) == '3' ||
			word.at( 0 ) == '4' || word.at( 0 ) == '5' || word.at( 0 ) == '6' ||
			word.at( 0 ) == '7' || word.at( 0 ) == '8' || word.at( 0 ) == '9' )
	{
		return false;
	}
	// If word is a stopword, it can't be inserted.
	else if( isStopword( word ) )
		return false;

	// Otherwise, it can be inserted.
	return true;
}

bool ReviewAnalyzer::readStopwords( std::string file_name )
{
	// Add ".txt" to file_name if client didn't provide it.
	string extension;
	if( file_name.size() >= 5 )
		extension = file_name.substr( file_name.size() - 4, 4 );
	if( extension != ".txt" )
		file_name = file_name + ".txt";

	// File to be read.
	ifstream file;

	// Attempt to open file.
	file.open( "Files/" + file_name );
	// Validate file path.
	if( !file.is_open() )
		return false;

	// Stopword to be filtered.
	string stopword;

	// Read entire file, line by line.
	while( !file.eof() )
	{
		// Read review.
		file >> stopword;

		// Insert it in hash table.
		this->stopwords_.insert( stopword, 0, 0 );
	}

	// Close file.
	file.close();

	// File was successfully read.
	return true;
}

bool ReviewAnalyzer::isStopword( std::string word )
{
	// Search for word in stopword hash table.
	return this->stopwords_.search( word );
}

unsigned ReviewAnalyzer::storeReview( std::string review, unsigned score )
{
	// Create pair< score, review >.
	pair< unsigned, std::string > review_pair( score, review );

	// Put pair into vector.
	this->reviews_.push_back( review_pair );

	// Return index.
	return this->reviews_.size() - 1;
}

void ReviewAnalyzer::updateScore( StringHashData *satellite_data, unsigned index, double new_score )
{
	// If new_score is negative (standard argument), satellite_data isn't in the tree
	// and must be inserted.
	if( new_score < 0.0 )
		this->score_ranking_.insert( satellite_data );

	// If satellite_data is in the tree, remove it and reinsert it.
	else if( this->score_ranking_.search( satellite_data ) )
	{
		this->score_ranking_.remove( satellite_data );
		satellite_data->recalculateScore( new_score, index );
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
		AvlTreeNode *left = traverse.top()->getLeftDescendant();
		// Node's right descendant.
		AvlTreeNode *right = traverse.top()->getRightDescendant();

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
		*i = tolower( *i, global );
}
