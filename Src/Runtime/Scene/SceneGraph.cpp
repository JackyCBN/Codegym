#include "SceneGraph.h"
#include "BaseClasses/GameObject.h"
#include "Component/Transform.h"

using namespace codegym::runtime;

SceneGraph::SceneGraph()
	:mRootNode(nullptr)
{
}

GameObject* SceneGraph::CreateGameObject(float x, float y, float z, const char* name) const
{
	auto go = new GameObject(name);
	auto trans = new Transform();
	trans->SetPosition(x, y, z);
	go->AddComponent(trans);

	return go;
}

void SceneGraph::Initlize()
{
	mRootNode = CreateGameObject(0, 0, 0, "root");
	auto parentNode = mRootNode->QueryComponent<Transform>();
	for(int i=1; i!=10; ++i)
	{
		for(int j=0; j!=i;++j)
		{
			auto newNode = CreateGameObject(i, i, i, (string("node") + to_string(i) + "_"+to_string(j)).c_str() );
			auto nodeTrans = newNode->QueryComponent<Transform>();
			nodeTrans->SetParent(parentNode);
		}
		if(parentNode->ChildCount()>0)
		{
			parentNode = parentNode->GetChild(parentNode->ChildCount() - 1);
		}
		
	}
}
