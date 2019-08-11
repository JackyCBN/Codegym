#pragma once

#include "Base.h"
#include "Type.h"
#include "Object.h"

namespace codegym::runtime
{
	class GameObject;
	class CG_API Component : public Object
	{
		
	public:
		Component();

		void SetGameObject(GameObject* go) { m_gameObject = go; }
		GameObject* GetGameObject() const { return m_gameObject; }
	private:
		GameObject* m_gameObject;

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND

	};
}
