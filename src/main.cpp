#include <QApplication>

#include "GuiInterface.h"

int main(int argc, char** argv)
{
	QApplication application(argc,argv);

	GuiInterface guiInterface;
	guiInterface.showMainWindow();

	return application.exec();
}