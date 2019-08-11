#pragma once

#include "Macros/RttrMacro.h"


// The template argument of TypeOf must be a complete type
// Whenever possible this should be used over WeakTypeOf to avoid errors where you mistakenly forward define
// a type in the wrong namespace or with a misspelled name and end up referencing the wrong type
template<typename T>
rttr::type TypeOf()
{
	return rttr::type::get<T>();
}
