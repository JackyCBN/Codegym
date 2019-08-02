#include "MainWindow.h"
#include <QMenuBar>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QTextBrowser>

#include "Hierarchy/HierarchyWindow.h"
#include "Inspector/InspectorWindow.h"
#include "Hierarchy/HierarchyViewModel.h"
#include "BaseClasses/GameObject.h"
#include "Scene/SceneGraph.h"
#include "Component/Transform.h"

using namespace codegym::editor;
using namespace codegym::runtime;
MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	m_menubar = menuBar();
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

QMenuBar* MainWindow::GetMenubar() const
{
	return m_menubar;
}

void MainWindow::CreateHierarchy()
{
	auto m_explorerDock = new QDockWidget(tr("World Hierarchy"), this);
	m_explorerDock->setAllowedAreas(Qt::AllDockWidgetAreas);

	QGridLayout* mainLayout = new QGridLayout();
	mainLayout->setRowMinimumHeight(0, 10);
	mainLayout->setRowStretch(0, 0);
	mainLayout->setRowStretch(1, 1);
	mainLayout->setRowMinimumHeight(2, 31);
	mainLayout->setRowStretch(2, 0);
	mainLayout->setColumnMinimumWidth(0, 10);
	mainLayout->setColumnStretch(0, 0);
	mainLayout->setColumnMinimumWidth(1, 150);
	mainLayout->setColumnStretch(1, 1);



	m_explorerDock->setFocusPolicy(Qt::ClickFocus);
	m_explorerDock->installEventFilter(this);
	if (m_explorerDock->titleBarWidget())
		m_explorerDock->titleBarWidget()->installEventFilter(this);

	addDockWidget(Qt::LeftDockWidgetArea, m_explorerDock);

	m_hierarchyWindow = new HierarchyWindow(m_explorerDock);


	m_hierarchyWindow->viewport()->installEventFilter(this);
	m_hierarchyWindow->setObjectName("treeview");
	m_hierarchyWindow->setFocusPolicy(Qt::StrongFocus);
	m_hierarchyWindow->setContextMenuPolicy(Qt::CustomContextMenu);

	mainLayout->addWidget(m_hierarchyWindow, 1, 1);
	mainLayout->setContentsMargins(0, 0, 0, 0);

	//m_explorerDock->setWidget(styleWidget);
	QWidget* styleWidget = new QWidget();

	styleWidget->setLayout(mainLayout);
	m_explorerDock->setWidget(styleWidget);
}


void MainWindow::CreateProperties()
{
	auto m_propertiesDock = new QDockWidget(tr("Properties"), this);
	m_propertiesDock->setAllowedAreas(Qt::AllDockWidgetAreas);

	QWidget* styleWidget = new QWidget();

	QWidget* styleGroup = new QWidget();
	QVBoxLayout* listLayout = new QVBoxLayout();
	listLayout->setMargin(0);

	QListWidget* customerList = new QListWidget();

	listLayout->addWidget(customerList);
	styleGroup->setLayout(listLayout);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(styleGroup);
	styleWidget->setLayout(mainLayout);

	m_propertiesDock->setWidget(styleWidget);

	addDockWidget(Qt::RightDockWidgetArea, m_propertiesDock);

	m_inspectorWindow = new InspectorWindow(this);
	m_inspectorWindow->installEventFilter(this);
	m_inspectorWindow->setObjectName(tr("propertyBrowser"));
	m_propertiesDock->setWidget(m_inspectorWindow);

	m_inspectorWindow->setFocusPolicy(Qt::StrongFocus);
	m_inspectorWindow->installEventFilter(this);
	m_propertiesDock->setFocusPolicy(Qt::ClickFocus);
	m_propertiesDock->installEventFilter(this);
	if (m_propertiesDock->titleBarWidget())
		m_propertiesDock->titleBarWidget()->installEventFilter(this);

	//Drag and drop
	m_inspectorWindow->setAcceptDrops(true);
	connect(m_propertiesDock, SIGNAL(visibilityChanged(bool)), this, SLOT(propertiesViewShowChange(bool)));

#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
	QList<QDockWidget*> docks; docks << m_propertiesDock;
	resizeDocks(docks, { 100, 120 }, Qt::Vertical);
#endif
}

void MainWindow::CreateOutput()
{
	auto m_outPutDock = new QDockWidget(tr("Output"), this);
	m_outPutDock->setAllowedAreas(Qt::AllDockWidgetAreas);

	QWidget* styleWidget = new QWidget();

	QWidget* styleGroup = new QWidget();
	QVBoxLayout* listLayout = new QVBoxLayout();
	listLayout->setMargin(0);

	QListWidget* customerList = new QListWidget();

	listLayout->addWidget(customerList);
	styleGroup->setLayout(listLayout);

	QVBoxLayout* mainLayout = new QVBoxLayout;
	mainLayout->addWidget(styleGroup);
	styleWidget->setLayout(mainLayout);

	m_outPutDock->setWidget(styleWidget);
	addDockWidget(Qt::BottomDockWidgetArea, m_outPutDock);

	m_outputWindow = new QTextBrowser(m_outPutDock);
	m_outPutDock->setWidget(m_outputWindow);

	m_outputWindow->setFocusPolicy(Qt::ClickFocus);
	m_outputWindow->installEventFilter(this);
	m_outPutDock->setFocusPolicy(Qt::ClickFocus);
	m_outPutDock->installEventFilter(this);

	if (m_outPutDock->titleBarWidget())
		m_outPutDock->titleBarWidget()->installEventFilter(this);

	m_outputWindow->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(m_outputWindow, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(showOutPutMenu(const QPoint&)));

	connect(m_outPutDock, SIGNAL(visibilityChanged(bool)), this, SLOT(outputViewShowChange(bool)));

#if (QT_VERSION >= QT_VERSION_CHECK(5,6,0))
	QList<QDockWidget*> docks; docks << m_outPutDock;
	resizeDocks(docks, { 100, 120 }, Qt::Vertical);
#endif
}

void MainWindow::CreateItemRecusive(Transform* node, QStandardItem* Item)
{
	for(int i=0; i!= node->ChildCount(); ++i)
	{
		auto childTrans = node->GetChild(i);

		QList<QStandardItem*> itemslst;
		QStandardItem* childItem = new QStandardItem(childTrans->GetGameObject()->Name());
		itemslst.append(childItem);
		Item->appendRow(itemslst);

		CreateItemRecusive(childTrans, childItem);
	}
}

void MainWindow::InitHierarchyContent(SceneGraph* sg)
{
	HierarchyViewModel* model = new HierarchyViewModel(m_hierarchyWindow);

	QList<QStandardItem*> itemslst;
	GameObject* node = sg->Root();
	QStandardItem* rootItem = new QStandardItem(node->Name());
	itemslst.append(rootItem);
	model->appendRow(itemslst);

	CreateItemRecusive(node->QueryComponent<Transform>(), rootItem);


	//model->setHorizontalHeaderLabels(QStringList() << QStringLiteral("序号") << QStringLiteral("名称"));     //设置列头
	//for (int i = 0; i < 5; i++)
	//{
	//	//一级节点，加入mModel
	//	QList<QStandardItem*> items1;
	//	QStandardItem* item1 = new QStandardItem(QString::number(i));
	//	QStandardItem* item2 = new QStandardItem(QStringLiteral("一级节点"));
	//	items1.append(item1);
	//	items1.append(item2);
	//	model->appendRow(items1);

	//	for (int j = 0; j < 5; j++)
	//	{
	//		//二级节点,加入第1个一级节点
	//		QList<QStandardItem*> items2;
	//		QStandardItem* item3 = new QStandardItem(QString::number(j));
	//		QStandardItem* item4 = new QStandardItem(QStringLiteral("二级节点"));
	//		items2.append(item3);
	//		items2.append(item4);
	//		item1->appendRow(items2);

	//		for (int k = 0; k < 5; k++)
	//		{
	//			//三级节点,加入第1个二级节点
	//			QList<QStandardItem*> items3;
	//			QStandardItem* item5 = new QStandardItem(QString::number(k));
	//			QStandardItem* item6 = new QStandardItem(QStringLiteral("三级节点"));
	//			items3.append(item5);
	//			items3.append(item6);
	//			item3->appendRow(items3);
	//		}
	//	}
	//}
	//2，给QTreeView应用model
	m_hierarchyWindow->setModel(model);
}
