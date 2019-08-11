#pragma once
#include "Base.h"
#include "QtPropertyBrowser/qttreepropertybrowser.h"
#include <rttr/property.h>

class QWidget;
class QtProperty;
class QStandardItem;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtEnumPropertyManager;
class QtEnumEditorFactory;

namespace codegym {
	namespace runtime {
		class Object;
		class Transform;
		class SceneGraph;
		class  GameObject;
		class Component;
	}
}

namespace codegym::editor
{
	class InspectorEditor;

	struct ProptertyInfo
	{
		runtime::Object* object;
		rttr::property rttr;
	};


	class InspectorWindow : public QtTreePropertyBrowser
	{
		Q_OBJECT
	public:
		InspectorWindow(QWidget* parent = nullptr);

		void initlize();
		void onItemSelectChange(runtime::GameObject* go);
		void onPropertyChanged(runtime::Object* object, const rttr::property& newVal);
		
	private Q_SLOTS:
		void onValueChanged(QtProperty* property, const QVariant& value);
	private:


		map< QtProperty*, ProptertyInfo> mPropertyMap;
		vector<InspectorEditor*> m_editors;

		QtVariantPropertyManager* m_pVarManager;
		QtVariantEditorFactory* m_pVarFactory;
	};
}