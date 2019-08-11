#pragma once

#include <QTreeView>

class QStandardItem;

namespace codegym {
	namespace runtime {
		class Transform;
		class SceneGraph;
	}
}

namespace codegym::editor
{
	class HierarchyWindow : public QTreeView
	{
		Q_OBJECT
	public:
		HierarchyWindow(QWidget* parent = nullptr);

		void initlize();
		void initHierarchyContent(runtime::SceneGraph* sg);
		void CreateItemRecusive(runtime::Transform* node, QStandardItem* item);
	};
}
