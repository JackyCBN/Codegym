#pragma once

#include <QMainWindow>
#include "BaseClasses/GameObject.h"

class QtProperty;
class QStandardItem;
class QtVariantPropertyManager;
class QtVariantEditorFactory;
class QtEnumPropertyManager;
class QtEnumEditorFactory;

namespace codegym {
	namespace runtime {
		class Transform;
		class SceneGraph;
	}
}

class QTextBrowser;

namespace codegym::editor
{
	class HierarchyWindow;
	class InspectorWindow;

	class MainWindow : public QMainWindow
	{
		Q_OBJECT
	public:
		MainWindow(QWidget* parent = nullptr);
		QMenuBar* GetMenubar() const;

		void SetupLayout();
		void InitContent(runtime::SceneGraph* sg);

	public Q_SLOTS:
		void treeViewClickProgress(const QModelIndex& index);
		void variantPropertyValueChanged(QtProperty* property, const QVariant& value);
	public:
		void CreateHierarchy();
		void CreateProperties();
		void CreateOutput();
		void CreateItemRecusive(runtime::Transform* node, QStandardItem* item);

		void InitHierarchyContent(runtime::SceneGraph* sg);

		QMenuBar* m_menubar = nullptr;
		HierarchyWindow* m_hierarchyWindow;
		InspectorWindow* m_inspectorWindow;
		QTextBrowser*	 m_outputWindow;

		QtVariantPropertyManager* m_pVarManager;	
		QtVariantEditorFactory* m_pVarFactory;
		//QtEnumPropertyManager* m_enumPropertyManager;
		//QtEnumEditorFactory* m_enumPropertyFactory;
	};
}
