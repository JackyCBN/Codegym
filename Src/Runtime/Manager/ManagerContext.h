#pragma once
#include "Base.h"

namespace codegym::runtime
{
	class BaseManger;
	enum class eManagerType;

	CG_API BaseManger& GetManagerFromContext(eManagerType type);
	CG_API BaseManger* GetManagerPtrFromContext(eManagerType type);
	void SetManagerPtrInContext(eManagerType type, BaseManger* ptr);
}

#define GET_MANAGER(x) \
	    CG_API x& codegym::runtime::Get##x() { return reinterpret_cast<x &>(codegym::runtime::GetManagerFromContext(eManagerType::k##x)); }
#define GET_MANAGER_PTR(x) \
	    CG_API x* codegym::runtime::Get##x##Ptr() { return reinterpret_cast<x *>(codegym::runtime::GetManagerPtrFromContext(eManagerType::k##x)); }


namespace codegym::runtime
{
	enum class eManagerType
	{
		kEventManager = 0,

		kManagerCount,
	};

	
	class CG_API ManagerContext
	{
	public:
		ManagerContext();

		BaseManger* GetManger(eManagerType index) const;
		void		SetManager(eManagerType index, BaseManger* manger);
		
	private:
		BaseManger* m_managers[static_cast<int>(eManagerType::kManagerCount)];
		
	};
	CG_API const ManagerContext& GetManagerContext();
}
