#include "Transform.h"

using namespace codegym::runtime;


Transform::Transform()
	:m_x(0),
	m_y(0),
	m_z(0),
	m_parent(nullptr)
{

}

Transform* Transform::GetChild(int index) const
{
	return m_childs[index];
}

void Transform::RemoveChild(Transform* child)
{
	m_childs.erase(find(m_childs.begin(), m_childs.end(), child));
}

void Transform::AddChild(Transform* child)
{
	m_childs.push_back(child);
}

void Transform::SetParent(Transform* parent)
{
	Transform* previousParent = m_parent;
	if(previousParent !=nullptr)
	{
		previousParent->RemoveChild(this);
	}

	m_parent = parent;
	m_parent->AddChild(this);
}

void Transform::SetPosition(float x, float y, float z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}


RTTR_REGISTRATION
{
	using namespace rttr;
	registration::class_<Transform>("codegym::runtime::Transform")
		.property("x", &Transform::m_x)
		.property("y", &Transform::m_y)
		.property("z", &Transform::m_z);
}