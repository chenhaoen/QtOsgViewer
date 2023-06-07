#pragma once

#include <memory>

#include "GuiExports.h"

class MainWindow;

class  GUI_EXPORT GuiInterface
{
public:
	GuiInterface();

	int runApplication(int argc, char** argv);
private:
	std::shared_ptr<MainWindow> m_mainWindow;
};