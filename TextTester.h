#ifndef TEXTTESTER_H_
#define TEXTTESTER_H_

#include <string>
#include <vector>

class TextTester {
public:
	TextTester();
	~TextTester();

	void testResize();
	void testReadText();
	void testAppendText();
	void testDeleteWordAll();

	
private:
	std::string* generateText_(std::vector<std::string>& text_vector, unsigned int& length, unsigned int& capacity);
	void generateWord_(std::string& word, unsigned int length);
	bool checkText_(std::string* text, std::vector<std::string>& text_vector, unsigned int length);

	void errorOut_(const std::string& errMsg, unsigned int errBit);
	void passOut_(const std::string& passMsg);

	char error_;
	std::string funcname_;

	static const unsigned int SMALL_LENGHT = 100;
	static const unsigned int SMALL_CAPACITY = 100;
	static const unsigned int WORD_LENGTH = 20;
	static const std::string DEL_WORD;
	
};

#endif /* TEXTTESTER_H_ */
