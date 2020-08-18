#pragma once

#include "html_tokenizer.h"

#include<vector>
#include<unordered_map>
#include<string>
#include<unordered_set>

enum struct NodeType {
	TEXT, TERMINAL_TAG, NONTERMINAL_TAG, COMMENT, DOCTYPE, ROOT
};

struct DomNode {
	std::string name;
	NodeType type;
	std::unordered_map<std::string, std::string> properties;
	std::vector<DomNode> children;
	std::string child;
	void printString(int offset);
};

class DomGenerator
{
private:
	const std::unordered_set<std::string> singleTagSet = {
		"AREA", "LINK", "IMG", "PARAM", "HR", 
		"INPUT", "COL", "BASE", "META", "BASEFONT",
		"BR", "FRAME", "ISINDEX"
	};
	HtmlTokenizer* tk;
	DomNode document;
	DomNode generateTag();
	DomNode generateDoctype();
	DomNode generateText();
	DomNode generateComment();
public:
	DomGenerator(HtmlTokenizer *tokenizer);
	void generate();
	DomNode getDom();
};
