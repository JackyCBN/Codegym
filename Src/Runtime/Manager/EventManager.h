#pragma once
#include "Base.h"
#include "BaseClasses/Type.h"
#include "BaseManger.h"
#include "Signal/Emitter.h"

namespace codegym::runtime
{
	class Object;
	
	class EventManager : public BaseManger
	{
	public:
		void Initlize() override;
		void Deinitlize() override;

		//Emitter<Object*, const rttr::property&> OnObjectPropertyChange;
	};

	EventManager* GetEventManagerPtr();
	EventManager& GetEventManager();
}
