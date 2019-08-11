#include "ManagerContext.h"
#include "Logging/LogAssert.h"

using namespace codegym::runtime;
static ManagerContext gContext;


BaseManger* ManagerContext::GetManger(eManagerType index) const
{
	return m_managers[static_cast<int>(index)];
}

void ManagerContext::SetManager(eManagerType index, BaseManger* manger)
{
	m_managers[static_cast<int>(index)] = manger;
}

BaseManger* codegym::runtime::GetManagerPtrFromContext(eManagerType type)
{
	int index = static_cast<int>(type);
	if (index < 0 || index >= static_cast<int>(eManagerType::kManagerCount))
	{
		FatalErrorString("GetManagerFromContext: index for managers table is out of bounds");
	}

	return gContext.GetManger(type);
}

void codegym::runtime::SetManagerPtrInContext(eManagerType type, BaseManger* ptr)
{
	int index = static_cast<int>(type);
	if (index < 0 || index >= static_cast<int>(eManagerType::kManagerCount))
	{
		FatalErrorString("GetManagerFromContext: index for managers table is out of bounds");
	}
	
	gContext.SetManager(type, ptr);
}

ManagerContext::ManagerContext()
{
	for (int i = 0; i < static_cast<int>(eManagerType::kManagerCount); i++)
		m_managers[i] = nullptr;
}

BaseManger& codegym::runtime::GetManagerFromContext(eManagerType type)
{
	const auto result = GetManagerPtrFromContext(type);

	return *result;
}



const ManagerContext& codegym::runtime::GetManagerContext()
{
	return gContext;
}
