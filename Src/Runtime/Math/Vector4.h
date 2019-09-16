#pragma once

#include "Base.h"
#include "Type/Type.h"
#include "Logging/LogAssert.h"
#include "FloatConversion.h"
#include "Vector3.h"

namespace codegym::runtime
{
	struct Vector2;


	class CG_API Vector4
	{
	public:

		Vector4(): x(0), y(0),z(0),w(0)	{}
		Vector4(const Vector4& v) : x(v.x), y(v.y), z(v.z), w(v.w) {} 
		Vector4(float inX, float inY, float inZ, float inW) : x(inX), y(inY), z(inZ), w(inW) {}
		explicit Vector4(const Vector3& v, float inW) : x(v.x), y(v.y), z(v.z), w(inW) {}
		explicit Vector4(const float* v) : x(v[0]), y(v[1]), z(v[2]), w(v[3]) {}

		void Set(float inX, float inY, float inZ, float inW) { x = inX; y = inY; z = inZ; w = inW; }
		void Set(const float* array) { x = array[0]; y = array[1]; z = array[2]; w = array[3]; }
		void SetZero() { x = 0.0f; y = 0.0f; z = 0.0f; w = 0.0f; }

		float* GetPtr() { return &x; }
		const float* GetPtr() const { return &x; }

		float& operator[](int i) { DebugAssert(i >= 0 && i <= 3); return (&x)[i]; }
		const float& operator[](int i) const { DebugAssert(i >= 0 && i <= 3); return (&x)[i]; }

		bool operator==(const Vector4& v) const { return x == v.x && y == v.y && z == v.z && w == v.w; }
		bool operator!=(const Vector4& v) const { return x != v.x || y != v.y || z != v.z || w != v.w; }
		bool operator==(const float v[4]) const { return x == v[0] && y == v[1] && z == v[2] && w == v[3]; }
		bool operator!=(const float v[4]) const { return x != v[0] || y != v[1] || z != v[2] || w != v[3]; }

		Vector4 operator-() const { return Vector4(-x, -y, -z, -w); }

		float x;
		float y;
		float z;
		float w;

		static const float    infinity;
		static const Vector4 infinityVec;
		static const Vector4 zero;
		static const Vector4 one;

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND
	};

	inline Vector4 operator*(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z, lhs.w * rhs.w); }
	inline Vector4 operator*(const Vector4& inV, const float s) { return Vector4(inV.x * s, inV.y * s, inV.z * s, inV.w * s); }
	inline Vector4 operator+(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z, lhs.w + rhs.w); }
	inline Vector4 operator-(const Vector4& lhs, const Vector4& rhs) { return Vector4(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z, lhs.w - rhs.w); }
	inline float Dot(const Vector4& lhs, const Vector4& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z + lhs.w * rhs.w; }

	inline float SqrMagnitude(const Vector4& inV) { return Dot(inV, inV); }
	inline float Magnitude(const Vector4& inV) { return Sqrt(Dot(inV, inV)); }

	inline bool IsFinite(const Vector4& f)
	{
		return IsFinite(f.x) & IsFinite(f.y) & IsFinite(f.z) && IsFinite(f.w);
	}

	inline bool CompareApproximately(const Vector4& inV0, const Vector4& inV1, const float inMaxDist = kEPSILON)
	{
		return SqrMagnitude(inV1 - inV0) <= inMaxDist * inMaxDist;
	}

	inline Vector4 Lerp(const Vector4& from, const Vector4& to, float t) { return to * t + from * (1.0F - t); }
}
