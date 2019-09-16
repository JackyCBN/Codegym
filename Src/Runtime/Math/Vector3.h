#pragma once

#include "Base.h"
#include "Type/Type.h"
#include "Logging/LogAssert.h"
#include "FloatConversion.h"

namespace codegym::runtime
{
	struct Vector2;

	struct CG_API Vector3
	{
		float x, y, z;

		static const Vector3 down;
		static const Vector3 left;
		static const Vector3 negativeInfinity;
		static const Vector3 one;
		static const Vector3 positiveInfinity;
		static const Vector3 right;
		static const Vector3 up;
		static const Vector3 zero;
		static const Vector3 forward;
		static const Vector3 back;
		static const Vector3 xAxis;
		static const Vector3 yAxis;
		static const Vector3 zAxis;
		
		Vector3():x(0),y(0),z(0){}
		Vector3(const float inX, const float inY, const float inZ) : x(inX), y(inY), z(inZ) {}
		explicit Vector3(const Vector2& v2);
		explicit Vector3(const float* ptr) { x = ptr[0]; y = ptr[1]; z = ptr[2]; }

		void Set(float inX, float inY, float inZ) { x = inX; y = inY; z = inZ; }
		void Set(const float* array) { x = array[0]; y = array[1]; z = array[2]; }
		void SetZero() { x = 0; y = 0; z = 0; }

		float* GetPtr() { return &x; }
		const float* GetPtr() const { return &x; }

		float& operator[](int i) { DebugAssert(i >= 0 && i <= 2); return (&x)[i]; }
		const float& operator[](int i) const { DebugAssert(i >= 0 && i <= 2); return (&x)[i]; }

		Vector3& operator += (const Vector3& rhs);
		Vector3& operator -= (const Vector3& rhs);
		Vector3& operator *= (float scale);
		Vector3& operator /= (float scale);		

		Vector3 operator / (float s) const { Vector3 temp(*this); temp /= s; return temp; }
		Vector3 operator-() const { return Vector3(-x, -y, -z); }

		Vector3& Scale(const Vector3& rhs) { x *= rhs.x; y *= rhs.y; z *= rhs.z; return *this; }
		Vector2 operator()()const;

		float SqrMagnitude() const { return x * x + y * y + z*z; }
		float Magnitude() const
		{
			return sqrtf(SqrMagnitude());
		}

		void Normalize() { const float mag = Magnitude();  x /= mag; y /= mag; z /= mag; }
		void SafeNormalize(const Vector3& defaultV = Vector3::zero)
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

		Vector3 Normalized() const
		{
			Vector3 temp(x, y,z);
			temp.Normalize();
			return temp;
		}

		Vector3 SafeNormalized(const Vector3& defaultV = Vector3::zero) const
		{
			Vector3 temp(x, y,z);
			temp.SafeNormalize(defaultV);
			return temp;
		}

		bool CheckFinite() const { return IsFinite(x) || IsFinite(y) || IsFinite(z); }

		bool IsNormalized(float epsilon = kEPSILON)
		{
			return CompareApproximately(SqrMagnitude(), 1.0F, epsilon);
		}

		string ToString() const;

		RTTR_ENABLE()
		RTTR_REGISTRATION_FRIEND

			// static
	public:
		static Vector3 Inverse(const Vector3& inVec);
		static float	Angle(const Vector3& from, const Vector3& to);
		static float  SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis);
		static Vector3 ClampMagnitude(Vector3 vector, float maxLength);
		static float Distance(Vector3 a, Vector3 b);
		static Vector3 Lerp(Vector3 a, Vector3 b, float t);
		static Vector3 LerpUnclamped(Vector3 a, Vector3 b, float t);
		static Vector3 Max(Vector3 lhs, Vector3 rhs);
		static Vector3 Min(Vector3 lhs, Vector3 rhs);
		static Vector3 MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta);
		static Vector3 Perpendicular(Vector3 inDirection);
		static Vector3 Reflect(Vector3 inDirection, Vector3 inNormal);
		static Vector3 SmoothDamp(Vector3 current, Vector3 target, Vector3& currentVelocity, float smoothTime, float maxSpeed, float deltaTime);
	
	};
	CG_INLINE	bool operator==(const Vector3& lhs, const Vector3& rhs) { return lhs.x == rhs.x && lhs.y == rhs.y && lhs.z == rhs.z; }
	CG_INLINE	bool operator!=(const Vector3& lhs, const Vector3& rhs) { return lhs.x != rhs.x || lhs.y != rhs.y || lhs.z != rhs.z; }
	CG_INLINE	Vector3 operator - (const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x - rhs.x, lhs.y - rhs.y, lhs.z - rhs.z); }
	CG_INLINE	Vector3 operator + (const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x + rhs.x, lhs.y + rhs.y, lhs.z + rhs.z); }
	CG_INLINE	Vector3 operator * (const Vector3& lhs, float s) { return Vector3(lhs.x * s, lhs.y * s, lhs.z * s); }
	CG_INLINE	Vector3 operator * (float s, const Vector3& rhs) { return Vector3(rhs.x * s, rhs.y * s, rhs.z * s); }
	CG_INLINE   Vector3 operator * (const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x * rhs.y, lhs.y * rhs.y, lhs.z*rhs.z); }
	
	CG_INLINE	Vector3 Scale(const Vector3& lhs, const Vector3& rhs) { return Vector3(lhs.x * rhs.x, lhs.y * rhs.y, lhs.z * rhs.z); }
	CG_INLINE	float Dot(const Vector3& lhs, const Vector3& rhs) { return lhs.x * rhs.x + lhs.y * rhs.y + lhs.z * rhs.z; }
	CG_INLINE Vector3 Cross(const Vector3& lhs, const Vector3& rhs)
	{
		return Vector3(
			lhs.y * rhs.z - lhs.z * rhs.y, 
			lhs.z * rhs.x - lhs.x * rhs.z,
			lhs.x * rhs.y - lhs.y * rhs.x);
	}
	CG_INLINE float Volume(const Vector3& inV) { return inV.x * inV.y * inV.z; }
	CG_INLINE Vector3 Abs(const Vector3& v) { return Vector3(Abs(v.x), Abs(v.y), Abs(v.z)); }
	/// Project one vector onto another.
	CG_INLINE Vector3 Project(const Vector3& v1, const Vector3& v2)
	{
		float sqrMag = Dot(v2, v2);
		if(sqrMag< kEpsilonNormalSqrt)
		{
			return Vector3::zero;
		}
		return v2 * Dot(v1, v2) / sqrMag;
	}
	
	// Projects a vector onto a plane defined by a normal orthogonal to the plane.
	CG_INLINE Vector3 ProjectOnPlane(Vector3 vector, Vector3 planeNormal)
	{
		return vector - Project(vector, planeNormal);
	}

	// Rotates lhs towards rhs by no more than max Angle
	// Moves the magnitude of lhs towards rhs by no more than maxMagnitude
	Vector3 RotateTowards(const Vector3& lhs, const Vector3& rhs, float maxAngle, float maxMagnitude);
	Vector3 Slerp(const Vector3& from, const Vector3& to, float t);
	bool CompareApproximately(const Vector3& inV0, const Vector3& inV1, const float inMaxDist = kEpsilonNormalSqrt);
	// Orthonormalizes the three vectors, assuming that a orthonormal basis can be formed
	void OrthoNormalizeFast(Vector3* inU, Vector3* inV, Vector3* inW);
	// Orthonormalizes the three vectors, returns false if no orthonormal basis could be formed.
	void OrthoNormalize(Vector3* inU, Vector3* inV, Vector3* inW);
	// Orthonormalizes the two vectors. inV is taken as a hint and will try to be as close as possible to inV.
	void OrthoNormalize(Vector3* inU, Vector3* inV);

	// Calculates a vector that is orthonormal to n.
	// Assumes that n is normalized
	Vector3 OrthoNormalVectorFast(const Vector3& n);
	
	CG_INLINE Vector3 Round(const Vector3& a, float factor)
	{
		return Vector3(Roundf(a.x / factor) * factor, Roundf(a.y / factor) * factor, Roundf(a.z / factor) * factor);
	}
}
