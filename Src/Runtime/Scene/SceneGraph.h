#pragma once

#include "Base.h"

namespace codegym::runtime
{
	class GameObject;
	class CG_API SceneGraph
	{
	public:
		SceneGraph();
		GameObject* CreateGameObject(float x, float y, float z, const char* name) const;
		GameObject* Root() const { return  mRootNode; }

		void Initlize();
	private:
		GameObject* mRootNode;
	};

}
