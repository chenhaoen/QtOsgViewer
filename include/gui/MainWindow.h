#pragma once

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindowClass; };
QT_END_NAMESPACE

class  MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = nullptr);
	~MainWindow() = default;

private slots:
	void on_action_Open_triggered(bool value);
	void on_action_Quit_triggered(bool value);

private:
	std::shared_ptr<Ui::MainWindowClass> m_ui;
};
