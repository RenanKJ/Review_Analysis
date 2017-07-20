/**
 * Definitions of ReviewAnalyzer class - it analyzes reviews in terms of expressed feelings.
 *
 *  Created on: July 16th, 2017
 *     Authors: Aline Weber,
 *              Renan Kummer
 */

#ifndef REVIEWANALYZER_H_
#define REVIEWANALYZER_H_

// Libraries:
#include <string>
#include <utility>
#include "StringHashTable.h"
#include "Trie.h"


class ReviewAnalyzer {
public:
	/**
	 * Constructor: initializes ranking trees and hash table (standard size is 150,001).
	 *  @param hash_size Hash table's initial size.
	 */
	ReviewAnalyzer( unsigned hash_size = 100001 );

	/**
	 * Run analysis program.
	 *  @return Finishing condition.
	 *  	0 = exit command
	 */
	int run();

	/**
	 * Read a command from the user.
	 *  @return Command input by the user if valid, empty string if invalid.
	 */
	std::string readCommand();

	/**
	 * Read command's argument from the user.
	 *  @return Command's argument.
	 *  	(1) Review text, or
	 *  	(2) File path.
	 */
	std::string readArgument();

	/**
	 * Command "help": print list of commands.
	 */
	void printCommands();

	/**
	 * Command "read": read a file of reviews and build database of words and feelings they express.
	 * The file must be formatted as following:
	 *    (1) each line has a single review,
	 *    (2) each line starts with a number from 0-4 expressing the review's overall feeling
	 *    	[0=negative, 1=slightly negative, 2=neutral, 3=slightly positive, 4=positive],
	 *    (3) this number must be followed by a space,
	 *    (4) each word must be separated by spaces (including concatenated words, e.g. "does n't").
	 * Also, it needs to be in the same directory as the executable.
	 *
	 *  @param file_name Name of file to be read.
	 *  @return True if file was read, false if there were any errors.
	 */
	bool readFile( std::string file_name );

	/**
	 * Command "analyze": analyze a review's overall feeling.
	 *  @param review Text to be analyzed.
	 *  @param score Text's score after analysis (optional).
	 *  @param print Indicate if result must be printed (optional).
	 *  @return True if at least one word was in the database, false otherwise.
	 */
	bool analyze( std::string review, double *score = nullptr, bool print = true );

    /**
     * Command "analyzef": analyze a file of reviews and output results in "analysis.csv".
     *  @param file_name Name of reviews' file.
     *  @return True if analyzed, false otherwise.
     */
     bool analyzeFile( std::string file_name, bool print = false );
	/**
	 * Insert word in database.
	 *  @param word Word to be inserted.
	 *  @param phrase_score Word's score in the phrase.
	 *  @param index Phrase's index.
	 *  @param key Word's key in the hash table if found (optional).
	 *  @return True if inserted, false if it's already in the database.
	 */
	bool insert( std::string word, double phrase_score, unsigned index, unsigned *key = nullptr );

	/**
	 * Search for a word in database.
	 *  @param word Word to be searched for.
	 *  @param key Word's key in the database if found (optional).
	 *  @return True if found, false otherwise.
	 */
	bool search( std::string word, unsigned *key = nullptr );

	/**
	 * Remove a word from database.
	 *  @param word Word to be removed.
	 */
	void remove( std::string word );

	/**
	 * Command "print+": print a ranking of the most positive words in the database.
	 *  @param rank_size How many words can be in the ranking list.
	 */
	void printMostPositive( unsigned rank_size );

	/**
	 * Command "print-": print a ranking of the most negative words in the database.
	 *  @param rank_size How many words can be in the ranking list.
	 */
	void printMostNegative( unsigned rank_size );

	/**
	 * Command "printf": print a ranking of the most frequent words in the database.
	 *  @param rank_size How many words can be in the ranking list.
	 */
	void printMostFrequent( unsigned rank_size );

	/**
	 * Command "getr": create file <word>"_reviews.csv" containing all reviews where
	 * a given word appears.
	 *  @param word Word to search for.
	 *  @return True if file was created, false otherwise.
	 */
	bool getReviews( std::string word );

	/**
	 * Command "search": search for words starting with given string, printing them.
	 *  @param word Word or partial word to search for.
	 *  @return True if a word has been found, false otherwise.
	 */
	bool searchWords( std::string word );

private:
	/**
	 * Get satellite data at position key by reference.
	 *  @param key Position in database.
	 *  @return Pointer to satellite data.
	 *  @attention This method breaks encapsulation of StringHashTable class,
	 *             use it carefully.
	 */
	StringHashData* getSatelliteData( unsigned key );

	/**
	 * Filter characters (e.g. comma) or words (e.g. isn't) for insertion.
	 *  @param word Word to be filtered. Return two words if they're concatenated.
	 *  @return True if insertion is allowed (although two words may need insertion)
	 *          and false otherwise.
	 */
	bool filterWord( std::string &word );

	/**
	 * Read stopwords from a file.
	 *  @param file_name Name of file with stopwords.
	 *  @result True if file was read, false otherwise.
	 */
	bool readStopwords( std::string file_name );

	/**
	 * Check if a word is a stopword.
	 *  @param word Word to be evaluated.
	 *  @return True if it is, false otherwise.
	 *  @attention False means it isn't in stopwords' database.
	 */
	bool isStopword( std::string word );

	/**
	 * Store review and its score.
	 *  @param review Review's text.
	 *  @param score Review's score.
	 *  @return Vector's index where this pair was inserted.
	 */
	unsigned storeReview( std::string review, unsigned score );

	/**
	 * Update words' score and ranking.
	 *  @param satellite_data Word to be inserted or updated in ranking tree.
	 *  @param index Review's index.
	 *  @param new_score Satellite data's next score.
	 *  @return True if there're any words to rank, false otherwise.
	 */
	void updateScore( StringHashData *satellite_data, unsigned index, double new_score = -1.0 );

	/**
	 * Create words's frequency ranking tree.
	 *  @return True if there're any words to rank, false otherwise.
	 *  @attention This method has a high cost, it is recommended to use it only
	 *             after all words have been read.
	 */
	bool rankFrequency();

	/**
	 * Change word (or phrase) to lowercase.
	 *  @param word Word (or phrase) to be changed.
	 */
	void changeToLowercase( std::string &word );


// Attributes:
	StringHashTable database_;  // Database of words and feelings they express.

	// Ranking trees.
	AvlTree score_ranking_;
	AvlTree frequency_ranking_;

	// Trie Tree of words.
	Trie words_;

	// Stopwords to filter.
	StringHashTable stopwords_;

	// Reviews - pair< score, review >.
	std::vector< std::pair< unsigned, std::string > > reviews_;
};

#endif /* REVIEWANALYZER_H_ */
