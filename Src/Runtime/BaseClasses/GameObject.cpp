#include "GameObject.h"

using namespace codegym::runtime;
GameObject::GameObject()
	:GameObject("GameObject")
{

}

GameObject::GameObject(const char* name)
	:mName(name)
{

}

const char* GameObject::Name()
{
	return mName.c_str();
}

void GameObject::SetName(const char* name)
{
	mName = name;
}

void GameObject::AddComponent(Component* comp)
{
	mComponents.emplace_back(comp);
	comp->SetGameObject(this);
}

void GameObject::RemoveComponent(Component* comp)
{
	const auto it = find(mComponents.begin(), mComponents.end(), comp);
	if(it != mComponents.end())
	{
		mComponents.erase(it);
	}
}

void GameObject::RemoveComponentAt(int index)
{
	auto it = mComponents.begin();
	advance(it, index);
	mComponents.erase(it);
}

Component* GameObject::QueryComponentByType(const rttr::type& type) const
{
	for (auto& comp : mComponents)
	{
		if( type.is_base_of(comp->get_type()))
		{
			return comp;
		}
	}

	return nullptr;
}

Component* GameObject::QueryComponentByExtraType(const rttr::type& type) const
{
	for (auto& comp : mComponents)
	{
		if (type == comp->get_type())
		{
			return comp;
		}
	}

	return nullptr;
}

rttr::type GameObject::GetComponentTypeAtIndex(int index) const
{
	return mComponents[index]->get_type();
}

Component& GameObject::GetComponentAtIndex(int index) const
{
	Component* comp = QueryComponentAtIndex(index);
	DebugAssert(comp != nullptr);
	return *comp;
}

Component* GameObject::QueryComponentAtIndex(int index) const
{
	Component* comp = mComponents[index];
	DebugAssert(comp != nullptr);

	return comp;
}

RTTR_REGISTRATION
{
	using namespace rttr;
		registration::class_<GameObject>("GameObject")
			.property("Name", &GameObject::mName);
}