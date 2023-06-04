#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class OsgWidget;

class  MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget* parent = nullptr);

private slots:
	void on_action_Open_triggered();
	void on_action_Quit_triggered();

private:
	std::shared_ptr<Ui::MainWindowClass> m_ui;

	OsgWidget* m_OsgWidget;
};
