#include "MainWindow.h"
#include <QMenuBar>
#include <QHBoxLayout>
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include <QLabel>
#include <QTableView>
#include <QLineEdit>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QTextBrowser>

MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	m_menubar = menuBar();


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

	HierarchyWindow* hierarchy = new HierarchyWindow(m_explorerDock);


	hierarchy->viewport()->installEventFilter(this);
	hierarchy->setObjectName("treeview");
	hierarchy->setFocusPolicy(Qt::StrongFocus);
	hierarchy->setContextMenuPolicy(Qt::CustomContextMenu);

	mainLayout->addWidget(hierarchy, 1, 1);
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

	auto m_propertiesBrowser = new InspectorWindow(this);
	m_propertiesBrowser->installEventFilter(this);
	m_propertiesBrowser->setObjectName(tr("propertyBrowser"));
	m_propertiesDock->setWidget(m_propertiesBrowser);

	m_propertiesBrowser->setFocusPolicy(Qt::StrongFocus);
	m_propertiesBrowser->installEventFilter(this);
	m_propertiesDock->setFocusPolicy(Qt::ClickFocus);
	m_propertiesDock->installEventFilter(this);
	if (m_propertiesDock->titleBarWidget())
		m_propertiesDock->titleBarWidget()->installEventFilter(this);

	//Drag and drop
	m_propertiesBrowser->setAcceptDrops(true);
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

	auto m_outputWindow = new QTextBrowser(m_outPutDock);
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
void MainWindow::InitLayout()
{
	CreateProperties();
	CreateHierarchy();
	CreateOutput();
	auto widget = new QWidget(this);
	this->setCentralWidget(widget);

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
