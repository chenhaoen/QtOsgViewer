#pragma once

#include <memory>

#include "GuiExports.h"

class MainWindow;

class  GUI_EXPORT GuiInterface
{
public:
	GuiInterface();

	void showMainWindow();

private:
	std::shared_ptr<MainWindow> m_mainWindow;
};