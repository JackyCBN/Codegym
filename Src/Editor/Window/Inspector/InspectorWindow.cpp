#include "InspectorWindow.h"
#include <QMenuBar>
#include "Window/MainWindow.h"
#include <QtPropertyBrowser/QtVariantPropertyManager>
using namespace codegym::runtime;
using namespace codegym::editor;

InspectorWindow::InspectorWindow(QWidget* parent)
	:QtTreePropertyBrowser(parent)
	, m_pVarManager(new QtVariantPropertyManager(this))
	, m_pVarFactory(new QtVariantEditorFactory(this))

{


}

void InspectorWindow::initlize()
{
	connect(m_pVarManager, &QtVariantPropertyManager::valueChanged, this, &InspectorWindow::onValueChanged);
	setFactoryForManager(m_pVarManager, m_pVarFactory);
	//installEventFilter(this);
	setObjectName(tr("propertyBrowser"));
	setFocusPolicy(Qt::StrongFocus);
	setAcceptDrops(true);
}

void InspectorWindow::onItemSelectChange(GameObject* go)
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
	addProperty(goGroupItem);

	mPropertyMap.insert(make_pair(item, ProptertyInfo{ go , TypeOf<GameObject>().get_property("Name") }));
	;
	for (int i = 0; i != go->GetComponentCount(); ++i)
	{
		Component& comp = go->GetComponentAtIndex(i);
		QtProperty* compGroupItem = m_pVarManager->addProperty(QtVariantPropertyManager::groupTypeId(), comp.get_type().get_name().begin());

		rttr::type type = comp.get_type();
		for (auto& prop : type.get_properties())
		{
			if (prop.get_type() == TypeOf<float>() || prop.get_type() == TypeOf<double>())
			{
				QtVariantProperty* copmItem = m_pVarManager->addProperty(QVariant::Double, prop.get_name().begin());
				auto val = prop.get_value(comp);
				copmItem->setValue(val.to_float());

				QVariant compVariant(QVariant::fromValue(static_cast<void*>(&comp)));
				copmItem->setAttribute("Component", compVariant);

				QVariant rttrVariant(prop.get_name().begin());
				copmItem->setAttribute("Rttr", rttrVariant);

				compGroupItem->addSubProperty(copmItem);
				mPropertyMap.insert(make_pair(copmItem, ProptertyInfo{ &comp, prop }));
			}
		}
		addProperty(compGroupItem);
	}
}

void InspectorWindow::onPropertyChanged(Object* object, const rttr::property& newVal)
{
	
}

void InspectorWindow::onValueChanged(QtProperty* property, const QVariant& value)
{
	auto iter = mPropertyMap.find(property);
	if (iter != mPropertyMap.end())
	{
		//int aa = 1;
		auto obj = iter->second.object;
		if (obj != nullptr)
		{
			rttr::instance inst(*obj);
			
			if(obj->Is< Component>())
			{
				// component	

				iter->second.rttr.set_value(inst, value.toFloat());
			}
			else
			{
				//game object
				string name = value.toString().toStdString();
				iter->second.rttr.set_value(inst, string(name));
			}
		}
	}
}
