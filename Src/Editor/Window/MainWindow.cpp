#include "MainWindow.h"
#include <QMenuBar>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QApplication>
#include <QDockWidget>
#include <QListWidget>
#include <QTextBrowser>
#include <QMessageBox>
#include "QtPropertyBrowser/QtVariantPropertyManager"

#include "Hierarchy/HierarchyWindow.h"
#include "Inspector/InspectorWindow.h"
#include "Hierarchy/HierarchyViewModel.h"
#include "BaseClasses/GameObject.h"
#include "Scene/SceneGraph.h"
#include "Component/Transform.h"
#include "Menu/MenuItem.h"





using namespace codegym::editor;
using namespace codegym::runtime;

struct ProptertyInfo
{
	GameObject* go;
	Component* comp;
	rttr::property rttr;
};

map< QtProperty*, ProptertyInfo> mPropertyMap;

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

void MainWindow::treeViewClickProgress(const QModelIndex& index)
{
	HierarchyViewModel* m_model = (HierarchyViewModel*)index.model();
	QStandardItem* pItem = m_model->itemFromIndex(index);

	GameObject* go = static_cast<GameObject*>(pItem->data().value<void*>());

	if (go)
	{
		auto name = go->Name();
		int a = 1;
		//QMessageBox::about(this, name, name);
		m_pVarManager->clear();

		// GameObject
		mPropertyMap.clear();
		QtProperty* goGroupItem = m_pVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), QStringLiteral("GameObject"));
		// name
		QtVariantProperty* item = m_pVarManager->addProperty(QVariant::String, QStringLiteral("Name: "));
		item->setValue(name);
		QVariant goVariant(QVariant::fromValue(static_cast<void*>(go)));
		item->setAttribute("GameObject", goVariant);
		goGroupItem->addSubProperty(item);
		m_inspectorWindow->addProperty(goGroupItem);

		mPropertyMap.insert(std::make_pair(item, ProptertyInfo{ go , nullptr, TypeOf<GameObject>().get_property("Name") }));
;
		for(int i=0; i!=go->GetComponentCount(); ++i)
		{
			Component& comp =  go->GetComponentAtIndex(i);
			QtProperty* compGroupItem = m_pVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), comp.get_type().get_name().begin());

			rttr::type type = comp.get_type();
			for(auto& prop : type.get_properties())
			{
				if(prop.get_type() == TypeOf<float>() || prop.get_type() == TypeOf<double>())
				{
					QtVariantProperty* copmItem = m_pVarManager->addProperty(QVariant::Double, prop.get_name().begin());
					auto val = prop.get_value(comp);
					copmItem->setValue(val.to_float());

					QVariant compVariant(QVariant::fromValue(static_cast<void*>(&comp)));
					copmItem->setAttribute("Component", compVariant);

					QVariant rttrVariant(prop.get_name().begin());
					copmItem->setAttribute("Rttr", rttrVariant);

					compGroupItem->addSubProperty(copmItem);
					mPropertyMap.insert(std::make_pair(copmItem, ProptertyInfo{ go , &comp, prop }));
				}
			}
			m_inspectorWindow->addProperty(compGroupItem);
		}

		//item = m_pVarManager->addProperty(QVariant::Bool, QStringLiteral("Bool: "));
		//item->setValue(true);
		//groupItem->addSubProperty(item);

		//item = m_pVarManager->addProperty(QVariant::Double, QStringLiteral("Double: "));
		//item->setValue(3.1415);
		//groupItem->addSubProperty(item);

		//item = m_pVarManager->addProperty(QVariant::String, QStringLiteral("String: "));
		//item->setValue(QStringLiteral("hello world"));
		//groupItem->addSubProperty(item);

		
	}
}

void MainWindow::variantPropertyValueChanged(QtProperty* property, const QVariant& value)
{

	auto iter = mPropertyMap.find(property);
	if(iter != mPropertyMap.end())
	{
		//int aa = 1;

		if(iter->second.comp != nullptr)
		{
			// component	
			rttr::instance inst(*(iter->second.comp));
			iter->second.rttr.set_value(inst, value.toFloat());
		}
		else
		{
			rttr::instance inst(*(iter->second.go));
			string name = value.toString().toStdString();
			iter->second.rttr.set_value(inst, string(name));
			//game object
		}
	}
	QtVariantProperty* varProperty = (QtVariantProperty*)property;
	if(varProperty)
	{
		QVariant goVar =  varProperty->attributeValue("GameObject");
		QVariant compVar = varProperty->attributeValue("Component");
		QVariant RttrVar = varProperty->attributeValue("Rttr");

		auto a= goVar.isNull();
		auto b = compVar.isNull();
		auto c = RttrVar.isNull();
		int xx = 1;
	}
	auto name = property->propertyName().toStdString().c_str();
	int a = 1;
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

	//const auto func = [=]() {
	//	int row =m_hierarchyWindow->currentIndex().row();
	//	int col = m_hierarchyWindow->currentIndex().column();
	//};

	//connect(m_hierarchyWindow, &QTreeView::clicked, func);

	QObject::connect(m_hierarchyWindow, SIGNAL(clicked(const QModelIndex)), this, SLOT(treeViewClickProgress(const QModelIndex)));


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

	m_pVarManager = new QtVariantPropertyManager(m_inspectorWindow);
	m_pVarFactory = new QtVariantEditorFactory(m_inspectorWindow);
	connect(m_pVarManager, &QtVariantPropertyManager::valueChanged, this, &MainWindow::variantPropertyValueChanged);
	m_inspectorWindow->setFactoryForManager(m_pVarManager, m_pVarFactory);
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
		QVariant variant(QVariant::fromValue(static_cast<void*>(childTrans->GetGameObject())));
		childItem->setData(variant);

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
	QVariant variant(QVariant::fromValue(static_cast<void*>(node)));
	rootItem->setData(variant);

	model->appendRow(itemslst);


	CreateItemRecusive(node->QueryComponent<Transform>(), rootItem);


	//2£¬set QTreeView model
	m_hierarchyWindow->setModel(model);
}
