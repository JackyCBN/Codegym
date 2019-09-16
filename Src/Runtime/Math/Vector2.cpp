#include "Vector2.h"
#include "FloatConversion.h"
#include "Utilities/Utility.h"
#include "MathUtils.h"
#include "Vector3.h"

using namespace codegym::runtime;

const Vector2 Vector2::down = Vector2(0, -1);
const Vector2 Vector2::left = Vector2(-1, 0);
const Vector2 Vector2::negativeInfinity = Vector2(-numeric_limits<float>::infinity(), -numeric_limits<float>::infinity());
const Vector2 Vector2::one = Vector2(1, 1);
const Vector2 Vector2::positiveInfinity = Vector2(numeric_limits<float>::infinity(), numeric_limits<float>::infinity());
const Vector2 Vector2::right = Vector2(1, 0);
const Vector2 Vector2::up = Vector2(0, 1);
const Vector2 Vector2::zero = Vector2(0, 0);


Vector2::Vector2(const Vector3& v3)
	:x(v3.x)
	,y(v3.y)
{
	
}

Vector2& Vector2::operator+=(const Vector2& rhs)
{
	x += rhs.x;
	y += rhs.y;
	return *this;
}

Vector2& Vector2::operator-=(const Vector2& rhs)
{
	x -= rhs.x;
	y -= rhs.y;
	return *this;
}

Vector2& Vector2::operator*=(float scale)
{
	x *= scale;
	y *= scale;
	return *this;
}

Vector2& Vector2::operator/=(float scale)
{
	DebugAssert(!CompareApproximately(scale, 0));

	x /= scale;
	y /= scale;
	return *this;
}

Vector3 Vector2::operator()() const
{
	return Vector3(x, y, 0);
}

string Vector2::ToString() const
{
	stringstream ss;
	ss << "( " << x << ", " << y << ")";

	return ss.str();
}

float Vector2::Angle(const Vector2& from, const Vector2& to)
{
	float denominator = sqrtf(from.SqrMagnitude() * to.SqrMagnitude());
	if (denominator < kEPSILON)
	{
		return 0;
	}
	float dot = Dot(from,to) / denominator;
	dot = MathUtils::Clamp(dot, -1.0f, 1.0f);
	return acos(dot);
}

float Vector2::SignedAngle(const Vector2& from, const Vector2& to)
{
	const float  unsigned_angle = Angle(from, to);
	// cross mul to check sign
	const float  sign = MathUtils::Sign(from.x*to.y - from.y*to.x);

	return unsigned_angle * sign;
}

Vector2 Vector2::ClampMagnitude(Vector2 vector, float maxLength)
{
	if(vector.SqrMagnitude() > maxLength * maxLength)
	{
		return vector.SafeNormalized() * maxLength;
	}

	return vector;
}

float Vector2::Distance(Vector2 a, Vector2 b)
{
	return (a - b).Magnitude();
}

Vector2 Vector2::Lerp(Vector2 a, Vector2 b, float t)
{
	t = clamp01(t);
	return a + (b - a) * t;
}

Vector2 Vector2::LerpUnclamped(Vector2 a, Vector2 b, float t)
{
	return a + (b - a) * t;
}

Vector2 Vector2::Max(Vector2 lhs, Vector2 rhs)
{
	return Vector2(max(lhs.x, rhs.x), max(lhs.y, rhs.y));
}

Vector2 Vector2::Min(Vector2 lhs, Vector2 rhs)
{
	return Vector2(min(lhs.x, rhs.x), min(lhs.y, rhs.y));
}

Vector2 Vector2::MoveTowards(Vector2 current, Vector2 target, float maxDistanceDelta)
{
	return current + ClampMagnitude(target - current, maxDistanceDelta);
}

Vector2 Vector2::Perpendicular(Vector2 inDirection)
{
	return Vector2(-inDirection.y, inDirection.x);
}

Vector2 Vector2::Reflect(Vector2 inDirection, Vector2 inNormal)
{
	return -2.0F * Dot(inDirection, inNormal) * inNormal + inDirection;
}

Vector2 Vector2::SmoothDamp(Vector2 current, Vector2 target, Vector2& currentVelocity, float smoothTime, float maxSpeed,
	float deltaTime)
{
	// Based on Game Programming Gems 4 Chapter 1.10
	smoothTime = max(0.0001F, smoothTime);
	float omega = 2.F / smoothTime;

	float x = omega * deltaTime;
	float exp = 1.F / (1.F + x + 0.48F * x * x + 0.235F * x * x * x);
	Vector2 change = current - target;
	Vector2 originalTo = target;

	// Clamp maximum speed
	float maxChange = maxSpeed * smoothTime;
	change = ClampMagnitude(change, maxChange);
	target = current - change;

	Vector2 temp = (currentVelocity + omega * change) * deltaTime;
	currentVelocity = (currentVelocity - omega * temp) * exp;
	Vector2 output = target + (change + temp) * exp;

	// Prevent overshooting
	if (::Dot(originalTo - current, output - originalTo) > 0)
	{
		output = originalTo;
		currentVelocity = (output - current) / deltaTime;
	}

	return output;
}


