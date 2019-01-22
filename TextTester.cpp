#include <stdlib.h>
#include <iostream>
#include <utility>
#include <sstream> 

#include "TextTester.h"
#include "Text.h"

using std::cout;
using std::cerr;
using std::endl;
using std::vector;
using std::string;

const string TextTester::DEL_WORD = "DELETE";

TextTester::TextTester() : error_(false), funcname_("") {
	unsigned int val(time(0)*rand());
	srand(val);
}

TextTester::~TextTester() {
}

void TextTester::testResize(){
	funcname_ = "testResize";

	string* text = nullptr;
	unsigned int length{0};
	unsigned int capacity{0};
	
	unsigned int new_capacity=rand()%SMALL_CAPACITY+1;
	resize(text,length,capacity,new_capacity);
	if(capacity!=new_capacity)
    	errorOut_("Capacity not increased correctly.",1);
    	
    
    if(text!=nullptr)
    	delete [] text;
    text = nullptr;	
	length=0;
	capacity=0;
	vector<string> text_vector;
	text = generateText_(text_vector,length,capacity);
    unsigned int capacity_copy = capacity;
    resize(text, length, capacity, capacity);
    if(capacity!=capacity_copy)
    	errorOut_("Modified capacity when new capacity equal to old.",2);

    if(!checkText_(text,text_vector,length))
    	errorOut_("Modified contents.",3);	


    if(text!=nullptr)
    	delete [] text;
    text = nullptr;     
    length=0;
    capacity=0;
    text = generateText_(text_vector,length,capacity);
    capacity_copy=capacity;
	unsigned int larger = capacity + rand()%SMALL_CAPACITY +1;
    resize(text, length, capacity, larger);
    if(capacity!=larger)
    	errorOut_("Capacity not increased correctly.",4);
    if(!checkText_(text,text_vector,length))
    	errorOut_("Modified contents.",5);	
    
    
    if(text!=nullptr)
    	delete [] text;
    text = nullptr;		
    length=0;
    capacity=0;
    text = generateText_(text_vector,length,capacity);
    capacity_copy=capacity;
    unsigned int smaller = capacity - (rand()%capacity +1);
	resize(text, length, capacity, smaller);
    if(capacity!=capacity_copy)
    	errorOut_("Modified capacity when new capacity smaller than old.",6);
    if(!checkText_(text,text_vector,length))
    	errorOut_("Modified contents.",7);	

    if(text!=nullptr)
        delete [] text;
    text = nullptr; 
    
	passOut_("Tested the resizing.");
}

void TextTester::testReadText(){
	funcname_ = "testReadText";

	unsigned int length_gen{SMALL_CAPACITY};
	unsigned int capacity_gen{2*SMALL_CAPACITY};
	vector<string> text_vector;
	string* tmp = generateText_(text_vector,length_gen,capacity_gen);
    if(tmp!=nullptr)
        delete [] tmp;
    tmp=nullptr;

	string whitespace [3] = {" ","\t","\n"};
	std::stringstream sstream;
    for(int i=0;i<text_vector.size();++i){
		sstream<< text_vector[i]<<whitespace[rand()%3];
	}
	sstream<<END_OF_TEXT;

	unsigned int length{0};
	unsigned int capacity{0};
	string* text = readText(sstream, length, capacity);

    if(length!=length_gen){
    	errorOut_("Text has wrong length.",1);
    }
    if(length > capacity){
    	errorOut_("Length larger than capacity.",2);
    }
    if(!checkText_(text,text_vector,length))
    	errorOut_("Wrong text contents.",3);	

    if(text!=nullptr)
        delete [] text;
    text=nullptr;

	passOut_("Tested reading a text from an input stream.");
}

void TextTester::testAppendText(){
	funcname_ = "testAppendText";

	unsigned int length_gen1{SMALL_CAPACITY};
	unsigned int capacity_gen1{SMALL_CAPACITY};
	vector<string> text_vector1;
	string* text1 = generateText_(text_vector1,length_gen1,capacity_gen1);
	unsigned int length1 = length_gen1;
	unsigned int capacity1 = capacity_gen1;

	unsigned int length_gen2{0};
	unsigned int capacity_gen2{0};
	vector<string> text_vector2;
	string* text2 = generateText_(text_vector2,length_gen2,capacity_gen2);
	unsigned int length2 = length_gen2;
	unsigned int capacity2 = capacity_gen2;

	text_vector1.insert(text_vector1.end(), text_vector2.begin(), text_vector2.end());

	appendText(text1, length1, capacity1, text2, length2);

    if(length1!=length_gen1+length_gen2){
    	errorOut_("Result has wrong length.",1);
    }
    if(length1 > capacity1){
    	errorOut_("Length of result is larger than its capacity.",2);
    }
    if(!checkText_(text1,text_vector1,length1))
    	errorOut_("Wrong text contents.",3);	

    if(text1!=nullptr)
        delete [] text1;
    text1=nullptr;

    if(text2!=nullptr)
        delete [] text2;
    text2=nullptr;

	passOut_("Tested appending texts.");
}

void TextTester::testDeleteWordAll(){
	funcname_ = "testDeleteWordAll";

	unsigned int length_gen{6};
	unsigned int capacity_gen{6};
	vector<string> text_vector;
	string* text = generateText_(text_vector,length_gen,capacity_gen);
	unsigned int length = length_gen;
	unsigned int capacity = capacity_gen;

	unsigned int mid = length/2;
	unsigned int numDel = 0;
	if(length >= 1){
		text[0] = DEL_WORD;
		text_vector.erase(text_vector.begin());
		numDel=1;
		if(length >= 2){
			text[length-1] = DEL_WORD;
			text_vector.erase(text_vector.end()-1);
			numDel=2;
			if(length >= 3){
				text[mid] = DEL_WORD;
				text_vector.erase(text_vector.begin()+(mid - 1));
				numDel=3;
			}
		}
	}

	deleteWordAll(text, length, DEL_WORD);

    if(length != length_gen - numDel){
    	errorOut_("Result has wrong length.",1);
    }
    if(length > capacity){
    	errorOut_("Length of result is larger than its capacity.",2);
    }
    if(!checkText_(text,text_vector,length))
    	errorOut_("Wrong text contents.",3);

	deleteWordAll(text, length, DEL_WORD);

   if(length != length_gen - numDel){
    	errorOut_("Changed length when word not present.",4);
    }
    if(length > capacity){
    	errorOut_("Length is larger than capacity.",5);
    }
    if(!checkText_(text,text_vector,length))
    	errorOut_("Wrong text contents.",6);

    if(text!=nullptr)
        delete [] text;
    text=nullptr;

	passOut_("Tested deleting a word.");
}


string* TextTester::generateText_(vector<string>& text_vector, unsigned int& length, unsigned int& capacity){
	if (capacity==0) {
    	capacity = rand()%SMALL_CAPACITY+1;
    	length = rand()%capacity;
  	}
    else if(length > capacity){
        length = rand()%capacity;
    }

	string* text = new string[capacity];
	text_vector.clear();
	for(int i =0; i < length; ++i){
		string w = "";
		generateWord_(w,rand() % WORD_LENGTH+1);
		text_vector.push_back(w);
		text[i] = w;
	}
	
	return text;
}

void TextTester::generateWord_(string& word, const unsigned int length) {
    static const char alphanum[] =
        "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

    do{
    	word = "";
    	for (int i = 0; i < length; ++i) 
        	word += alphanum[rand() % (sizeof(alphanum) - 1)];
    } while(word==DEL_WORD);
}

bool TextTester::checkText_(string* text, vector<string>& text_vector, unsigned int length){
	if((text == nullptr || length == 0) && text_vector.size()>0)
		return false;

    if(length < text_vector.size())
        return false;

	for(int i =0; i < text_vector.size(); ++i){
		if(text_vector[i]!=text[i])
			return false;			
	}
	
	return true;
}
void TextTester::errorOut_(const string& errMsg, unsigned int errBit) {
        cerr << funcname_ << " " << "(fail";
        if (errBit)
                cerr << errBit;
        cerr << "): " << errMsg << endl;
        error_|=(1<<errBit);
}

void TextTester::passOut_(const string& passMsg) {
        if (!error_) {
                cerr << funcname_ << " " << "(pass): " << passMsg << endl;
        }
}
