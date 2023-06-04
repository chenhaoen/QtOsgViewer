#include "MainWindow.h"

#include "ui_MainWindow.h"

#include "OsgWidget.h"

MainWindow::MainWindow(QWidget* parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindowClass())
	, m_OsgWidget(new OsgWidget())
{
	m_ui->setupUi(this);

	setCentralWidget(m_OsgWidget);
}

void MainWindow::on_action_Open_triggered(bool value)
{
}

void MainWindow::on_action_Quit_triggered(bool value)
{
	close();
}
