/**
 * Class TrieNode member-function definitions.
 *
 *  Created on: July 14th, 2017
 *     Authors: Aline Weber
 *              Renan Kummer
 */

// Libraries:
#include <stack>
#include <utility>
#include <string>
#include <iostream>
#include "../Classes/TrieNode.h"

using namespace std;

TrieNode::TrieNode()
{
	// Initialize node's key to indicate there's no data.
	this->key_ = 0;

	// Set node 
	this->endWord_ = false;

	this->children_;

}

void TrieNode::insert( string word)
{
	TrieNode* new_node;
	vector<TrieNode*> children;
	TrieNode* node = this; // Set root as current node

	// For each char in word
	for(int i = 0; i < word.length(); i++){
		children = node->getChildren(); // Get all children from root

		// If the current char is already at the Trie, just change the current node to the specific one 
		int index = node->getChildIndex(word[i]);
		if(index != -1){
			new_node = children[index];
		}
		//If is not at the Trie, create a new node, and link the current one to the new
		else{
			new_node = new TrieNode;
			new_node->setKey(word[i]);
			node->addChild(new_node);
		}
		//Set new node as a last char, if that's the case
		if(i == (word.length()-1)){
			new_node->setEndWord(true);
		}
		//Change node for next iteration
		node = new_node;
	}	
}

TrieNode* TrieNode::nodeSearch( string word, TrieNode* current_node)
{

	TrieNode* new_node;
	vector<TrieNode*> children;
	TrieNode* node = current_node; // Set root as current node

	// For each char in word
	for(int i = 0; i < word.length(); i++){
		children = node->getChildren(); // Get all children from root
		// If the current char is at the Trie, just change the current node to the specific next one 
		int index = node->getChildIndex(word[i]);
		if(index != -1){
			new_node = children[index];
		}
		//If is not at the Trie, the word can't be found
		else{
			return nullptr;
		}
		//If it's the last char, return the last node;
		if(i == (word.length()-1)){
			return new_node;
		}
		//Change node for next iteration
		node = new_node;
	}
	return nullptr;	
}

void TrieNode::printAll( string prefix, TrieNode* current_node)
{
	if(current_node->getEndWord()){
		cout << prefix << "\n";
	}

	vector<TrieNode*> children = current_node->getChildren();

	for(int i = 0; i<children.size(); i++){
		TrieNode* child = children[i];
		prefix.push_back(child->getKey());
		printAll(prefix, child);
		prefix.pop_back();
	}
}

int TrieNode::getChildIndex(char ch){
	vector<TrieNode* > children = this->getChildren();
	for(int i=0; i<children.size(); i++){
		if(children[i]->getKey() == ch)
			return i;
	}
	return -1;
}

bool TrieNode::setKey( char key )
{
	// Assign key argument to node's key.
	return this->key_ = key;
}

char TrieNode::getKey()
{
	// Return node's key.
	return this->key_;
}

void TrieNode::setEndWord( bool end_word )
{

	// Assign end_word argument to node's endWord flag.
	this->endWord_ = end_word;
}

bool TrieNode::getEndWord()
{
	// Return node's endWord flag.
	return this->endWord_;
}

bool TrieNode::addChild( TrieNode* child )
{
	
	// Add child argument to node's children vector.
	this->children_.push_back(child);
	return true;
}

vector<TrieNode*> TrieNode::getChildren()
{
	// Return node's children.
	return this->children_;
}
