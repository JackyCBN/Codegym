#pragma once
// rttr
#include "rttr/type"
#include "rttr/rttr_enable.h"
#include "rttr/registration.h"
#include "rttr/registration_friend.h"

//property flags stored in metadata
#define RTTR_FLAGS				1
#define RTTR_CHANGE_EVENT		2

//Editor flags
#define PF_NO_FLAG					0x00000000
#define PF_EDITOR_READ				0x00000001
#define PF_EDITOR_WRITE				0x00000002
#define PF_EDITOR_RW				(PF_EDITOR_READ | PF_EDITOR_WRITE)

#define RTTR_BEGIN_CLASS(_CLASSNAME) \
	{ \
		using _REGISTERINGCLASS = _CLASSNAME; \
		registration::class_<_CLASSNAME>(#_CLASSNAME) \
		.constructor<>()(policy::ctor::as_raw_ptr)


#define RTTR_PROPERTY_1(_PROPNAME)	RTTR_PROPERTY_2(_PROPNAME, PF_NO_FLAG)
#define RTTR_PROPERTY_2(_PROPNAME, _FLAGS) \
		.property(#_PROPNAME, &_REGISTERINGCLASS::##m_##_PROPNAME)(policy::prop::as_reference_wrapper, detail::metadata(RTTR_FLAGS, _FLAGS))
#define RTTR_PROPERTY_3(_PROPNAME, _FLAGS, _EVENT) \
		.property(#_PROPNAME, &_REGISTERINGCLASS::##m_##_PROPNAME)(policy::prop::as_reference_wrapper, detail::metadata(RTTR_FLAGS, _FLAGS), detail::metadata(RTTR_CHANGE_EVENT, _EVENT))

#define RTTR_VAR_PROPERTY(...) PP_VARG_SELECT_OVERLOAD(RTTR_PROPERTY_,(__VA_ARGS__))



#define RTTR_METHOD_PROPERTY_2(_PROPNAME, _FLAGS) \
		.property(#_PROPNAME, &_REGISTERINGCLASS::Get##_PROPNAME, &_REGISTERINGCLASS::Set##_PROPNAME)(detail::metadata(RTTR_FLAGS, _FLAGS))

#define RTTR_METHOD_PROPERTY_3(_PROPNAME, _FLAGS, _EVENT) \
		.property(#_PROPNAME, &_REGISTERINGCLASS::Get##_PROPNAME, &_REGISTERINGCLASS::Set##_PROPNAME)(detail::metadata(RTTR_FLAGS, _FLAGS), detail::metadata(RTTR_CHANGE_EVENT, _EVENT))

#define RTTR_METHOD_PROPERTY(...) PP_VARG_SELECT_OVERLOAD(RTTR_METHOD_PROPERTY_,(__VA_ARGS__))

#define RTTR_PROPERTY(...) RTTR_METHOD_PROPERTY(__VA_ARGS__)

#define RTTR_METHOD_PROPERTY_READONLY(_PROPNAME, _FLAGS) \
		.property_readonly(#_PROPNAME, &_REGISTERINGCLASS::Get##_PROPNAME)(detail::metadata(RTTR_FLAGS, _FLAGS&(PF_EDITOR_READ&~PF_EDITOR_WRITE)))

#define RTTR_PROPERTY_READONLY(...) DETAIL__PP_EXPAND_2(RTTR_METHOD_PROPERTY_READONLY(__VA_ARGS__))

#define RTTR_END_CLASS ;}

#define RTTR_METHOD(_METHODNAME) \
		.method(#_METHODNAME, &_REGISTERINGCLASS::_METHODNAME)


inline bool propertyCheckFlag(const rttr::property& prop, uint32_t flag)
{
	uint32_t metaData = prop.get_metadata(RTTR_FLAGS).to_uint32();
	bool hr = metaData & flag;
	return hr;
}

inline bool methodCheckFlag(const rttr::method& method, uint32_t flag)
{
	uint32_t metaData = method.get_metadata(RTTR_FLAGS).to_uint32();
	bool hr = metaData & flag;
	return hr;
}