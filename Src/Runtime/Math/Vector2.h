#pragma once

#include "Base.h"
#include "Type/Type.h"
#include "Logging/LogAssert.h"
#include "FloatConversion.h"

namespace codegym::runtime
{
	struct Vector3;

	struct CG_API Vector2
	{
		float x, y;
		
		static const Vector2 down;
		static const Vector2 left;
		static const Vector2 negativeInfinity;
		static const Vector2 one;
		static const Vector2 positiveInfinity;
		static const Vector2 right;
		static const Vector2 up;
		static const Vector2 zero;

		Vector2():x(0), y(0){}
		Vector2(const float inX, const float inY) : x(inX), y(inY){}
		explicit Vector2(const Vector3& v3);
		explicit Vector2(const float* ptr) { x = ptr[0]; y = ptr[1]; }

		void Set(float inX, float inY) { x = inX; y = inY; }
		void SetZero() { x = 0; y = 0; }

		float* GetPtr() { return &x; }
		const float* GetPtr() const { return &x; }

		float& operator[](int i) { DebugAssert(i >= 0 && i <= 1); return (&x)[i]; }
		const float& operator[](int i) const { DebugAssert(i >= 0 && i <= 1); return (&x)[i]; }

		Vector2& operator += (const Vector2& rhs);
		Vector2& operator -= (const Vector2& rhs);
		Vector2& operator *= (float scale);
		Vector2& operator /= (float scale);
		
		Vector2 operator / (float s) const { Vector2 temp(*this); temp /= s; return temp; }

		Vector2 operator-() const { return Vector2(-x, -y); }
		Vector3 operator()() const;

		Vector2& Scale(const Vector2& rhs) { x *= rhs.x; y *= rhs.y; return *this; }

		float SqrMagnitude() const { return x * x + y * y; }
		float Magnitude() const
		{
			return sqrtf(SqrMagnitude());
		}

		void Normalize() {	const float mag = Magnitude();  x /= mag; y /= mag; }
		void SafeNormalize(const Vector2& defaultV = Vector2::zero)
		{
			const float mag = Magnitude();
			if (mag > kEPSILON)
			{
				Normalize();
			}
			else
			{
				*this = defaultV;
			}
		}
		
		Vector2 Normalized() const
		{
			Vector2 temp(x,y);
			temp.Normalize();
			return temp;
		}
		
		Vector2 SafeNormalized(const Vector2& defaultV = Vector2::zero) const
		{
			Vector2 temp(x, y);
			temp.SafeNormalize(defaultV);
			return temp;
		}

		bool CheckFinite() const { return IsFinite(x) || IsFinite(y); }

		bool IsNormalized(float epsilon = kEPSILON)
		{
			return CompareApproximately(SqrMagnitude(), 1.0F, epsilon);
		}

		string ToString() const;
		
		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND

		// static
	public:
		static  float Angle(const Vector2& from, const Vector2& to);
		static float  SignedAngle(const Vector2& from, const Vector2& to);
		static Vector2 ClampMagnitude(Vector2 vector, float maxLength);
		static float Distance(Vector2 a, Vector2 b);
		static Vector2 Lerp(Vector2 a, Vector2 b, float t);
		static Vector2 LerpUnclamped(Vector2 a, Vector2 b, float t);
		static Vector2 Max(Vector2 lhs, Vector2 rhs);
		static Vector2 Min(Vector2 lhs, Vector2 rhs);
		static Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta);
		static Vector2 Perpendicular(Vector2 inDirection);
		static Vector2 Reflect(Vector2 inDirection, Vector2 inNormal);
		static Vector2 SmoothDamp(Vector2 current, Vector2 target, Vector2& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
	};
	CG_INLINE bool operator==(const Vector2& lhs, const Vector2& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y; }
	CG_INLINE bool operator!=(const Vector2& lhs, const Vector2& rhs) { return lhs.x != rhs.x || lhs.y != rhs.y; }
	CG_INLINE  Vector2 operator - (const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.x - rhs.x, lhs.y - rhs.y); }
	CG_INLINE  Vector2 operator + (const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.x + rhs.x, lhs.y + rhs.y); }
	CG_INLINE  Vector2 operator * (const Vector2& lhs, float s){ return Vector2(lhs.x * s, lhs.y * s);}
	CG_INLINE  Vector2 operator * (float s, const Vector2& rhs){ return Vector2(rhs.x * s, rhs.y * s);}
	CG_INLINE  Vector2 operator * (const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.x * rhs.y, lhs.y * rhs.y); }

	CG_INLINE  Vector2 Scale(const Vector2& lhs, const Vector2& rhs) { return Vector2(lhs.x * rhs.x, lhs.y * rhs.y); }
	CG_INLINE  float Dot(const Vector2& lhs, const Vector2& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y; }
	
}
