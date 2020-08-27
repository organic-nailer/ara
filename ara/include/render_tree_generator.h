#pragma once

#include "dom_generator.h"

#include<vector>
#include<unordered_map>
#include<string>
#include<unordered_set>

enum struct RenderNodeType {
	TEXT, TERMINAL_TAG, NONTERMINAL_TAG, ROOT
};

struct RenderNode {
	std::string name;
	RenderNodeType type;
	std::unordered_map<std::string, std::string> styles;
	std::vector<RenderNode> children;
	std::string child;
	void printString(int offset);
};

class RenderTreeGenerator {
private:
	RenderNode renderTree;
	RenderNode generateTag(DomNode dom);
	RenderNode generateText(DomNode dom);
public:
	RenderTreeGenerator();
	void generate(DomNode dom);
	RenderNode getTree();
};
