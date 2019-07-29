#include "Base.h"
#include "EditorHelper.h"
#include "MenuManager.h"
#include "MenuItem.h"

using namespace codegym::editor;


class CreateObjectMenu : public MenuInterface
{
public:
	bool m_bremove = false;
	bool m_clickeed = false;
	bool Validate(const sMenuItem& menuItem) override
	{
		return m_clickeed ? menuItem.m_name != "abc": menuItem.m_name == "abc";
	}

	bool Execute(const sMenuItem& menuItem) override
	{
		m_bremove = menuItem.m_name == "abc";
		m_clickeed = true;
		return true;
	}

	bool Update() override
	{
		if(m_bremove)
		{
			cMenuManager::RemoveMenuItem("File/foo/abc");
			m_bremove = false;
			return true;
		}
		return false;
	}
};

CreateObjectMenu object_menu;
void test()
{
	cMenuManager::AddMenuItem("File/test", -1, "0", "test", &object_menu);
	cMenuManager::AddMenuItem("File/test2", 9, "0", "test", &object_menu);
	cMenuManager::AddMenuItem("File/foo/abc", 1, "0", "test", &object_menu);
	cMenuManager::AddMenuItem("File/foo1/foo2/cde", 1, "0", "test", &object_menu);
	cMenuManager::AddMenuItem("Help/about", 1, "0", "test", &object_menu);
}

STARTUP(test);