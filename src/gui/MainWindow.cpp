#include "MainWindow.h"

#include "ui_MainWindow.h"

#include "moc_MainWindow.cpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindowClass())
{
	m_ui->setupUi(this);

	
}

void MainWindow::on_action_Open_triggered(bool value)
{
}

void MainWindow::on_action_Quit_triggered(bool value)
{
	close();
}
