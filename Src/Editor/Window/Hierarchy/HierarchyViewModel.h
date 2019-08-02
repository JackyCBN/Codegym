#pragma once

#include <QStandardItemModel>

namespace codegym::editor
{
	class HierarchyViewModel : public QStandardItemModel
	{
		Q_OBJECT
	public:
		HierarchyViewModel(QWidget* parent = nullptr);

	};
}