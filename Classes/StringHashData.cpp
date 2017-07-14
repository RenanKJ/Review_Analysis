/**
 * Class StringHashData member-function definitions.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include "StringHashData.h"

using namespace std;


StringHashData::StringHashData()
{
	this->score_ = 0.0;   // Initial score.
	this->frequency_ = 0; // No frequency.

	// Indicate that position hasn't been used yet.
	this->was_occupied_ = false;
}

StringHashData::StringHashData( std::string word, double phrase_score )
{
	this->satellite_data_ = word; // Set satellite data (word from text).

	this->score_ = phrase_score;  // Set initial score as phrase's score.
	this->frequency_ = 1;         // Initial frequency.

	// Indicate that position has been used already.
	this->was_occupied_ = true;
}

bool StringHashData::insert( std::string word, double phrase_score )
{
	// If position is free, insert word:
	if( getData().empty() )
	{
		// Assign word as satellite data.
		this->satellite_data_ = word;
		// Set initial frequency as one (1).
		incrementFrequency();
		// Set initial score as phrase_score.
		setScore( phrase_score );

		// Set position as already occupied.
		this->was_occupied_ = true;

		// Indicate word was inserted.
		return true;
	}

	// Indicate position is occupied and word wasn't inserted.
	return false;
}

void StringHashData::remove()
{
	// Erase stored word.
	this->satellite_data_.clear();

	// Reset score to zero.
	setScore( 0.0 );
	// Reset frequency to zero.
	resetFrequency();
}

void StringHashData::clear()
{
	// Remove satellite data and reset score and frequency.
	remove();

	// Reset was_occupied flag.
	this->was_occupied_ = false;
}

bool StringHashData::recalculateScore( double phrase_score )
{
	// If phrase_score is valid (0.0 ~ 4.0):
	if( phrase_score >= 0.0 && phrase_score <= 4.0 )
	{
		// Get sum of all scores so far.
		unsigned sum_of_scores = static_cast< unsigned >( getScore() * getFrequency() + phrase_score );
		// Increment frequency by one (1).
		incrementFrequency();
		// Calculate and set new score.
		setScore( sum_of_scores / static_cast< double >( getFrequency() ) );

		// Indicate that score was recalculated.
		return true;
	}

	// Since phrase_score is invalid, return false.
	return false;
}

std::string StringHashData::getData()
{
	// Return satellite data (word).
	return this->satellite_data_;
}

double StringHashData::getScore( bool interpret, std::string *value )
{
	// If the score is to be interpreted, change value string to reflect word's score.
	if( interpret )
	{
		// Negative = 0.00 ~ 0.95
		if( this->score_ < 0.95 )
			*value = "negative";
		// Slightly negative = 0.95 ~ 1.95
		else if( this->score_ < 1.95 )
			*value = "slightly negative";
		// Neutral = 1.95 ~ 2.95
		else if( this->score_ < 2.95 )
			*value = "neutral";
		// Slightly positive = 2.95 ~ 3.95
		else if( this->score_ < 3.95 )
			*value = "slightly positive";
		// Positive = 3.95 ~ 5.00
		else
			*value = "positive";
	}

	// Return word's score.
	return this->score_;
}

unsigned StringHashData::getFrequency()
{
	// Return word's frequency.
	return this->frequency_;
}

bool StringHashData::wasOccupied()
{
	// Check if this position has been occupied at least once.
	return this->was_occupied_;
}

bool StringHashData::setScore( double score )
{
	// Assign score to this word's score if valid:
	if( score >= 0.0 && score <= 4.0 )
	{
		// Set word's score.
		this->score_ = score;
		// Indicate that score is valid.
		return true;
	}

	// Since score is invalid, return false.
	return false;
}

void StringHashData::incrementFrequency()
{
	// Increment frequency by one (1).
	this->frequency_++;
}

void StringHashData::resetFrequency()
{
	// Set frequency to zero (0).
	this->frequency_ = 0;
}
