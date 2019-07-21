#include "Base.h"
#include "EditorHelper.h"

static vector<StartUpFunc*> gStartups;
void AddStartup(StartUpFunc* func)
{
	gStartups.push_back(func);
}

void ExecuteStartups()
{
	for (auto func : gStartups)
	{
		if(func)
		{
			func();
		}
	}
}
