#pragma once
namespace codegym::runtime
{
	class MathUtils
	{
	public:
		static float Clamp(float value, float min, float max)
		{
			if(value < min)
				value = min; 
			else if(value>max)
				value = max;
			return value;
		}

		static int Clamp(int value, int min, int max)
		{
			if (value < min)
				value = min;
			else if (value > max)
				value = max;
			return value;
		}

		static float Clamp01(float value)
		{
			if (value < 0.F)
				value = 0.F;
			else if (value > 1.F)
				value = 1.F;
			return value;
		}
		static float Sign(float f) { return f >= 0 ? 1.0f : -1.0f; }
	};
}