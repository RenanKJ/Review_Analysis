/* Definitions of StringHashData class - satellite data for StringHashTable class.
 *
 *  Created on: July 7th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

#ifndef STRINGHASHDATA_H_
#define STRINGHASHDATA_H_

// Libraries:
#include <string>
#include <utility>


class StringHashData {
public:
	/**
	 *  Constructor: sets hash table position as free.
	 */
	StringHashData();

	/**
	 * Constructor: sets hash table position as free, sets word (satellite data) initialized
	 * with its score in the phrase.
	 *  @param word String to be inserted.
	 *  @param phrase_score Overall score of phrase where this word was written.
	 */
	StringHashData( std::string word, double phrase_score );

	/**
	 * Set key and word data.
	 *  @param word String to be inserted.
	 *  @param phrase_score Overall score of phrase where this word was written.
	 *  @return True if inserted or previously inserted (in which case score and quantity,
	 *          are recalculated), false if there were any collisions.
	 */
	bool insert( std::string word, double phrase_score );

	/**
	 * Remove satellite data and reset score and frequency.
	 */
	void remove();

	/**
	 * Clear all data, including was_occupied flag.
	 */
	void clear();

	/**
	 * Recalculate score of expressed feelings with phrase_score.
	 *  @param phrase_score Overall score of phrase where this word was written.
	 *  @return True if phrase_score is valid, false otherwise.
	 */
	bool recalculateScore( double phrase_score );

	/**
	 * Get satellite data (word from text).
	 *  @return Satellite data (word from text).
	 */
	std::string getData();

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
	 * Check if this position has been previously occupied by any satellite data.
	 *  @return True if it was (or still is) occupied, false otherwise.
	 */
	bool wasOccupied();

private:
	/**
	 * Set this word's score of expressed feelings.
	 *  @param score Word's score.
	 *  @return True if score is valid, false otherwise.
	 */
	bool setScore( double score );

	/**
	 * Increment word's frequency.
	 */
	void incrementFrequency();

	/**
	 * Reset frequency to zero.
	 */
	void resetFrequency();


// Attributes:
	std::string satellite_data_;  // Word from review.

	double score_;        // Score for expressed feelings.
	unsigned frequency_;  // Amount of times word was found in reviews.

	bool was_occupied_;   // Indicate if key_ position in hash table was occupied at least once.
};


#endif /* STRINGHASHDATA_H_ */
