#include "Component.h"

using namespace codegym::runtime;

Component::Component()
{

}
RTTR_REGISTRATION
{
	using namespace rttr;
	registration::class_<Component>("codegym::runtime::Component")
		;
}