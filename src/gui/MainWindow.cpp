#include "MainWindow.h"

#include "ui_MainWindow.h"

#include "moc_MainWindow.cpp"

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent)
	, m_ui(new Ui::MainWindowClass())
{
	m_ui->setupUi(this);
}