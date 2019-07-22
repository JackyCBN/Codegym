#pragma once
#include "Base.h"

namespace codegym::editor
{
	enum class eExecutionCause;
	struct sMenuItem;
	using Menu = vector<sMenuItem*>;

	class cMenuManager
	{
	public:
		static void				Initlize();
		static sMenuItem*		GetRootMenu();

		static sMenuItem*		FindMenuItem(const string& menuPath);
		static sMenuItem*		FindMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath);

		static bool				RemoveMenuItem(const string& menuPath);
		static bool				RemoveMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath);

		static sMenuItem*		AddMenuItem(const string& menuPath);
		static sMenuItem*		AddMenuItemRecurisve(sMenuItem* parent, const string& subMenuPath);

		static bool				ExecuteMenu(const string& menuName, eExecutionCause cause);
		static bool				ExecuteMenuItem(const sMenuItem& menuITem, eExecutionCause cause);
	private:
	};
}
