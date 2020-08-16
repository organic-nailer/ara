#include "html_tokenizer.h"
#include "util.h"

#include<sstream>
#include<vector>

HtmlTokenizer::HtmlTokenizer(std::ifstream* stream) {
	input = stream;
	advance(true);
}

bool HtmlTokenizer::hasMoreTokens() {
	return !finished;
}

void HtmlTokenizer::advance(bool init) {
	using std::string;
	using fastUtil::safeGetline;
	if (finished) throw 0;
	if (!nextTokens.empty()) {
		currentToken = nextTokens.front();
		nextTokens.pop_front();
	}

	while (nextTokens.empty()) {
		string bufLine;
		if (!safeGetline(*input, bufLine)) {
			finished = true;
			break;
		}

		int start = 0;
		for (int i = 0; i < bufLine.length(); i++) {
			if (bufLine[i] == '<' && !inTag) {
				if (i - start >= 1) {
					nextTokens.push_back({
						HtmlTokenType::CONTENT,
						bufLine.substr(start, i - start)
					});
				}
				nextTokens.push_back({
					HtmlTokenType::SYMBOL,
					"<"
				});
				start = i + 1;
				inTag = true;
			}
			else if (bufLine[i] == '>' && inTag) {
				inTag = false;
				auto tagInfo = bufLine.substr(start, i - start);
				start = i + 1;
				std::vector<string> lines;
				bool inSingleQuote = false;
				bool inDoubleQuote = false;
				int quoteStart = 0;
				for (int j = 0; j < tagInfo.length(); j++) {
					if (tagInfo[j] == '\"') {
						if (inSingleQuote) continue;
						if (inDoubleQuote) {
							lines.push_back(tagInfo.substr(quoteStart, j - quoteStart + 1));
							quoteStart = j + 1;
							inDoubleQuote = false;
						}
						else {
							lines.push_back(tagInfo.substr(quoteStart, j - quoteStart));
							quoteStart = j;
							inDoubleQuote = true;
						}
					}
					else if (tagInfo[j] == '\'') {
						if (inDoubleQuote) continue;
						if (inSingleQuote) {
							lines.push_back(tagInfo.substr(quoteStart, j - quoteStart + 1));
							quoteStart = j + 1;
							inSingleQuote = false;
						}
						else {
							lines.push_back(tagInfo.substr(quoteStart, j - quoteStart));
							quoteStart = j;
							inSingleQuote = true;
						}
					}
				}
				if (quoteStart < tagInfo.length()) {
					lines.push_back(tagInfo.substr(quoteStart));
				}
				for (string line : lines) {
					if (line.empty()) continue;
					if (line[0] == '\"' || line[0] == '\'') {
						nextTokens.push_back({
							HtmlTokenType::ATTR_VALUE,
							line.substr(1, line.length() - 2)
						});
						continue;
					}
					std::istringstream iss(line);
					string s;
					while (iss >> s) {
						int atomStart = 0;
						for (int k = 0; k < s.length(); k++) {
							if (s[k] == '/' || s[k] == '!' || s[k] == '=') {
								if (k > atomStart) {
									nextTokens.push_back({
										HtmlTokenType::TAG_IDENTIFY,
										s.substr(atomStart, k - atomStart)
									});
								}
								nextTokens.push_back({
									HtmlTokenType::SYMBOL,
									string(1, s[k])
								});
								atomStart = k + 1;
							}
							else if (s[k] == '-' && k + 1 < s.length() && s[k + 1] == '-') {
								if (k > atomStart) {
									nextTokens.push_back({
										HtmlTokenType::TAG_IDENTIFY,
										s.substr(atomStart, k - atomStart)
									});
								}
								nextTokens.push_back({
									HtmlTokenType::SYMBOL,
									s.substr(k,2)
								});
								atomStart = k + 2;
								k++;
							}
						}
						if (atomStart < s.length()) {
							nextTokens.push_back({
								HtmlTokenType::TAG_IDENTIFY,
								s.substr(atomStart)
							});
						}
					}
				}
				nextTokens.push_back({
					HtmlTokenType::SYMBOL,
					">"
				});
			}
		}
	}
	if (init) return;

	_type = currentToken.type;
	_tokenValue = currentToken.data;
}

HtmlTokenType HtmlTokenizer::type() {
	return _type;
}

std::string HtmlTokenizer::tokenValue() {
	return _tokenValue;
}
