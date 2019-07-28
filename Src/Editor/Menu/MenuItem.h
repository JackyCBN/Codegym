#pragma once
#include "Base.h"
namespace codegym::editor
{
	struct sMenuItem;

	using Menu = vector<sMenuItem*>;

	enum class eExecutionCause
	{
		kExecutedByMenuSelection,
		kExecutedByKeyboardShortcut,
		kExecutedByAutomation
	};

	class MenuInterface
	{
	public:
		virtual ~MenuInterface() = default;

		/**
		 * for enable check
		 */
		virtual bool Validate(const sMenuItem& menuItem) = 0;


		/**
		 * Execute method
		 */
		virtual bool Execute(const sMenuItem& menuItem) = 0;

		/** Called before the menu will be displayed.
		 *  You can addand remove menu items in here.
		 * 	Return true to update the OS menu.
		*/
		virtual bool Update()
		{
			return false;
		}
	};


	struct sMenuItem
	{
		int			m_position;
		string		m_name;
		string		m_hotkey;
		string		m_command;
		Menu*		m_submenu;
		MenuInterface* m_target;
		sMenuItem*	m_parent;
		bool		m_checked;
		bool		m_enabled;

		sMenuItem(int position, const string& name, const string& hotkey, const string& cmd, MenuInterface* target, Menu* submenu);
		~sMenuItem();
		bool IsSeparator() const;
		bool IsSubmenu() const;
		bool Execute() const;

		bool		AddMenuItem(sMenuItem* menuItem);
		sMenuItem*	GetSubmenu(const string& submenuName) const;
		bool		RemoveMenuItem(const string& submenuName);
		bool		Enabled() const;
		bool		Checked() const;

		void		CollectInterfaces(vector<MenuInterface*>& menuInterfaces);

		/**
		 * Create Factory
		 */
		static sMenuItem* CreateMenuItem(int position, const string& name, const string& hotkey, const string& cmd, MenuInterface* target);
		static sMenuItem* CreateSubmenu(int postion, const string& name);
		static sMenuItem* CreateSeparator();
	};

}
