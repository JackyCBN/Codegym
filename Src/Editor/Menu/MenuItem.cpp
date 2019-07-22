#include "MenuItem.h"
#include <cassert>

using namespace codegym::editor;

sMenuItem::sMenuItem(int postion, const string& name, const string& hotkey, const string& cmd, MenuInterface* target,
	Menu* submenu)
	:m_position(postion)
	,m_name(name)
	,m_hotkey(hotkey)
	,m_command(cmd)
	,m_submenu(submenu)
	,m_target(target)	
{

}

sMenuItem::~sMenuItem()
{
	SAFE_DELETE(m_submenu);
	SAFE_DELETE(m_target);
}

bool sMenuItem::IsSeparator() const
{
	return m_name.empty();
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
		size_t pos = m_submenu->size();
		for (size_t i = 0; i != m_submenu->size(); ++i)
		{
			if (i > menuItem->m_position)
			{
				pos = i;
				break;
			}

			auto iter = m_submenu->begin();
			advance(iter, pos);
			m_submenu->insert(iter, menuItem);
		}
	}
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


sMenuItem* sMenuItem::CreateMenuItem(int postion, const string& name,
	const string& hotkey, const string& cmd, MenuInterface* target)
{
	return new sMenuItem(postion, name, hotkey, cmd, target, nullptr);
}

sMenuItem* sMenuItem::CreateSubmenu(int postion, const string& name, const string& hotkey, const string& cmd,
	Menu* submenu)
{
	return new sMenuItem(postion, name, hotkey, cmd, nullptr, submenu);
}

/**
 * name is empty
 */
sMenuItem* sMenuItem::CreateSeparator()
{
	return new sMenuItem(-1, "", "", "", nullptr, nullptr);
}
