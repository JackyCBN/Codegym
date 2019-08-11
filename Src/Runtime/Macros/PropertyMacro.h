#pragma once

#define GC_PROPERTY(_PROPERTY_TYPE, _PROPERTY_NAME, _INITIAL_VALUE) \
private: \
	_PROPERTY_TYPE m_##_PROPERTY_NAME = _INITIAL_VALUE; \
public: \
	ParameterType_t<_PROPERTY_TYPE> Get##_PROPERTY_NAME() const \
	{\
		return m_##_PROPERTY_NAME; \
	}\
	void Set##_PROPERTY_NAME(ParameterType_t<_PROPERTY_TYPE> _value) \
	{ \
		if (m_##_PROPERTY_NAME != _value) \
		{ \
			m_##_PROPERTY_NAME = _value; \
			NotifyPropertyChange(#_PROPERTY_NAME); \
		} \
	    else\
	    {\
			m_##_PROPERTY_NAME = _value; \
		}\
	}

#define GC_PROPERTY_READONLY(_PROPERTY_TYPE, _PROPERTY_NAME, _INITIAL_VALUE) \
private: \
	_PROPERTY_TYPE m_##_PROPERTY_NAME = _INITIAL_VALUE; \
	public: \
	_PROPERTY_TYPE Get##_PROPERTY_NAME() const \
	{\
		return m_##_PROPERTY_NAME; \
	}