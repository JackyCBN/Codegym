#include "Object.h"
#include "Manager/EventManager.h"

using namespace codegym::runtime;

void Object::NotifyPropertyChange(const char* propertyName)
{
	const auto prop = get_type().get_property(propertyName);
	if(prop.is_valid())
	{
		bool bOk = false;
		auto f = prop.get_value(*this).to_string(&bOk);
		
		GetEventManager().OnObjectPropertyChange.publish(*this, prop);
	}
}


RTTR_REGISTRATION
{
	using namespace rttr;
		registration::class_<Object>("Object");
}

