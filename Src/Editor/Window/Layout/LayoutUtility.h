#pragma once

class QString;
class QWidget;
class QDockWidget;
class QGridLayout;
namespace codegym::editor
{
	class LayoutUtility
	{
	public:
		static QDockWidget* CreateHDock(QWidget* parent, QString name);

		static QDockWidget* CreateHierarchyDock(QWidget* parent, QWidget* contentWiget);
		static QDockWidget* CreatePropertiesDock(QWidget* parent, QWidget* contentWiget);
		static QDockWidget* CreateOutputDock(QWidget* parent, QWidget* contentWiget);

		static QGridLayout* CreateGridLayout();
	};
}