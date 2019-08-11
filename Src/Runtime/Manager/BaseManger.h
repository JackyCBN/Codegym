#pragma once
#include "Base.h"
namespace codegym::runtime
{
	class BaseManger
	{
	public:
		virtual ~BaseManger();
		
		virtual void Initlize() = 0;
		virtual void Deinitlize() = 0;
	};
}