#include "MenuItem.h"
#include <cassert>
#include "MenuManager.h"

using namespace codegym::editor;

sMenuItem::sMenuItem(int position, const string& name, const string& hotkey, const string& cmd, MenuInterface* target,
	Menu* submenu)
	:m_position(position)
	,m_name(name)
	,m_hotkey(hotkey)
	,m_command(cmd)
	,m_submenu(submenu)
	,m_target(target)
	,m_parent(nullptr)
	,m_enabled(true)
	,m_checked(false)
{
	cMenuManager::RegisterMenuItem(this);
}

sMenuItem::~sMenuItem()
{
	SAFE_DELETE(m_submenu);
	// m_target share by multi item. so not necessary to delete it. 
	// just let it even memory leak
	// also for most, it may be static global variable
	//SAFE_DELETE(m_target);
	cMenuManager::RegisterMenuItem(this);
}

bool sMenuItem::IsSeparator() const
{
	return m_name.empty();
}

bool sMenuItem::IsSubmenu() const
{
	return m_submenu != nullptr;
}

bool sMenuItem::Execute() const
{
	assert(m_submenu== nullptr && m_target && "m_submenu==null && m_target!=null");
	if(m_submenu!=nullptr)
	{
		// log
		assert(m_submenu == nullptr && "this is submenu");
		return false;
	}
	if (m_target == nullptr)
	{
		// log
		assert(m_target != nullptr && "this menu item not exist target");
		return false;
	}

	return m_target->Execute(*this);
}

bool sMenuItem::AddMenuItem(sMenuItem* menuItem)
{
	if (m_submenu)
	{
		int pos = (int)m_submenu->size();
		for (int i = 0; i != m_submenu->size(); ++i)
		{
			if (menuItem->m_position < (*m_submenu)[i]->m_position)
			{
				pos = i;
				break;
			}
		}

		auto iter = m_submenu->begin();
		advance(iter, pos);
		m_submenu->insert(iter, menuItem);
		menuItem->m_parent = this;
		return true;
	}

	return false;
}

sMenuItem* sMenuItem::GetSubmenu(const string& submenuName) const
{
	if (m_submenu)
	{
		for (size_t i = 0; i != m_submenu->size(); ++i)
		{
			if((*m_submenu)[i]->m_name == submenuName)
			{
				return (*m_submenu)[i];
			}
		}
	}

	return nullptr;
}

bool sMenuItem::RemoveMenuItem(const string& submenuName)
{
	if (m_submenu)
	{
		for (size_t i = 0; i != m_submenu->size(); ++i)
		{
			sMenuItem* item = (*m_submenu)[i];
			if (item->m_name == submenuName && !item->IsSubmenu())
			{
				auto iter = m_submenu->begin();
				advance(iter, i);
				SAFE_DELETE(*iter);
				m_submenu->erase(iter);

				return true;
			}
		}
	}

	return false;
}

bool sMenuItem::Enabled() const
{
	return m_enabled && (m_target == nullptr || m_target->Validate(*this));
}

bool sMenuItem::Checked() const
{
	return m_checked;
}

void sMenuItem::CollectInterfaces(vector<MenuInterface*>& menuInterfaces)
{
	if(!IsSubmenu())
	{
		menuInterfaces.push_back(m_target);
	}
	else
	{
		for(auto& item : *m_submenu)
		{
			item->CollectInterfaces(menuInterfaces);
		}
	}
}


sMenuItem* sMenuItem::CreateMenuItem(int position, const string& name,
	const string& hotkey, const string& cmd, MenuInterface* target)
{
	return new sMenuItem(position, name, hotkey, cmd, target, nullptr);
}

sMenuItem* sMenuItem::CreateSubmenu(int position, const string& name)
{
	return new sMenuItem(position, name, "", "", nullptr, new Menu);
}

/**
 * name is empty
 */
sMenuItem* sMenuItem::CreateSeparator()
{
	return new sMenuItem(-1, "", "", "", nullptr, nullptr);
}
