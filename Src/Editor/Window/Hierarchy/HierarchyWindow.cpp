#include "HierarchyWindow.h"
#include <QMenuBar>
#include "HierarchyViewModel.h"
#include "BaseClasses/GameObject.h"
#include "Component/Transform.h"
#include "Scene/SceneGraph.h"

using namespace codegym::editor;
using namespace codegym::runtime;

HierarchyWindow::HierarchyWindow(QWidget* parent)
	:QTreeView(parent)
{

}

void HierarchyWindow::initlize()
{
	viewport()->installEventFilter(this);
	setObjectName("treeview");
	setFocusPolicy(Qt::StrongFocus);
	setContextMenuPolicy(Qt::CustomContextMenu);
}

void HierarchyWindow::initHierarchyContent(SceneGraph* sg)
{
	HierarchyViewModel* model = new HierarchyViewModel(this);

	QList<QStandardItem*> itemslst;
	GameObject* node = sg->Root();
	QStandardItem* rootItem = new QStandardItem(node->Name());
	itemslst.append(rootItem);
	QVariant variant(QVariant::fromValue(static_cast<void*>(node)));
	rootItem->setData(variant);

	model->appendRow(itemslst);


	CreateItemRecusive(node->QueryComponent<runtime::Transform>(), rootItem);

	//2£¬set QTreeView model
	this->setModel(model);
}

void HierarchyWindow::CreateItemRecusive(Transform* node, QStandardItem* item)
{
	for (int i = 0; i != node->ChildCount(); ++i)
	{
		auto childTrans = node->GetChild(i);

		QList<QStandardItem*> itemslst;
		QStandardItem* childItem = new QStandardItem(childTrans->GetGameObject()->Name());
		QVariant variant(QVariant::fromValue(static_cast<void*>(childTrans->GetGameObject())));
		childItem->setData(variant);

		itemslst.append(childItem);
		item->appendRow(itemslst);

		CreateItemRecusive(childTrans, childItem);
	}
}

