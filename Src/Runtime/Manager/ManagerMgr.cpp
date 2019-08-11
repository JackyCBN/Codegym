#include "ManagerMgr.h"
#include "EventManager.h"
#include "ManagerContext.h"

using namespace codegym::runtime;
void ManagerMgr::Initlize()
{
	registerModules();
	initModules();
}

void ManagerMgr::Deinitlize()
{
	deinitModules();
	unregisterModules();
}

void ManagerMgr::registerModules()
{
	SetManagerPtrInContext(eManagerType::kEventManager, new EventManager());
}

void ManagerMgr::initModules()
{
	for(auto& mod : m_modules)
	{
		mod->Initlize();
	}
}

void ManagerMgr::deinitModules()
{
	for (auto& mod : m_modules)
	{
		mod->Deinitlize();
	}
}

void ManagerMgr::unregisterModules()
{
	m_modules.clear();
}
