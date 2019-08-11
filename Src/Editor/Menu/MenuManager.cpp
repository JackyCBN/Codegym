#include "MenuManager.h"
#include "MenuItem.h"
#include <cassert>
#include <QMenuBar>
#include <QMenu>
#include <QAction>
#include "Window/MainWindow.h"

using namespace codegym::editor;

namespace
{
	sMenuItem* gMenuRoot = nullptr;

	const char MENU_SEPERATOR = '/';

	MainWindow* gEditorWindow = nullptr;
}


set<const sMenuItem*> cMenuManager::m_ExistingMenuItems;

void cMenuManager::Initlize(MainWindow* editorWindow)
{
	gEditorWindow = editorWindow;

	gMenuRoot = sMenuItem::CreateSubmenu(-1, "MenuRoot");

	int priority = numeric_limits<int>::min();
	gMenuRoot->AddMenuItem(sMenuItem::CreateSubmenu(priority, "File"));
	gMenuRoot->AddMenuItem(sMenuItem::CreateSubmenu(priority, "Help"));
}

sMenuItem* cMenuManager::GetRootMenu()
{
	return gMenuRoot;
}

sMenuItem* cMenuManager::GetMenuItem(const string& menuPath)
{
	return GetMenuItemRecurisve(gMenuRoot, menuPath);
}

sMenuItem* cMenuManager::GetMenuItem(sMenuItem* parent, const string& menuName)
{
	return  parent->GetSubmenu(menuName);
}

sMenuItem* cMenuManager::GetMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath)
{
	string menuName, path;

	if (!SeparateMenuPath(subMenuPath, menuName, path))
	{
		return parent->GetSubmenu(menuName);
	}
	else
	{
		sMenuItem* subMenuItem = parent->GetSubmenu(menuName);
		if (subMenuItem)
		{
			return GetMenuItemRecurisve(subMenuItem, path);
		}
	}

	return nullptr;
}

sMenuItem* cMenuManager::GetOrCreateSubMenuItem(sMenuItem* parent, const string& menuName, const int position)
{
	sMenuItem* subMenuItem = GetMenuItem(gMenuRoot, menuName);
	if (subMenuItem == nullptr)
	{
		subMenuItem = sMenuItem::CreateSubmenu(position, menuName);
		parent->AddMenuItem(subMenuItem);
	}

	return subMenuItem;
}

bool cMenuManager::RemoveMenuItem(const string& menuPath)
{
	return RemoveMenuItemRecurisve(gMenuRoot, menuPath);
}

bool cMenuManager::RemoveMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath)
{
	string menuName, path;

	if (!SeparateMenuPath(subMenuPath, menuName, path))
	{
		return parent->RemoveMenuItem(menuName);
	}
	else
	{
		sMenuItem* subMenuItem = parent->GetSubmenu(menuName);
		if(subMenuItem)
		{
			return RemoveMenuItemRecurisve(subMenuItem, path);
		}
	}

	return false;
}

sMenuItem* cMenuManager::AddMenuItem(const string& menuPath, int position,
	const string& hotkey, const string& cmd, MenuInterface* target)
{
	string menuName, path;

	if (!SeparateMenuPath(menuPath, menuName, path))
	{
		assert("menuPath dose not have submenu");
		return nullptr;
	}
	else
	{
		sMenuItem*  subMenuItem = GetOrCreateSubMenuItem(gMenuRoot, menuName, position);
		return AddMenuItemRecurisve(subMenuItem, path, position, hotkey, cmd, target);
	}
}

sMenuItem* cMenuManager::AddMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath, int position,
	const string& hotkey, const string& cmd, MenuInterface* target)
{
	if (target == nullptr)
	{
		assert(target != nullptr && "menuPath's target == nullptr");
		return nullptr;
	}

	string menuName, path;

	if (!SeparateMenuPath(subMenuPath, menuName, path))
	{
		sMenuItem* subMenuItem = sMenuItem::CreateMenuItem(position, menuName, hotkey, cmd, target);
		parent->AddMenuItem(subMenuItem);

		return subMenuItem;
	}
	else
	{
		sMenuItem* subMenuItem = GetOrCreateSubMenuItem(parent, menuName, position);
		return AddMenuItemRecurisve(subMenuItem, path, position, hotkey, cmd, target);
	}
}

bool cMenuManager::ExecuteMenu(const string& menuPath, eExecutionCause cause)
{
	sMenuItem*  menuItem = GetMenuItem(menuPath);
	if(menuItem)
	{
		ExecuteMenuItem(menuItem, cause);
	}
	return false;
}

bool cMenuManager::ExecuteMenuItem(const sMenuItem* menuItem, eExecutionCause cause)
{
	if(menuItem)
	{
		return menuItem->Execute();
	}

	return false;
}

bool cMenuManager::SeparateMenuPath(const string& menuPath, string& menuName, string& subMenuPath)
{
	const char* str = menuPath.c_str();
	const char* sep = strchr(str, MENU_SEPERATOR);
	
	if(sep)
	{
		menuName = string(str, sep);
		subMenuPath = string(sep + 1);
	}
	else
	{
		menuName = menuPath;
	}

	return sep != nullptr;
}

void UpdateEnabled(QMenu* menu)
{
	foreach(QAction * action, menu->actions())
	{
		if (!action->isSeparator() && !action->menu())
		{
			sMenuItem* menuItem = static_cast<sMenuItem*>(action->data().value<void*>());
			if (cMenuManager::MenuItemExists(menuItem))
			{
				action->setEnabled(menuItem->Enabled());
			}			
		}
	}
}

static void RemoveEmptyParentMenus(Menu& menu)
{
	Menu::iterator next;
	for(auto iter = menu.begin(); iter!=menu.end();)
	{
		sMenuItem* item = *iter;

		if (item->IsSubmenu())
			RemoveEmptyParentMenus(*item->m_submenu);

		if ((item->m_submenu == nullptr || item->m_submenu->empty()) &&
			item->m_target == nullptr && !item->m_name.empty())
		{
			iter = menu.erase(iter);
		}
		else
		{
			++iter;
		}
	}
}


void RemoveEmptyParentMenus(sMenuItem* submenu)
{
	if (!submenu)
		return;

	if(submenu->IsSubmenu())
	{
		RemoveEmptyParentMenus(*(submenu->m_submenu));
		if(submenu->m_submenu->empty())
		{
			auto& parentSubmenu = *(submenu->m_parent->m_submenu);
			auto iter = find(parentSubmenu.begin(), parentSubmenu.end(), submenu);
			if(iter != parentSubmenu.end())
			{
				parentSubmenu.erase(iter);
			}
		}
	}
}

void UpdateMenusRecusive(QMenu* menu, sMenuItem* submenu)
{
	vector<MenuInterface*> interfaces;
	submenu->CollectInterfaces(interfaces);

	bool needUpdate = false;
	for (auto& oneItem : interfaces)
	{
		needUpdate |= oneItem->Update();
	}

	if(needUpdate)
	{
		RemoveEmptyParentMenus(submenu);
		menu->clear();
		cMenuManager::RebuildOsMenusRecusive(menu, submenu);
	}
}

void UpdateMenus(QMenu* menu)
{
	const auto& actions = menu->actions();

	bool menuEmpty = actions.empty();
	if(!actions.isEmpty())
	{
		const auto& action = actions[0];
		sMenuItem* menuItem = static_cast<sMenuItem*>(action->data().value<void*>());
		if (cMenuManager::MenuItemExists(menuItem))
		{
			UpdateMenusRecusive(menu, menuItem->m_parent);
		}

		menuEmpty = menu->actions().empty();
	}

	if(menuEmpty)
	{
		const auto parent = menu->parent();
		if(parent)
		{
			QAction* menuToBeRemoved = menu->menuAction();
			if(parent!=gEditorWindow->GetMenubar())
			{
				static_cast<QMenu*>(parent)->removeAction(menuToBeRemoved);
			}
			else
			{
				gEditorWindow->GetMenubar()->removeAction(menuToBeRemoved);
			}
		}
	}
}

void AboutToShowMenu(QMenu* menu)
{
	UpdateMenus(menu);
	UpdateEnabled(menu);
}


void cMenuManager::RebuildOsMenus()
{
	gEditorWindow->GetMenubar()->clear();
	RebuildOsMenusRecusive(gEditorWindow->GetMenubar(), gMenuRoot);
}


void cMenuManager::DoAddSubMenu(QWidget* parent, QMenu* menu)
{
	if(QMenuBar * parentMenuBar = qobject_cast<QMenuBar*>(parent))
	{
		parentMenuBar->addMenu(menu);
	}
	else if(QMenu * parentMenu = qobject_cast<QMenu*>(parent))
	{
		parentMenu->addMenu(menu);
	}
}

QMenu* cMenuManager::AddSubMenu(QWidget* parent, sMenuItem* menuItem)
{
	auto menu = new QMenu(menuItem->m_name.c_str(), parent);

	DoAddSubMenu(parent, menu);
	const auto func = [=]() {
		AboutToShowMenu(menu);
	};
	gEditorWindow->connect(menu, &QMenu::aboutToShow, func);

	return menu;
}

void cMenuManager::DoAddAction(QWidget* parent, QAction* action)
{
	if (QMenuBar * menuBar = qobject_cast<QMenuBar*>(parent))
	{
		menuBar->addAction(action);
	}
	else if (QMenu * parentMenu = qobject_cast<QMenu*>(parent))
	{
		parentMenu->addAction(action);
	}
}

QAction* cMenuManager::AddAction(QWidget* parent, sMenuItem* menuItem)
{
	QAction* action = new QAction(menuItem->m_name.c_str(), parent);
	QVariant variant(QVariant::fromValue(static_cast<void*>(menuItem)));
	action->setData(variant);
	DoAddAction(parent, action);
	const auto func = [=]() {
		(void)menuItem->Execute();
	};

	gEditorWindow->connect(action, &QAction::triggered, func);

	return action;
}

void cMenuManager::RebuildOsMenusRecusive(QWidget* parent, const sMenuItem* parentItem)
{
	Menu* submenu = parentItem->m_submenu;

	int position = -1;
	for (int i=0; i!=submenu->size(); ++i)
	{
		sMenuItem* menuItem = (*submenu)[i];
		if(menuItem->m_position != -1 && menuItem->m_position/10 > position/10)
		{
			if (QMenu * parentMenu = qobject_cast<QMenu*>(parent))
			{
				parentMenu->addSeparator();
			}
		}
		position = menuItem->m_position;

		if (menuItem->IsSubmenu())
		{
			QMenu* menu =	AddSubMenu(parent, menuItem);
			RebuildOsMenusRecusive(menu, menuItem);
		}
		else
		{
			AddAction(parent, menuItem);
		}
	}

}
