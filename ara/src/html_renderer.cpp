#include "html_renderer.h"

#include<iostream>
#include<sstream>

HtmlRenderer::HtmlRenderer(AraWindow* w, RenderNode t) {
	window = w;
	tree = t;
}

void HtmlRenderer::measure() {
	tree.styles["width"] = std::to_string(window->windowWidth);
	tree.styles["height"] = std::to_string(window->windowHeight);
	for (int i = 0; i < tree.children.size(); i++) {
		measureNode(&tree.children[i], window->windowWidth);
	}
}

int HtmlRenderer::measureNode(RenderNode* node, int pWidth) {
	if (node->type == RenderNodeType::TEXT) {
		node->styles["width"] = std::to_string(pWidth);
		node->styles["height"] = "50";
		return 50;
	}
	else if (node->type == RenderNodeType::TERMINAL_TAG) {
		node->styles["width"] = std::to_string(pWidth - 4);
		node->styles["height"] = "20";
		return 20;
	}
	else if (node->type == RenderNodeType::NONTERMINAL_TAG) {
		std::cout << "measure" << node->name << ":" << std::to_string(pWidth - 4) << std::endl;
		node->styles["width"] = std::to_string(pWidth - 4);
		int h = 0;
		for (int i = 0; i < node->children.size(); i++) {
			h += measureNode(&node->children[i], pWidth - 4);
		}
		node->styles["height"] = std::to_string(h);
		return h;
	}
	else {
		std::cout << "Renderer: measureNode error" << std::endl;
		throw 0;
	}
}

void HtmlRenderer::render() {
	window->clearWhite();
/*	window->drawRect(0, 10,
		100,
		200,
		SK_ColorBLUE
	);*/
	renderNode(tree, 0, 0);
	window->swapBuffers();
}

void HtmlRenderer::renderNode(RenderNode node, int sX, int sY) {
	int startX = sX;
	int startY = sY;
	int64_t color;
	if (node.styles.find("color") != node.styles.end()) {
		std::istringstream buf{ node.styles["color"] };
		buf >> std::hex >> color;
		std::cout << node.styles["color"] << "->" << color;
		//color = std::stoi(node.styles["color"]);
	}
	else {
		color = 0x00;
	}
	std::cout << "rect(" << sX << "," << sY;
	std::cout << "," << sX + std::stoi(node.styles["width"]);
	std::cout << "," << sY + std::stoi(node.styles["height"]) << ")" << color << std::endl;
	window->drawRect(sX, sY,
		sX + std::stoi(node.styles["width"]),
		sY + std::stoi(node.styles["height"]),
		color
	);
	if (node.type == RenderNodeType::TEXT ) {
		std::cout << "text: " << node.child << "(" << sX << "," << sY << ")" << std::endl;
		window->drawText(node.child.c_str(), sX, sY);
	}
	else {
		for (auto child : node.children) {
			renderNode(child, startX, startY);
			startY += std::stoi(child.styles["height"]);
		}
	}
}
