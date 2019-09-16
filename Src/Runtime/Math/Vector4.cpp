#include "Vector4.h"
namespace codegym::runtime
{
	const float     Vector4::infinity = std::numeric_limits<float>::infinity();
	const Vector4  Vector4::infinityVec = Vector4(std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity(), std::numeric_limits<float>::infinity());

	const Vector4  Vector4::zero = Vector4(0, 0, 0, 0);
	const Vector4  Vector4::one = Vector4(1, 1, 1, 1);
}
