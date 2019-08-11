#include "LayoutUtility.h"
#include <QGridLayout>
#include <QDockWidget>
#include <QListWidget>
using namespace codegym::editor;

QDockWidget* LayoutUtility::CreateHDock(QWidget* parent, QString name)
{
	auto dock = new QDockWidget(name, parent);
	dock->setAllowedAreas(Qt::AllDockWidgetAreas);
	dock->setFocusPolicy(Qt::ClickFocus);
	dock->installEventFilter(parent);
	if (dock->titleBarWidget())
		dock->titleBarWidget()->installEventFilter(parent);

	return dock;
}

QDockWidget* LayoutUtility::CreateHierarchyDock(QWidget* parent, QWidget* contentWiget)
{
	// create dock
	auto hierarchyDock = CreateHDock(parent, "World Hierarchy" );

	// set layout
	//QGridLayout* mainLayout = CreateGridLayout();
	//mainLayout->addWidget(contentWiget, 1, 1);

	//QWidget* styleWidget = new QWidget();
	//styleWidget->setLayout(mainLayout);
	hierarchyDock->setWidget(contentWiget);

	return hierarchyDock;
}


QGridLayout* LayoutUtility::CreateGridLayout()
{
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
	mainLayout->setContentsMargins(0, 0, 0, 0);

	return mainLayout;
}

QDockWidget* LayoutUtility::CreatePropertiesDock(QWidget* parent, QWidget* contentWiget)
{
	auto propertiesDock = CreateHDock(parent, "World Properties");

	propertiesDock->setWidget(contentWiget);

	return propertiesDock;
}

QDockWidget* LayoutUtility::CreateOutputDock(QWidget* parent, QWidget* contentWiget)
{
	auto outputDock = CreateHDock(parent, "Output");
	outputDock->setWidget(contentWiget);

	return outputDock;
}
