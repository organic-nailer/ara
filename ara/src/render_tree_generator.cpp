#include "render_tree_generator.h"

#include<iostream>

void RenderNode::printString(int offset) {
	using std::string;
	using std::cout;
	using std::endl;

	string offStr = string(offset, ' ');

	string propStr = "";

	for (std::unordered_map<string, string>::const_iterator it = styles.begin();
		it != styles.end(); it++) {
		if (it->second.size() == 0) {
			propStr += " " + it->first;
		}
		else {
			propStr += " " + it->first + "=\"" + it->second + "\"";
		}
	}

	switch (type)
	{
	case RenderNodeType::TEXT:
		cout << offStr << "#text: " << child << endl;
		break;
	case RenderNodeType::NONTERMINAL_TAG:
		cout << offStr << name << propStr << endl;
		for (auto c = children.begin(); c != children.end(); c++) {
			c->printString(offset + 1);
		}
		break;
	case RenderNodeType::TERMINAL_TAG:
		cout << offStr << name << propStr << endl;
		break;
	case RenderNodeType::ROOT:
		cout << "render tree:" << endl;
		for (auto c = children.begin(); c != children.end(); c++) {
			c->printString(offset + 1);
		}
		break;
	}
}

RenderTreeGenerator::RenderTreeGenerator() {

}

RenderNode RenderTreeGenerator::getTree() {
	return renderTree;
}

void RenderTreeGenerator::generate(DomNode dom) {
	if (dom.type != NodeType::ROOT) {
		return;
	}

	renderTree = RenderNode();
	renderTree.name = "root";
	renderTree.type = RenderNodeType::ROOT;
	
	for (auto child : dom.children) {
		switch (child.type) {
			case NodeType::NONTERMINAL_TAG:
			case NodeType::TERMINAL_TAG:
				renderTree.children.push_back(generateTag(child));
				break;
			case NodeType::TEXT:
				renderTree.children.push_back(generateText(child));
				break;
		}
	}
}

RenderNode RenderTreeGenerator::generateTag(DomNode dom) {
	auto node = RenderNode();
	if (dom.type == NodeType::NONTERMINAL_TAG) {
		node.name = dom.name;
		node.type = RenderNodeType::NONTERMINAL_TAG;
		node.styles = dom.properties;
		for (auto child : dom.children) {
			switch (child.type) {
			case NodeType::NONTERMINAL_TAG:
			case NodeType::TERMINAL_TAG:
				node.children.push_back(generateTag(child));
				break;
			case NodeType::TEXT:
				node.children.push_back(generateText(child));
				break;
			}
		}
		return node;
	}
	else if (dom.type == NodeType::TERMINAL_TAG) {
		node.name = dom.name;
		node.type = RenderNodeType::TERMINAL_TAG;
		node.styles = dom.properties;
		return node;
	}
	else {
		std::cout << "RenderTreeGenerator: generateTag error" << std::endl;
		throw 0;
	}
}

RenderNode RenderTreeGenerator::generateText(DomNode dom) {
	if (dom.type != NodeType::TEXT) {
		std::cout << "RenderTreeGenerator: generateText error" << std::endl;
		throw 0;
	}
	auto node = RenderNode();
	node.name = dom.name;
	node.type = RenderNodeType::TEXT;
	node.styles = dom.properties;
	node.child = dom.child;
	return node;
}
