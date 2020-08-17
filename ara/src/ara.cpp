// ara.cpp : Defines the entry point for the application.
//

#include "ara.h"
#include "html_tokenizer.h"

using namespace std;

int main(int argc, char *argv[])
{
	cout << "Hello CMake." << endl;

	if (argc < 2) {
		cout << "too short arguments" << endl;
		//return -1;
	}
	string path = "C:\\Users\\hykwy\\Downloads\\abehiroshi.html"; //argv[1];

	auto input = ifstream(path);
	auto tokenizer = HtmlTokenizer(&input);

	while (tokenizer.hasMoreTokens()) {
		tokenizer.advance();
		cout << "type=" << static_cast<int>(tokenizer.type()) << ", value=\"" << tokenizer.tokenValue() << "\"" << endl;
	}

	return 0;
}
