#include <iostream>     
#include <string>     

#include "Text.h"

using std::string;
using std::ostream;


void resize(string*& text, uint length, uint& capacity, uint new_capacity){
	if (new_capacity > capacity) {
		// Allocate memory for an array of size new_capacity
		string * temp = new string[new_capacity];
		// Copy the contents of old array into the new one
		for (int i=0; i<length; i++){
			temp[i] = text[i];
		}
		// Free memory
		delete[] text;
		// Point text to new memory location where a larger amount of memory was allocated
		text = temp;
		// Finished with temp so make it point to nothing
		temp = nullptr;
		// Update the capacity of current array to new_capacity
		capacity = new_capacity;
	}
}

string* readText(istream& in, uint& length, uint& capacity){	
	string* text=nullptr;
	// The current word being read
	string word = "";
	while(true){
		in >> word;
		// Stop execution of loop if the word "EOT" is read
		if (word.compare("EOT")==0){
			break;
		}
		// Need to update new_capacity so array can be resized to fit the new word
		int new_capacity = capacity + 1;
		resize(text, length, capacity, new_capacity);
		text[length] = word;
		// Just added new word to array, so update the length
		length++;
	}
	return text;
}


void outputText(ostream& out, const string* text, uint length){
	// Output contents of text array with each element separated by a space
	for (int i=0; i<length; i++){
			out << text[i] << " ";
	}
}

void appendText(string*& text, uint& length, uint& capacity, 
	            const string * const other, uint length_other){ 
					// Used when we iterate over the second array
					int x = 0;
					// Use resize function to allocate memory of required size
					resize(text, length, capacity, capacity + length_other);
					// Iterate over the resized array filling the space with contents of second array
					// starting at the point at which the first sequence of words ends
					for (int i=length; i<length+length_other; i++){
						text[i] = other[x];
						x++;
					}
					// Reset iterator variable to 0
					x=0;
					// Update the length
					length = length + length_other;
					// Do not need to update capacity here as this is done in resize()		
}


bool containsWord(const string *text, uint length, const string& word){
	bool contains=false;
	// Iterate over array text
	for (int i=0; i<length; i++){ 
		if (!text[i].compare(word)) {
			contains=true;
		}
	}
	return contains;
}

uint wordCount(const string *text, uint length, const string& word){
	// Keeps track of how many times word has been encountered in array text
	uint count=0;
	for (int i=0; i<length; i++){
		if (!text[i].compare(word)) {
			count++;
		}
	}
	return count;
}

string* getWords(const string *text, uint length, uint& number_unique, uint& capacity_unique){
	string* unique_words=nullptr;
	// Iterate over text array
	for (int i=0; i<length; i++) {
		if (!containsWord(unique_words, number_unique, text[i])){
			// Incease capacity of unique_words by 1 only if we are adding a unique word to it
			resize(unique_words, number_unique, capacity_unique, capacity_unique+1);
			unique_words[number_unique] = text[i];
			// Increase total number of unique words in unique_words array by 1
			number_unique++;
		}
	}
	return unique_words;
}

void replaceWordAll(string*& text, uint length, const string& word, const string& new_word){
	for (int i=0; i<length; i++){
		// If they are the same at index i
		if (!text[i].compare(word)){
			// Do the replacement at index i
			text[i] = new_word;
		}
	}
}

void deleteWordAll(string*& text, uint& length, const string& word){
	// Save the original length because length gets decremented as words are deleted
	uint orig_length = length; 
	// Iterate over array text
	for (int i = 0; i < length; i++) {
		if (!text[i].compare(word)) {
			// Then they are the same, so delete it by shifting left
			for (int j = i; j < length; j++) {
				// If it's the last word...
				if (j == (length - 1)) {
					// .. make it an empty string because there is no word after it
					text[j] = "";
					break;
				}
				// Overwrite current word with the next word (shifting left)
				text[j] = text[j + 1];
			}
			// Every time we delete a word, length is decremented by 1
			length--;
		}
	}
}

void replacePhraseFirst(string*& text, uint& length, uint& capacity,
	               const string *phrase, uint phrase_length,
	               const string *new_phrase, uint new_phrase_length){
	bool found = false;
	// Store the index of the first matching element of the first matching phrase
	int k=0;
	for (int i = 0; i < length; i++) {
		// If we find the first matching phrase, break the for loop to stop searching
		if (found) {
			break;
		}
		// Check whether element of i is the same as first element of phrase
		if (!text[i].compare(phrase[0])) {
			// If we pass the above check it means an element in text that is first word of phrase has been found
			// Update k with index i
			k = i;
			// Check whether the rest of the phrase matches
			for (int j = 0; j < phrase_length; j++) {
			
				// If an element does not match where it should, stop checking - break for loop
				if (text[i].compare(phrase[j])) {
					//	Break and try to find next occurance of first word of phrase
					break;
				}
				// Move on to next element for checking
				i++;
				// Once we reach the end, we have found a matching phrase
				if (j == phrase_length - 1) {
					found = true;
				}
			}
		}
	}
	// Only do this if found an occurance of phrase
	if (found) {
		int new_length = length - phrase_length + new_phrase_length;
		int diff = new_phrase_length - phrase_length;

		// If new_phrase_length > phrase_length
		if (diff > 0) {
			resize(text, length, capacity, new_length);
			// Shift the elements that come after the phrase by amount diff
			int last_index = length - 1;
			for (int i = last_index; i >= k + phrase_length; i--) {
				text[i + diff] = text[i];
			}
		}
		// new_phrase is shorter than phrase
		else {
			int start = k + new_phrase_length;
			for (int x = start; x < length-1; x++) {
				text[x] = text[x - diff];
			}
		}
		// Overwrite phrase with new_phrase
		int j = 0;
		// Start at k because this is the position of first element of phrase
		for (int i = k; i < k + new_phrase_length; i++) {
			text[i] = new_phrase[j];
			j++;
		}
		length = new_length;
	}
}

