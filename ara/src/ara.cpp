// ara.cpp : Defines the entry point for the application.
//

#include "ara.h"
#include "html_tokenizer.h"
#include "dom_generator.h"
#include "ara_window.h"
#include "render_tree_generator.h"
#include "html_renderer.h"

using namespace std;

const int kWidth = 960;
const int kHeight = 640;

int main(int argc, char *argv[])
{
	cout << "Hello CMake." << endl;
	if (argc < 2) {
		cout << "too short arguments" << endl;
		//return -1;
	}
	string path = "D:\\Projects\\ara\\ara\\test\\hoge.html"; //argv[1];

	auto input = ifstream(path);
	auto tokenizer = HtmlTokenizer(&input);
	auto generator = DomGenerator(&tokenizer);
	generator.generate();
	auto dom = generator.getDom();

	dom.printString(0);

	auto renderTreeGenerator = RenderTreeGenerator();
	renderTreeGenerator.generate(dom);
	auto tree = renderTreeGenerator.getTree();

	tree.printString(0);

	AraWindow window(kWidth, kHeight);

	auto renderer = HtmlRenderer(&window, tree);
	renderer.measure();
	//renderer.render();

	while (!window.shouldClose()) {
		window.pollEvents();
		renderer.render();
	}
	return 0;
}
