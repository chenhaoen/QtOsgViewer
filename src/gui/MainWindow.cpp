#include "MainWindow.h"

#include <QFileDialog>
#include <QSettings>

#include <osgDB/ReadFile>

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

void MainWindow::on_action_Open_triggered()
{
	QSettings settings;
	const QString openFileLastDir("MainWindow/OpenFileLastDir");
	const QString& lastDir = settings.value(openFileLastDir).toString();
	const QString& openFileName = QFileDialog::getOpenFileName(this, tr("Open file"), lastDir, QStringLiteral("*"));

	if (openFileName.isEmpty())
	{
		return;
	}

	settings.setValue(openFileLastDir, openFileName);

	osg::ref_ptr<osg::Node> node = osgDB::readNodeFile(openFileName.toStdString());

	if (!node.valid())
	{
		return;
	}

	m_OsgWidget->insertNode(QString::fromStdString(node->getName()), node);
}

void MainWindow::on_action_Quit_triggered()
{
	close();
}
