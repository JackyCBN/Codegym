#pragma once
#include "Base.h"

class QWidget;


namespace codegym::editor
{
	enum class eExecutionCause;
	struct sMenuItem;
	class MenuInterface;
	//class MainWindow;
	using Menu = vector<sMenuItem*>;

	class cMenuManager
	{
	public:
		//static void			Initlize(MainWindow* editorWindow);
		static sMenuItem*		GetRootMenu();

		// GET
		static sMenuItem*		GetMenuItem(const string& menuPath);
		static sMenuItem*		GetMenuItem(sMenuItem* parent, const string& menuName);
		static sMenuItem*		GetMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath);

		static sMenuItem*		GetOrCreateSubMenuItem(sMenuItem* parent, const string& menuName, int position);

		// REMOVE
		static bool				RemoveMenuItem(const string& menuPath);
		static bool				RemoveMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath);


		// ADD
		static sMenuItem*		AddMenuItem(const string& menuPath, int position,
								const string& hotkey, const string& cmd, MenuInterface* target);

		static sMenuItem*		AddMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath, int position,
								const string& hotkey, const string& cmd, MenuInterface* target);

		// EXCUTE
		static bool				ExecuteMenu(const string& menuPath, eExecutionCause cause);
		static bool				ExecuteMenuItem(const sMenuItem* menuItem, eExecutionCause cause);

		// MenuItems must register and deregister with the controller as they are created and destroyed.
		static void				RegisterMenuItem(const sMenuItem* item) { m_ExistingMenuItems.insert(item); }
		static void				UnregisterMenuItem(const sMenuItem* item) { m_ExistingMenuItems.erase(item); }
		static bool				MenuItemExists(const sMenuItem* item) { return m_ExistingMenuItems.find(item) != m_ExistingMenuItems.end(); }


		static void				RebuildOsMenus();

		static void				RebuildOsMenusRecusive(QWidget* parent, const sMenuItem* menuItem);
	private:
		static bool				SeparateMenuPath(const string& menuPath, string& menuName,  string& subMenuPath);
		//static QMenu*			AddSubMenu(QWidget* parent, sMenuItem* menuItem);
		//static void				DoAddSubMenu(QWidget* parent, QMenu* menu);
		//static QAction*			AddAction(QWidget* parent, sMenuItem* menuItem);
		//static void				DoAddAction(QWidget* parent, QAction* action);


		static set<const sMenuItem*> m_ExistingMenuItems;
	};
}
