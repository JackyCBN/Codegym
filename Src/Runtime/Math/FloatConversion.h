#pragma once
#include "Platform/PlatformEnvironment.h"
namespace codegym::runtime
{
	const float kEPSILON = 0.00001F;
	const float kEpsilonNormalSqrt = 1e-15f;
	const float k1OverSqrt2 = (0.7071067811865475244008443621048490F);
	const float kBiggestFloatSmallerThanOne = 0.99999994f;
	const double kBiggestDoubleSmallerThanOne = 0.99999999999999989;
	
	const float FLOAT_INFINITY = numeric_limits<float>::infinity();
	const double DOUBLE_INFINITY = numeric_limits<double>::infinity();

	inline float Abs(float v)
	{
		return v < 0.0F ? -v : v;
	}
	inline double Abs(double v)
	{
		return v < 0.0F ? -v : v;
	}

	inline int Abs(int v)
	{
		return v < 0 ? -v : v;
	}

	// Returns true if the distance between f0 and f1 is smaller than epsilon
	inline bool CompareApproximately(float f0, float f1, float epsilon = kEPSILON)
	{
		float dist = f0 - f1;
		dist = Abs(dist);

		return  dist <= epsilon;
	}

	inline bool CompareApproximatelyD(double d0, double d1, double epsilon = kEPSILON)
	{
		double dist = (d0 - d1);
		dist = Abs(dist);
		return dist <= epsilon;
	}

	inline bool IsFinite(const float value)
	{
		return isfinite(value);
	}

	inline float InvSqrt(float p) { return 1.0F / sqrt(p); }
	inline float Sqrt(float p) { return sqrt(p); }


	inline int FloorfToInt(float f)
	{
		DebugAssert(f >= INT_MIN && f <= INT_MAX);
		return f >= 0 ? (int)f : (int)(f - kBiggestFloatSmallerThanOne);
	}

	inline UInt32 FloorfToIntPos(float f)
	{
		DebugAssert(f >= 0 && f <= UINT_MAX);
		return (UInt32)f;
	}

	inline float Floorf(float f)
	{
		// Use std::floor().
		// We are interested in reliable functions that do not lose precision.
		// Casting to int and back to float would not be helpful.
		return floor(f);
	}

	inline double Floord(double f)
	{
		// Use std::floor().
		// We are interested in reliable functions that do not lose precision.
		// Casting to int and back to float would not be helpful.
		return floor(f);
	}

	inline float Ceilf(float f)
	{
		// Use std::ceil().
		// We are interested in reliable functions that do not lose precision.
		// Casting to int and back to float would not be helpful.
		return ceil(f);
	}

	inline double Ceild(double f)
	{
		// Use std::ceil().
		// We are interested in reliable functions that do not lose precision.
		// Casting to int and back to float would not be helpful.
		return ceil(f);
	}

	inline int CeilfToInt(float f)
	{
		DebugAssert(f >= INT_MIN && f <= INT_MAX);
		return f >= 0 ? (int)(f + kBiggestFloatSmallerThanOne) : (int)(f);
	}

	inline uint32_t CeilfToIntPos(float f)
	{
		DebugAssert(f >= 0 && f <= UINT_MAX);
		return (uint32_t)(f + kBiggestFloatSmallerThanOne);
	}
	inline int RoundfToInt(float f)
	{
		return FloorfToInt(f + 0.5F);
	}

	inline uint32_t RoundfToIntPos(float f)
	{
		return FloorfToIntPos(f + 0.5F);
	}

	inline float Roundf(float f)
	{
		return Floorf(f + 0.5F);
	}

	inline double Roundf(double f)
	{
		return Floord(f + 0.5);
	}

}
