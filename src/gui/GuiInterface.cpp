#include "GuiInterface.h"

#include "MainWindow.h"

GuiInterface::GuiInterface()
	:m_mainWindow(std::make_shared<MainWindow>())
{
}

void GuiInterface::showMainWindow()
{
	m_mainWindow->show();
}
