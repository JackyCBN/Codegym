#pragma once
#include "Base.h"
namespace codegym::runtime
{
	enum class eManagerType;
	class BaseManger;
	
	class CG_API  ManagerMgr
	{
	public:
		void Initlize();

		void Deinitlize();
	
		
	private:
		void registerModules();
		void initModules();
		void deinitModules();
		void unregisterModules();

		vector<BaseManger*>	m_modules;
	};
};