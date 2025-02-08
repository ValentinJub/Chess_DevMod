#include "CApp.h"

int main(int argc, char *argv[]) {
	Util::initLogger(argc, argv);
	CApp theApp(Util::showTitleScreen(argc, argv));
	return theApp.exec();
}

