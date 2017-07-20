/**
 *  Definitions of StringHashData class - satellite data for StringHashTable class.
 *
 *  Created on: July 7th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

#ifndef STRINGHASHDATA_H_
#define STRINGHASHDATA_H_

// Libraries:
#include <string>
#include <vector>


class StringHashData {
public:
	/**
	 *  Constructor: sets hash table position as free.
	 */
	StringHashData();

	/**
	 * Constructor: sets hash table position as free, sets word (satellite data) and
	 * initialize it with its score in the phrase.
	 *  @param word String to be inserted.
	 *  @param phrase_score Overall score of phrase where this word was written.
	 */
	StringHashData( std::string word, double phrase_score );

	/**
	 * Set key and word data.
	 *  @param word String to be inserted.
	 *  @param phrase_score Overall score of phrase where this word was written.
	 *  @param index Index to review where this word was written.
	 *  @return True if inserted, false it was already inserted..
	 */
	bool insert( std::string word, double phrase_score, unsigned index );

	/**
	 * Remove satellite data and reset score and frequency.
	 */
	void remove();

	/**
	 * Recalculate score of expressed feelings with phrase_score.
	 *  @param phrase_score Overall score of phrase where this word was written.
	 *  @param index Index to review where this word was written.
	 *  @return True if phrase_score is valid, false otherwise.
	 */
	bool recalculateScore( double phrase_score, unsigned index );

	/**
	 * Get satellite data (word from text).
	 *  @return Satellite data (word from text).
	 */
	std::string getWord();

	/**
	 * Get this word's score of expressed feelings.
	 *  @param interpret Indicate if score should be interpreted to negative/positive/neutral,
	 *                   in which case a string must be passed by reference as argument.
	 *  @param value String with negative, positive or neutral evaluation, required if client
	 *               wants the score to be interpreted.
	 *  @return Word's score of expressed feelings.
	 */
	double getScore( bool interpret = false, std::string *value = nullptr );

	/**
	 * Get this word's frequency in the reviews.
	 *  @return Word's frequency.
	 */
	unsigned getFrequency();

	/**
	 * Get indexes of reviews where this word appeared.
	 *  @return Vector of indexes.
	 */
	std::vector< unsigned > getIndexes();

private:
	/**
	 * Set this word's score of expressed feelings.
	 *  @param score Word's score.
	 *  @return True if score is valid, false otherwise.
	 */
	bool setScore( double score );

	/**
	 * Include a given review's index at indexes_ vector.
	 *  @param index Review's index.
	 *  @return True if included, false if it was already included.
	 */
	bool addIndex( unsigned index );

	/**
	 * Increment word's frequency.
	 */
	void incrementFrequency();

	/**
	 * Reset frequency to zero.
	 */
	void resetFrequency();


// Attributes:
	std::string satellite_data_;      // Word from review.
	std::vector< unsigned > indexes_; // Indexes of reviews where word was written.

	double score_;        // Score for expressed feelings.
	unsigned frequency_;  // Amount of times word was found in reviews.
};

#endif /* STRINGHASHDATA_H_ */
