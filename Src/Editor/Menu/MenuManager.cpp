#include "MenuManager.h"
#include "MenuItem.h"

using namespace codegym::editor;

namespace
{
	sMenuItem* gMenuRoot = nullptr;

	const char MENU_SEPERATOR = '/';
}

void cMenuManager::Initlize()
{
	gMenuRoot = sMenuItem::CreateSubmenu(-1, "MenuRoot", "", "", new Menu);

	gMenuRoot->AddMenuItem(sMenuItem::CreateSubmenu(-1, "File", "", "", new Menu));
	gMenuRoot->AddMenuItem(sMenuItem::CreateSubmenu(-1, "Help", "", "", new Menu));
}

sMenuItem* cMenuManager::GetRootMenu()
{
	return gMenuRoot;
}

sMenuItem* cMenuManager::FindMenuItem(const string& menuPath)
{
	const char* str = menuPath.c_str();
	const char* sep = strchr(str, MENU_SEPERATOR);
	if(sep == nullptr)
	{
		
	}
	else
	{
		
	}

}

sMenuItem* cMenuManager::FindMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath)
{

}

bool cMenuManager::RemoveMenuItem(const string& menuPath)
{
}

bool cMenuManager::RemoveMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath)
{
}

sMenuItem* cMenuManager::AddMenuItem(const string& menuPath)
{
}

sMenuItem* cMenuManager::AddMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath)
{
}

bool cMenuManager::ExecuteMenu(const string& menuName, eExecutionCause cause)
{

	return false;
}

bool cMenuManager::ExecuteMenuItem(const sMenuItem& menuITem, eExecutionCause cause)
{

}
