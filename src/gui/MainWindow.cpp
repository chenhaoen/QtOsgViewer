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