#include "MainWindow.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	m_menubar = menuBar();
	//if(m_menubar)
	//{
	//	QMenu* fileMenu = new QMenu(tr("File(&F)"), m_menubar);
	//	m_menubar->addMenu(fileMenu);
	//	//fileMenu->addAction("close");


	//}


}

QMenuBar* MainWindow::GetMenubar() const
{
	return m_menubar;
}

void MainWindow::clickMenuItem()
{
	int a = 1;

	int b = 2;
}
