#pragma once

#include "Base.h"
#include "BaseClasses/Component.h"

namespace codegym::runtime
{
	class GameObject;
	class CG_API Transform : public Component
	{
		
	public:
		Transform();

		int ChildCount() const { return static_cast<int>(m_childs.size()); }
		Transform* GetChild(int index) const;

		void RemoveChild(Transform* child);

		void AddChild(Transform* child);

		Transform* GetParent() const { return m_parent; }
		void SetParent(Transform* parent);

		void SetPosition(float x, float y, float z);

		GC_PROPERTY(string, Test, "test");
		//GC_PROPERTY_READONLY(float, MyTest, 3);
		private: float m_MyTest = 3; public: float GetMyTest() const { return m_MyTest; };
	private:
		float m_x;
		float m_y;
		float m_z;

		Transform* m_parent;
		vector<Transform*> m_childs;

		RTTR_ENABLE(Component)
		RTTR_REGISTRATION_FRIEND
	};
}
