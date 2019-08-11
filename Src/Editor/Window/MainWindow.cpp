#include "MainWindow.h"
#include <QMenuBar>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QTextBrowser>
#include <QTimer>

#include "Hierarchy/HierarchyWindow.h"
#include "Inspector/InspectorWindow.h"
#include "Hierarchy/HierarchyViewModel.h"
#include "BaseClasses/GameObject.h"
#include "Scene/SceneGraph.h"
#include "Layout/LayoutUtility.h"


using namespace codegym::editor;
using namespace codegym::runtime;

GameObject* ggg = nullptr;
MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	m_menubar = menuBar();

	auto timer = new QTimer(parent);
	connect(timer, &QTimer::timeout, this, &MainWindow::tick);
	timer->start();
}

void MainWindow::SetupLayout()
{
	CreateProperties();
	CreateHierarchy();
	CreateOutput();
	auto widget = new QWidget(this);
	this->setCentralWidget(widget);

}

void MainWindow::InitContent(SceneGraph* sg)
{
	InitHierarchyContent(sg);
}

void MainWindow::treeViewClickProgress(const QModelIndex& index)
{
	HierarchyViewModel* m_model = (HierarchyViewModel*)index.model();
	QStandardItem* pItem = m_model->itemFromIndex(index);

	GameObject* go = static_cast<GameObject*>(pItem->data().value<void*>());

	if (go)
	{
		m_inspectorWindow->onItemSelectChange(go);
	}
}

void MainWindow::tick()
{
	if(ggg && m_inspectorWindow)
	{
		//m_inspectorWindow->onItemSelectChange(ggg);
	}
}


QMenuBar* MainWindow::GetMenubar() const
{
	return m_menubar;
}

void MainWindow::CreateHierarchy()
{
	m_hierarchyWindow = new HierarchyWindow();
	m_hierarchyWindow->initlize();

	connect(m_hierarchyWindow, SIGNAL(clicked(const QModelIndex)),
		this, SLOT(treeViewClickProgress(const QModelIndex)));

	auto hierarchyDock = LayoutUtility::CreateHierarchyDock(this, m_hierarchyWindow);
	addDockWidget(Qt::LeftDockWidgetArea, hierarchyDock);
}


void MainWindow::CreateProperties()
{
	m_inspectorWindow = new InspectorWindow();
	m_inspectorWindow->initlize();

	auto dockwidget = LayoutUtility::CreatePropertiesDock(this, m_inspectorWindow);
	addDockWidget(Qt::RightDockWidgetArea, dockwidget);
}

void MainWindow::CreateOutput()
{
	m_outputWindow = new QTextBrowser();
	m_outputWindow->setFocusPolicy(Qt::ClickFocus);
	m_outputWindow->installEventFilter(this);
	m_outputWindow->setContextMenuPolicy(Qt::CustomContextMenu);

	auto outputDock= LayoutUtility::CreateOutputDock(this, m_outputWindow);
	addDockWidget(Qt::BottomDockWidgetArea, outputDock);
}


void MainWindow::InitHierarchyContent(SceneGraph* sg)
{
	m_hierarchyWindow->initHierarchyContent(sg);
}
