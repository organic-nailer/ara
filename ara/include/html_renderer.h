#pragma once

#include "render_tree_generator.h"
#include "ara_window.h"

class HtmlRenderer {
private:
	AraWindow* window;
	RenderNode tree;
	int measureNode(RenderNode* node, int pWidth); //高さを返す
	void renderNode(RenderNode node, int sX, int sY);
public:
	HtmlRenderer(AraWindow* w, RenderNode t);
	void measure();
	void render();
};
