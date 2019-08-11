#pragma once
#include "Base.h"
#include "Type/Type.h"
#include "BaseManger.h"
#include "Signal/Signal.h"

namespace codegym::runtime
{
	class Object;
	
	class CG_API EventManager : public BaseManger
	{
	public:
		void Initlize() override;
		void Deinitlize() override;

		signal<void(Object&, const rttr::property&)> OnObjectPropertyChange;
		signal<void(int)> OnObjectPropertyChange2;
	};

	CG_API EventManager* GetEventManagerPtr();
	CG_API EventManager& GetEventManager();
}
