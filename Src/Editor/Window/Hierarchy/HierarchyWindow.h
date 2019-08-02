#pragma once

#include <QTreeView>
namespace codegym::editor
{
	class HierarchyWindow : public QTreeView
	{
		Q_OBJECT
	public:
		HierarchyWindow(QWidget* parent = nullptr);

	};
}