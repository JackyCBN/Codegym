#pragma once

#include <QMainWindow>
#include "BaseClasses/GameObject.h"

class QStandardItem;

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

	private:
		void CreateHierarchy();
		void CreateProperties();
		void CreateOutput();
		void CreateItemRecusive(runtime::Transform* node, QStandardItem* item);

		void InitHierarchyContent(runtime::SceneGraph* sg);

		QMenuBar* m_menubar = nullptr;
		HierarchyWindow* m_hierarchyWindow;
		InspectorWindow* m_inspectorWindow;
		QTextBrowser*	 m_outputWindow;
	};
}
