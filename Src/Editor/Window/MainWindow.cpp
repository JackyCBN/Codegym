#include "MainWindow.h"
#include <QMenuBar>

MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	QMenuBar* mb = menuBar();
	if(mb)
	{
		QMenu* fileMenu = new QMenu(tr("File(&F)"), mb);
		mb->addMenu(fileMenu);
		fileMenu->addAction("close");


	}
}
