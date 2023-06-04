#pragma once

#include <memory>

#include "GuiExports.h"

class MainWindow;

class  GUI_EXPORT GuiInterface
{
public:
	GuiInterface();
	~GuiInterface() = default;

	void showMainWindow();

private:
	std::shared_ptr<MainWindow> m_mainWindow;
};