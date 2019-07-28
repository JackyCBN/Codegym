#include <QApplication>
#include <QMainWindow>
#include <QLabel>
#include <rttr/type>
#include <rttr/registration>
#include <iostream>
#include "Runtime/ExportTest.h"
#include "Window/MainWindow.h"
#include "EditorHelper.h"
#include "Menu/MenuManager.h"

using namespace codegym::editor;

int WinMain(int argc, char **argv)
{
	ExportTest et;


	QApplication app (argc, argv);
	MainWindow mw;
	mw.InitLayout();
	cMenuManager::Initlize(&mw);
	ExecuteStartups();
	cMenuManager::RebuildOsMenus();

	mw.showMaximized();


	//QMainWindow mw;

	//QLabel* label = new QLabel(&mw);
	//label->setText(std::to_string(et.foo()).c_str());
	//mw.show();
	
	return app.exec();
	//std::cout << "123123212" << std::endl;
	//return 0;
}
