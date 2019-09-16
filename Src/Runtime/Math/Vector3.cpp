#include "Vector3.h"
#include "FloatConversion.h"
#include "Utilities/Utility.h"
#include "MathUtils.h"
#include "Vector2.h"

namespace codegym::runtime
{

	const Vector3 Vector3::down = Vector3(0, -1, 0);
	const Vector3 Vector3::left = Vector3(-1, 0, 0);
	const Vector3 Vector3::negativeInfinity = Vector3(-FLOAT_INFINITY, -FLOAT_INFINITY, -FLOAT_INFINITY);
	const Vector3 Vector3::one = Vector3(1, 1, 1);
	const Vector3 Vector3::positiveInfinity = Vector3(FLOAT_INFINITY, FLOAT_INFINITY, FLOAT_INFINITY);
	const Vector3 Vector3::right = Vector3(1, 0, 0);
	const Vector3 Vector3::up = Vector3(0, 1, 0);
	const Vector3 Vector3::zero = Vector3(0, 0, 0);
	const Vector3 Vector3::forward = Vector3(0, 0, 1);
	const Vector3 Vector3::back = Vector3(0, 0, -1);
	const Vector3 Vector3::xAxis = Vector3::right;
	const Vector3 Vector3::yAxis = Vector3::up;
	const Vector3 Vector3::zAxis = Vector3::forward;


	Vector3::Vector3(const Vector2& v2)
		:x(v2.x)
		, y(v2.y)
		, z(0)
	{

	}

	Vector3& Vector3::operator+=(const Vector3& rhs)
	{
		x += rhs.x;
		y += rhs.y;
		z += rhs.z;
		return *this;
	}

	Vector3& Vector3::operator-=(const Vector3& rhs)
	{
		x -= rhs.x;
		y -= rhs.y;
		z -= rhs.z;
		return *this;
	}

	Vector3& Vector3::operator*=(float scale)
	{
		x *= scale;
		y *= scale;
		z *= scale;
		return *this;
	}

	Vector3& Vector3::operator/=(float scale)
	{
		DebugAssert(!CompareApproximately(scale, 0));

		x /= scale;
		y /= scale;
		z /= scale;
		return *this;
	}

	Vector2 Vector3::operator()() const
	{
		return Vector2(x, y);
	}

	string Vector3::ToString() const
	{
		stringstream ss;
		ss << "( " << x << ", " << y << ", " << z << ")";

		return ss.str();
	}

	Vector3 Vector3::Inverse(const Vector3& inVec)
	{
		return Vector3(1.0F / inVec.x, 1.0F / inVec.y, 1.0F / inVec.z);
	}

	float Vector3::Angle(const Vector3& from, const Vector3& to)
	{
		float denominator = sqrtf(from.SqrMagnitude() * to.SqrMagnitude());
		if (denominator < kEPSILON)
		{
			return 0;
		}
		float dot = Dot(from, to) / denominator;
		dot = MathUtils::Clamp(dot, -1.0f, 1.0f);
		return acos(dot);
	}

	float Vector3::SignedAngle(const Vector3& from, const Vector3& to, const Vector3& axis)
	{
		float  unsigned_angle = Angle(from, to);
		const float  sign = MathUtils::Sign(Dot(axis, Cross(from, to)));

		return unsigned_angle * sign;
	}

	Vector3 Vector3::ClampMagnitude(Vector3 vector, float maxLength)
	{
		if (vector.SqrMagnitude() > maxLength * maxLength)
		{
			return vector.SafeNormalized() * maxLength;
		}

		return vector;
	}

	float Vector3::Distance(Vector3 a, Vector3 b)
	{
		return (a - b).Magnitude();
	}

	Vector3 Vector3::Lerp(Vector3 a, Vector3 b, float t)
	{
		t = clamp01(t);
		return a + (b - a) * t;
	}

	Vector3 Vector3::LerpUnclamped(Vector3 a, Vector3 b, float t)
	{
		return a + (b - a) * t;
	}

	Vector3 Vector3::Max(Vector3 lhs, Vector3 rhs)
	{
		return Vector3(max(lhs.x, rhs.x), max(lhs.y, rhs.y), max(lhs.z, rhs.z));
	}

	Vector3 Vector3::Min(Vector3 lhs, Vector3 rhs)
	{
		return Vector3(min(lhs.x, rhs.x), min(lhs.y, rhs.y), min(lhs.z, rhs.z));
	}

	Vector3 Vector3::MoveTowards(Vector3 current, Vector3 target, float maxDistanceDelta)
	{
		return current + ClampMagnitude(target - current, maxDistanceDelta);
	}

	Vector3 Vector3::Perpendicular(Vector3 inDirection)
	{
		return Vector3(-inDirection.y, inDirection.x, 0);
	}

	Vector3 Vector3::Reflect(Vector3 inDirection, Vector3 inNormal)
	{
		return -2.0F * Dot(inDirection, inNormal) * inNormal + inDirection;
	}

	Vector3 Vector3::SmoothDamp(Vector3 current, Vector3 target, Vector3& currentVelocity, float smoothTime, float maxSpeed,
		float deltaTime)
	{
		// Based on Game Programming Gems 4 Chapter 1.10
		smoothTime = max(0.0001F, smoothTime);
		float omega = 2.F / smoothTime;

		float x = omega * deltaTime;
		float exp = 1.F / (1.F + x + 0.48F * x * x + 0.235F * x * x * x);
		Vector3 change = current - target;
		Vector3 originalTo = target;

		// Clamp maximum speed
		float maxChange = maxSpeed * smoothTime;
		change = ClampMagnitude(change, maxChange);
		target = current - change;

		Vector3 temp = (currentVelocity + omega * change) * deltaTime;
		currentVelocity = (currentVelocity - omega * temp) * exp;
		Vector3 output = target + (change + temp) * exp;

		// Prevent overshooting
		if (Dot(originalTo - current, output - originalTo) > 0)
		{
			output = originalTo;
			currentVelocity = (output - current) / deltaTime;
		}

		return output;
	}


	Vector3 RotateTowards(const Vector3& lhs, const Vector3& rhs, float maxAngle, float maxMagnitude)
	{
		// ???
		return  Vector3::zero;
	}

	Vector3 Slerp(const Vector3& from, const Vector3& to, float t)
	{
		return Vector3::zero;
	}

	bool CompareApproximately(const Vector3& inV0, const Vector3& inV1, const float inMaxDist)
	{
		return (inV0 - inV1).SqrMagnitude() < inMaxDist;
	}


	void OrthoNormalizeFast(Vector3* inU, Vector3* inV, Vector3* inW)
	{
		// compute u0
		inU->Normalize();

		// compute u1
		float dot0 = Dot(*inV, *inU);

		*inV = *inV - *inU * dot0;
		inV->Normalize();

		// compute u2
		float dot1 = Dot(*inV, *inW);
		dot0 = Dot(*inU, *inW);
		*inW = *inW - dot1 * *inV - dot0 * *inU;
		inW->Normalize();
	}


	void OrthoNormalize(Vector3* inU, Vector3* inV, Vector3* inW)
	{
		// compute u0
		float mag = inU->Magnitude();
		if (mag > kEPSILON)
			* inU /= mag;
		else
			*inU = Vector3(1, 0, 0);

		// compute u1
		float dot0 = Dot(*inV, *inU);
		*inV = *inV - *inU * dot0;
		mag = inV->Magnitude();
		if (mag > kEPSILON)
			* inV /= mag;
		else
			*inV = OrthoNormalVectorFast(*inU);

		float dot1 = Dot(*inV, *inW);
		dot0 = Dot(*inU, *inW);
		*inW = *inW - dot1 * *inV - dot0 * *inU;
		mag = inW->Magnitude();
		if (mag > kEPSILON)
			* inW /= mag;
		else
			*inW = Cross(*inU, *inV);

	}
	void OrthoNormalize(Vector3* inU, Vector3* inV)
	{
		// compute u0
		float mag = inU->Magnitude();
		if (mag > kEPSILON)
			* inU /= mag;
		else
			*inU = Vector3(1, 0, 0);

		// compute u1
		float dot0 = Dot(*inV, *inU);
		*inV = *inV - *inU * dot0;
		mag = inV->Magnitude();
		if (mag > kEPSILON)
			* inV /= mag;
		else
			*inV = OrthoNormalVectorFast(*inU);
	}

	Vector3 OrthoNormalVectorFast(const Vector3& n)
	{
		// now n is normaled. so n.x*n.x + n.y+n.y + n.z*n.z = 1;
		// we want set one axis that not biggest axis to zero
		Vector3 res;
		if(Abs(n.z) > k1OverSqrt2)
		{
			// choose p in y-z plane
			float a = n.y * n.y + n.z * n.z;
			float k = 1.0F / Sqrt(a);
			res.x = 0;
			res.y = -n.z * k;
			res.z = n.y * k;
		}
		else
		{
			// choose p in x-y plane
			float a = n.x * n.x + n.y * n.y;
			float k = 1.0F / Sqrt(a);
			res.x = -n.y * k;
			res.y = n.x * k;
			res.z = 0;			
		}

		return res;
		
	}
}
