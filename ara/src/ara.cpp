// ara.cpp : Defines the entry point for the application.
//

#include "ara.h"
#include "html_tokenizer.h"
#include "dom_generator.h"
#include "ara_window.h"

using namespace std;

const int kWidth = 960;
const int kHeight = 640;

int main(int argc, char *argv[])
{
	AraWindow window(kWidth, kHeight);

	while (!window.shouldClose()) {
		window.pollEvents();
		window.draw();
		window.swapBuffers();
	}
	return 0;
}
