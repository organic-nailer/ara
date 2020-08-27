#include "dom_generator.h"

#include<iostream>
#include<algorithm>

void DomNode::printString(int offset) {
	using std::string;
	using std::cout;
	using std::endl;

	string offStr = string(offset, ' ');

	string propStr = "";

	for (std::unordered_map<string, string>::const_iterator it = properties.begin();
		it != properties.end(); it++) {
		if (it->second.size() == 0) {
			propStr += " " + it->first;
		}
		else {
			propStr += " " + it->first + "=\"" + it->second + "\"";
		}
	}

	switch (type)
	{
	case NodeType::TEXT:
		cout << offStr << "#text: " << child << endl;
		break;
	case NodeType::NONTERMINAL_TAG:
		cout << offStr << name << propStr << endl;
		for (auto c = children.begin(); c != children.end(); c++) {
			c->printString(offset + 1);
		}
		break;
	case NodeType::TERMINAL_TAG:
		cout << offStr << name << propStr << endl;
		break;
	case NodeType::COMMENT:
		cout << offStr << "#comment: " << child << endl;
		break;
	case NodeType::DOCTYPE:
		cout << offStr << "DOCTYPE: " << propStr << endl;
		break;
	case NodeType::ROOT:
		cout << "document:" << endl;
		for (auto c = children.begin(); c != children.end(); c++) {
			c->printString(offset + 1);
		}
		break;
	}
}

DomGenerator::DomGenerator(HtmlTokenizer* tokenizer) {
	tk = tokenizer;
}

void DomGenerator::generate() {
	document = DomNode();
	document.name = "document";
	document.type = NodeType::ROOT;

	while (tk->hasMoreTokens()) {
		tk->advance();
		if (tk->type() == HtmlTokenType::CONTENT) {
			document.children.push_back(generateText());
		}
		else if (tk->hasMoreTokens()) {
			tk->advance();
			if (tk->type() == HtmlTokenType::TAG_IDENTIFY) {
				tk->restore();
				document.children.push_back(generateTag());
			}
			else if (tk->tokenValue() == "!" && tk->hasMoreTokens()) {
				tk->advance();
				if (tk->tokenValue() == "--") {
					tk->restore();
					tk->restore();
					document.children.push_back(generateComment());
				}
				else {
					tk->restore();
					tk->restore();
					document.children.push_back(generateDoctype());
				}
			}
			else {
				std::cout << "domgenerator::generate error" << std::endl;
				throw 1;
			}
		}
		else {
			std::cout << "domgenerator::generate error" << std::endl;
			throw 2;
		}
	}
}

DomNode DomGenerator::generateTag() {
	auto node = DomNode();
	if (tk->type() != HtmlTokenType::SYMBOL
		|| tk->tokenValue() != "<") {
		std::cout << "domgenerator::generateTag error" << std::endl;
		throw 1;
	}
	std::string tagName;
	if (tk->hasMoreTokens()) {
		tk->advance();
		if (tk->type() == HtmlTokenType::TAG_IDENTIFY) {
			tagName = tk->tokenValue();
		}
		else {
			std::cout << "domgenerator::generateTag error" << std::endl;
			throw 2;
		}
	}
	else {
		std::cout << "domgenerator::generateTag error" << std::endl;
		throw 3;
	}

	bool isContentEmpty = false;
	std::transform(tagName.cbegin(), tagName.cend(), tagName.begin(), toupper);
	if (singleTagSet.find(tagName) != singleTagSet.end()) {
		isContentEmpty = true;
	}
	node.name = tagName;
	if (isContentEmpty) {
		node.type = NodeType::TERMINAL_TAG;
	}
	else {
		node.type = NodeType::NONTERMINAL_TAG;
	}

	while (tk->hasMoreTokens()) {
		tk->advance();
		if (tk->type() == HtmlTokenType::SYMBOL
			&& tk->tokenValue() == ">") {
			break;
		}
		if (tk->type() == HtmlTokenType::TAG_IDENTIFY) {
			auto attrName = tk->tokenValue();
			if (tk->hasMoreTokens()) {
				tk->advance();
				if (tk->type() == HtmlTokenType::SYMBOL
					&& tk->tokenValue() == "=") {
					if (tk->hasMoreTokens()) {
						tk->advance();
						if (tk->type() == HtmlTokenType::ATTR_VALUE) {
							auto attrValue = tk->tokenValue();
							node.properties[attrName] = attrValue;
						}
						else {
							std::cout << "domgenerator::generateTag error" << std::endl;
							throw 4;
						}
					}
					else {
						std::cout << "domgenerator::generateTag error" << std::endl;
						throw 5;
					}
				}
				else {
					node.properties[attrName] = "";
					tk->restore();
				}
			}
			else {
				std::cout << "domgenerator::generateTag error" << std::endl;
				throw 6;
			}
		}
		else {
			std::cout << "domgenerator::generateTag error" << std::endl;
			throw 7;
		}
	}

	if (isContentEmpty) return node;

	while (tk->hasMoreTokens()) {
		tk->advance();
		if (tk->type() == HtmlTokenType::SYMBOL
			&& tk->tokenValue() == "<") {
			tk->advance();
			if (tk->type() == HtmlTokenType::SYMBOL
				&& tk->tokenValue() == "/") {
				tk->advance();
				if (tk->type() == HtmlTokenType::TAG_IDENTIFY) {
					std::string endTag = tk->tokenValue();
					std::transform(endTag.cbegin(), endTag.cend(), endTag.begin(), toupper);
					if (endTag == tagName) {
						tk->advance();
						if (tk->type() == HtmlTokenType::SYMBOL
							&& tk->tokenValue() == ">") {
							break;
						}
						else {
							std::cout << "domgenerator::generateTag error" << std::endl;
							throw 8;
						}
					}
					else {
						std::cout << "domgenerator::generateTag error" << std::endl;
						throw 9;
					}
				}
				else {
					std::cout << "domgenerator::generateTag error" << std::endl;
					throw 10;
				}
			}
			else if (tk->type() == HtmlTokenType::TAG_IDENTIFY) {
				tk->restore();
				node.children.push_back(generateTag());
			}
			else if (tk->type() == HtmlTokenType::SYMBOL
				&& tk->tokenValue() == "!") {
				tk->advance();
				if (tk->type() == HtmlTokenType::SYMBOL
					&& tk->tokenValue() == "--") {
					tk->restore();
					tk->restore();
					node.children.push_back(generateComment());
				}
				else {
					std::cout << "domgenerator::generateTag error" << std::endl;
					throw 12;
				}
			}
			else {
				std::cout << "domgenerator::generateTag error" << std::endl;
				throw 11;
			}
		}
		else if (tk->type() == HtmlTokenType::CONTENT) {
			node.children.push_back(generateText());
		}
		else {
			std::cout << "domgenerator::generateTag error" << std::endl;
			throw 13;
		}
	}

	return node;
}

DomNode DomGenerator::generateDoctype() {
	auto node = DomNode();
	if (tk->type() != HtmlTokenType::SYMBOL
		|| tk->tokenValue() != "<") {
		std::cout << "domgenerator::generateDoctype error" << std::endl;
		throw 1;
	}
	node.name = "DOCTYPE";
	node.type = NodeType::DOCTYPE;
	while (tk->hasMoreTokens()) {
		tk->advance();
		if (tk->type() == HtmlTokenType::SYMBOL
			&& tk->tokenValue() == ">") {
			break;
		}
		if (tk->type() == HtmlTokenType::TAG_IDENTIFY) {
			node.properties[tk->tokenValue()] = "";
		}
	}
	return node;
}

DomNode DomGenerator::generateText() {
	auto node = DomNode();
	if (tk->type() != HtmlTokenType::CONTENT) {
		std::cout << "domgenerator::generateText error" << std::endl;
		throw 1;
	}
	node.name = "TEXT";
	node.type = NodeType::TEXT;
	node.child = tk->tokenValue();
	while (tk->hasMoreTokens()) {
		tk->advance();
		if (tk->type() != HtmlTokenType::CONTENT) {
			tk->restore();
			break;
		}
		else {
			node.child += " " + tk->tokenValue();
		}
	}
	return node;
}

DomNode DomGenerator::generateComment() {
	auto node = DomNode();
	tk->advance();
	tk->advance();
	tk->advance();
	if (tk->type() != HtmlTokenType::COMMENT_CONTENT) {
		std::cout << "domgenerator::generateComment error" << std::endl;
		throw 1;
	}
	node.name = "COMMENT";
	node.type = NodeType::COMMENT;
	node.child = tk->tokenValue();
	while (tk->hasMoreTokens()) {
		tk->advance();
		if (tk->type() != HtmlTokenType::COMMENT_CONTENT) {
			tk->advance();
			break;
		}
		else {
			node.child += " " + tk->tokenValue();
		}
	}
	return node;
}

DomNode DomGenerator::getDom() {
	return document;
}
