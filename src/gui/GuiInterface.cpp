#include "GuiInterface.h"

#include <QApplication>
#include <QPalette>
#include <QStyleFactory>

#include "MainWindow.h"

GuiInterface::GuiInterface()
{
}

int GuiInterface::runApplication(int argc, char** argv)
{
    QApplication application(argc, argv);

    QCoreApplication::setOrganizationName("chenhaoen");
    QCoreApplication::setOrganizationDomain("");
    QCoreApplication::setApplicationName("QtOsgViewer");

    qApp->setStyle(QStyleFactory::create("Fusion"));
    QPalette p = qApp->palette();
    p.setColor(QPalette::Base, QColor(80, 80, 80));
    p.setColor(QPalette::Window, QColor(37, 37, 37));
    p.setColor(QPalette::Button, QColor(73, 73, 73));
    p.setColor(QPalette::Text, Qt::white);
    p.setColor(QPalette::ButtonText, Qt::white);
    p.setColor(QPalette::WindowText, Qt::white);
    p.setColor(QPalette::HighlightedText, Qt::white);
    const QColor linkColor(115, 131, 191);
    p.setColor(QPalette::Link, linkColor);
    p.setColor(QPalette::LinkVisited, linkColor);

    const QColor disabledGray(40, 40, 40);
    const QColor disabledTextGray(128, 128, 128);
    p.setColor(QPalette::Disabled, QPalette::Window, disabledGray);
    p.setColor(QPalette::Disabled, QPalette::Base, disabledGray);
    p.setColor(QPalette::Disabled, QPalette::AlternateBase, disabledGray);
    p.setColor(QPalette::Disabled, QPalette::Button, disabledGray);
    p.setColor(QPalette::Disabled, QPalette::Text, disabledTextGray);
    p.setColor(QPalette::Disabled, QPalette::ButtonText, disabledTextGray);
    p.setColor(QPalette::Disabled, QPalette::WindowText, disabledTextGray);
    qApp->setPalette(p);

    const QString css =
        R"(
                QFrame[frameShape="5"] {
                    color: gray;
                    margin-top: 2px;
                    margin-bottom: 2px;
                }
                QAbstractItemView {
                    show-decoration-selected: 1;
                    background: #252525;
                    selection-background-color: #505050;
                }
                QAbstractItemView::item:hover { background: #383838; }
                QMenu {
                    background: #252525;
                    border: 1px solid rgb(100,100,100);
                }
                QMenu::item:selected { background: rgb(110,110,110); }
                QMenu::separator {
                    background: rgb(110,110,110);
                    height: 1px;
                }
                QLineEdit { background: #505050; }
                QTextEdit { background: #505050; }
                QSpinBox  { background: #505050; }
                QDoubleSpinBox { background: #505050; }
                QToolButton:checked { background: #383838; }
                QToolButton:pressed { background: #383838; }
                QComboBox { background: #505050; }
                QGroupBox {
                    border: 1px solid #808080;
                    margin-top: 4ex;
                }
                QFileDialog { background: #505050; }
                QComboBox:editable { background: #505050; }
                QComboBox:disabled { background: rgb(40,40,40); }
                QProgressBar { background: #505050; }
                )";
    qApp->setStyleSheet(css);

    m_mainWindow = std::make_shared<MainWindow>();
    m_mainWindow->showMaximized();
   return application.exec();

}
