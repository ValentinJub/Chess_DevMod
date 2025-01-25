#include "CApp.h"

int main(int argc, char *argv[]) {
	std::cout << "Hello World!" << std::endl;
	CApp theApp(Util::showTitleScreen(argc, argv));
	return theApp.exec();
}

