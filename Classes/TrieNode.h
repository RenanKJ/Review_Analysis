/**
 * Definitions of TrieNode class - implements a node of an Trie data structure
 *
 *  Created on: July 7th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

#include <vector>
#include <string>
#ifndef TRIENODE_H_
#define TRIENODE_H_

class TrieNode {
public:

	/**
	 * Constructor: initializes pointers as null pointers.
	 */
	TrieNode();

	/**
	 * Insert data into the Trie tree, returning pointer to tree's root.
	 *  @param word to be inserted at the Trie.
	 */
	void insert( std::string word );

	/**
	 * Print all words at the Trie that starts at given node
	 *  @param prefix to be searched for at Trie.
	 */
	void printAll( std::string prefix, TrieNode* current_node);

	/**
	 * Search for the node at where the given word is complete.
	 *  @param word to be searched for at the Trie.
	 *  @return TrieNode pointer to the node, nullptr if the word was not found
	 */
	TrieNode* nodeSearch( std::string word, TrieNode* current_node);
	/**
	 * Get node's key.
	 *  @return Node's key.
	 */
	char getKey();

	/**
	 * Set node's key.
	 *  @param key Node's key.
	 */
	bool setKey( char key );

	/**
	 * Set node's flag of end word.
	 *  @param end_word Node's flag of end word.
	 */
	void setEndWord( bool end_word );

	/**
	 * Get node's flag of end word.
	 *  @return Node's flag of end word.
	 */
	bool getEndWord();

	/**
	 * Add node's child.
	 *  @param child Node's child.
	 */
	bool addChild( TrieNode* child );

	/**
	 * Get node's children.
	 *  @return Node's children.
	 */
	std::vector<TrieNode*> getChildren();


private:
	/**
	 * Check if the given char is a child of the node.
	 * @param ch - char to be checked
	 * @return int - index of the child if is there, -1 otherwise
	 */
	int getChildIndex(char ch);

	// Attributes:
	char key_;            // Node's key 
	std::vector<TrieNode*> children_;  // Pointers to the node's children.
	bool endWord_;   // Flag indicating if the node can be the end of a word.

};

#endif /* AVLTREENODE_H_ */
