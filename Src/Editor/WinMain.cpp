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
#include "BaseClasses/GameObject.h"
#include "Component/Transform.h"
#include "Scene/SceneGraph.h"

using namespace codegym::editor;
using namespace codegym::runtime;

int WinMain(int argc, char **argv)
{
	ExportTest et;

	SceneGraph  sg;
	sg.Initlize();

	QApplication app (argc, argv);
	MainWindow mw;
	
	cMenuManager::Initlize(&mw);
	ExecuteStartups();
	cMenuManager::RebuildOsMenus();
	mw.SetupLayout();
	mw.InitContent(&sg);
	mw.showMaximized();

	return app.exec();
}
