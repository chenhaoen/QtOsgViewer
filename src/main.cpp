#include <QApplication>

#include "GuiInterface.h"

int main(int argc, char** argv)
{
	QApplication application(argc, argv);

	QCoreApplication::setOrganizationName("chenhaoen");
	QCoreApplication::setOrganizationDomain("");
	QCoreApplication::setApplicationName("QtOsgViewer");

	GuiInterface guiInterface;
	guiInterface.showMainWindow();

	return application.exec();
}