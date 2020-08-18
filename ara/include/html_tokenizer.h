#pragma once

#include<fstream>
#include<deque>
#include<string>

enum struct HtmlTokenType {
	SYMBOL, TAG_IDENTIFY, ATTR_VALUE, CONTENT, COMMENT_CONTENT
};

struct HtmlToken {
	HtmlTokenType type;
	std::string data;
};

// < > / ! -- > =

class HtmlTokenizer {
private:
	std::ifstream* input;
	std::deque<HtmlToken> nextTokens;
	std::deque<HtmlToken> prevTokens;
	bool finished = false;
	HtmlToken currentToken;
	bool inTag = false;
	bool inComment = false;
	HtmlTokenType _type;
	std::string _tokenValue;
	HtmlTokenType _prevType;
	std::string _prevTokenValue;
	void tokenizeInsideTag(std::string tagInfo);
public:
	HtmlTokenizer(std::ifstream *stream);
	bool hasMoreTokens();
	void advance(bool init = false);
	void restore();
	HtmlTokenType type();
	std::string tokenValue();
};
