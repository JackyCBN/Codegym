#pragma once

#include "Base.h"
#include "BaseClasses/Type.h"
#include "Component.h"
#include "Logging/LogAssert.h"


namespace codegym::runtime
{
	class Component;

	class CG_API GameObject
	{
	public:
		GameObject();
		explicit GameObject(const char* name);

		const char* Name();
		void		SetName(const char* name);

		// Add
		void AddComponent(Component* comp);

		// remove
		void RemoveComponent(Component* comp);
		void RemoveComponentAt(int index);

		// Get & Query
		template<class T>
		T& GetComponent() const;

		template<class T>
		T* QueryComponent() const;

		Component* QueryComponentByType(const rttr::type& type) const;
		Component* QueryComponentByExtraType(const rttr::type& type) const;

		rttr::type GetComponentTypeAtIndex(int index) const;
		Component& GetComponentAtIndex(int index) const;
		Component* QueryComponentAtIndex(int index) const;

		template<class T> T& GetComponentAtIndex(int index) const;
		template<class T> T* QueryComponentAtIndex(int index) const;

		int GetComponentCount() const
		{
			return static_cast<int>(mComponents.size());
		}

	private:
		string mName;

		vector<Component*> mComponents;

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND

	};

	template <class T>
	T& GameObject::GetComponent() const
	{
		T* component = QueryComponentByType(TypeOf<T>());
		DebugAssertMsg(component != nullptr, "GetComponent returned NULL. You cannot use GetComponent unless the component is guaranteed to be part of the GO");
		return *component;
	}

	template <class T>
	T* GameObject::QueryComponent() const
	{
		return static_cast<T*>(QueryComponentByType(TypeOf<T>()));
	}

	template <class T>
	T& GameObject::GetComponentAtIndex(int index) const
	{
		T*  comp = QueryComponentAtIndex<T>(index);
		DebugAssert(comp != nullptr);
		return comp;
	}

	template <class T>
	T* GameObject::QueryComponentAtIndex(int index) const
	{
		Component* comp = mComponents[index];
		DebugAssert(comp != nullptr);

		return static_cast<T*>(comp);

	}
}
