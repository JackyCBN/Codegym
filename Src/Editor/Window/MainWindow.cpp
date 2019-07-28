#include "MainWindow.h"
#include <QMenuBar>
#include <QHBoxLayout>
#include "HierarchyWindow.h"
#include "InspectorWindow.h"
#include <QLabel>
#include <QTableView>
#include <QLineEdit>
#include <QApplication>

MainWindow::MainWindow(QWidget* parent)
	:QMainWindow(parent)
{
	m_menubar = menuBar();


}

void MainWindow::InitLayout()
{
	//QWidget window;

	QLabel* queryLabel = new QLabel(
		QApplication::translate("nestedlayouts", "Query:"),this);
	//QLineEdit* queryEdit = new QLineEdit();
	//QTableView* resultView = new QTableView();

	//QHBoxLayout* queryLayout = new QHBoxLayout();
	//queryLayout->addWidget(queryLabel);
	//queryLayout->addWidget(queryEdit);

	//QVBoxLayout* mainLayout = new QVBoxLayout();
	//mainLayout->addLayout(queryLayout);
	//mainLayout->addWidget(resultView);
	////window.setLayout(mainLayout);
	//setLayout(mainLayout);
	////// Set up the model and configure the view...
	////window.setWindowTitle(
	////	QApplication::translate("nestedlayouts", "Nested layouts"));
	////window.show();

	/*QHBoxLayout* hlayout = new QHBoxLayout();

	HierarchyWindow* hierarchy = new HierarchyWindow();
	InspectorWindow* inspector = new InspectorWindow();
	hlayout->addWidget(hierarchy);
	hlayout->addWidget(inspector);

	QSpacerItem* item = new QSpacerItem(1, 1, QSizePolicy::Expanding, QSizePolicy::Fixed);
	hlayout->addSpacerItem(item);
	for (int i = 0; i < 10; i++)
	{
		QLabel* label = new QLabel("NOTE");
		hlayout->insertWidget(i, label);
	}

	setLayout(hlayout);*/
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
