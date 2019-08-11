#pragma once

#include "Base.h"
#include "BaseClasses/Type.h"

namespace codegym::runtime
{
	class CG_API Object
	{
	public:
		bool Is(rttr::type type) const { return type.is_derived_from(get_type()) ; }
		template<typename T> bool Is() const { return TypeOf<T>().is_derived_from(get_type()); }
		
		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
	};
}
