#pragma once
//
//#include "Runtime/Allocator/MemoryMacros.h"
//#include "Runtime/Math/Simd/config.h"
//
//#define SAFE_RELEASE(obj) { if (obj) { (obj)->Release(); (obj) = NULL; } else {} }
//
//template<class DataType>
//inline bool ArrayEquals(const DataType* lhs, const DataType* rhs, size_t arraySize)
//{
//    for (size_t i = 0; i < arraySize; i++)
//    {
//        if (lhs[i] != rhs[i])
//            return false;
//    }
//    return true;
//}
//
//template<class DataType>
//inline int ArrayCompare(const DataType* lhs, const DataType* rhs, size_t arraySize)
//{
//    // Compares based on the endianness of the platform, *not* lexicographical order like e.g. memcmp()
//#if PLATFORM_ARCH_LITTLE_ENDIAN
//    // Higher memory addresses are more significant, compare back to front
//    for (size_t i = arraySize - 1;; i--)
//    {
//        if (lhs[i] != rhs[i])
//            return lhs[i] < rhs[i] ? -1 : 1;
//        if (i == 0)
//            break;
//    }
//#else
//    // Lower memory addresses are more significant
//    for (size_t i = 0; i < arraySize; i++)
//    {
//        if (lhs[i] != rhs[i])
//            return lhs[i] < rhs[i] ? -1 : 1;
//    }
//#endif
//    return 0;
//}
//
//template<class DataType>
//inline bool ArrayLessThan(const DataType* lhs, const DataType* rhs, size_t arraySize)
//{
//    return ArrayCompare(lhs, rhs, arraySize) < 0;
//}
//
//template<class DataType>
//inline bool ArrayGreaterThan(const DataType* lhs, const DataType* rhs, size_t arraySize)
//{
//    return ArrayCompare(lhs, rhs, arraySize) > 0;
//}
//
//// Check if it's safe to access a type using given alignment
//#define IS_SAFE_ALIGNMENT_FOR_TYPE(Alignment, Type) ((Alignment) >= alignof(Type) || !UNITY_NO_UNALIGNED_MEMORY_ACCESS)
//
//// Compare memory by casting to largest possible integer type. ElementSize and Alignment should be compile time constants.
//#define COMPARE_FIXED_SIZE_AND_ALIGNMENT(CompareFunc, ElementSize, Alignment, lhs, rhs, elementCount) \
//    if (PLATFORM_ARCH_64 && IS_SAFE_ALIGNMENT_FOR_TYPE(Alignment, UInt64) && ((ElementSize) % sizeof(UInt64)) == 0) \
//        return CompareFunc(reinterpret_cast<const UInt64*>(lhs), reinterpret_cast<const UInt64*>(rhs), (ElementSize) * (elementCount) / sizeof(UInt64)); \
//    else if (IS_SAFE_ALIGNMENT_FOR_TYPE(Alignment, UInt32) && ((ElementSize) % sizeof(UInt32)) == 0) \
//        return CompareFunc(reinterpret_cast<const UInt32*>(lhs), reinterpret_cast<const UInt32*>(rhs), (ElementSize) * (elementCount) / sizeof(UInt32)); \
//    else if (IS_SAFE_ALIGNMENT_FOR_TYPE(Alignment, UInt16) && ((ElementSize) % sizeof(UInt16)) == 0) \
//        return CompareFunc(reinterpret_cast<const UInt16*>(lhs), reinterpret_cast<const UInt16*>(rhs), (ElementSize) * (elementCount) / sizeof(UInt16)); \
//    else \
//        return CompareFunc(reinterpret_cast<const UInt8*>(lhs), reinterpret_cast<const UInt8*>(rhs), (ElementSize) * (elementCount) / sizeof(UInt8));
//
//template<class DataType>
//inline bool MemoryEquals(const DataType& lhs, const DataType& rhs)
//{
//    COMPARE_FIXED_SIZE_AND_ALIGNMENT(ArrayEquals, sizeof(DataType), alignof(DataType), &lhs, &rhs, 1);
//}
//
//template<class DataType>
//inline bool MemoryEqualsCount(const DataType* lhs, const DataType* rhs, size_t count)
//{
//    COMPARE_FIXED_SIZE_AND_ALIGNMENT(ArrayEquals, sizeof(DataType), alignof(DataType), lhs, rhs, count);
//}
//
//template<class DataType>
//inline bool MemoryLessThan(const DataType& lhs, const DataType& rhs)
//{
//    // Compares as large little/big endian numbers, *not* lexicographical order like memcmp(). For performance, we compare by casting to integers.
//    // Doing the whole comparison based on the endianness of the platform means it doesn't matter which underlying type we cast to.
//    COMPARE_FIXED_SIZE_AND_ALIGNMENT(ArrayLessThan, sizeof(DataType), alignof(DataType), &lhs, &rhs, 1);
//}
//
//template<class DataType>
//inline bool MemoryGreaterThan(const DataType& lhs, const DataType& rhs)
//{
//    // Compares as large little/big endian numbers, *not* lexicographical order like memcmp(). For performance, we compare by casting to integers.
//    // Doing the whole comparison based on the endianness of the platform means it doesn't matter which underlying type we cast to.
//    COMPARE_FIXED_SIZE_AND_ALIGNMENT(ArrayGreaterThan, sizeof(DataType), alignof(DataType), &lhs, &rhs, 1);
//}
//
//template<typename T>
//struct MemoryCompLess
//{
//    bool operator()(const T& lhs, const T& rhs) const
//    {
//        return MemoryLessThan(lhs, rhs);
//    }
//};
//
//template<typename T>
//struct MemoryCompGreater
//{
//    bool operator()(const T& lhs, const T& rhs) const
//    {
//        return MemoryGreaterThan(lhs, rhs);
//    }
//};
//
template<class T>
inline T clamp(const T&t, const T& t0, const T& t1)
{
    if (t < t0)
        return t0;
    else if (t > t1)
        return t1;
    else
        return t;
}

template<>
inline float clamp(const float&t, const float& t0, const float& t1)
{
    if (t < t0)
        return t0;
    else if (t > t1)
        return t1;
    else
        return t;
}

template<class T>
inline T clamp01(const T& t)
{
    if (t < 0)
        return 0;
    else if (t > 1)
        return 1;
    else
        return t;
}

template<>
inline float clamp01<float>(const float& t)
{
    if (t < 0.0F)
        return 0.0F;
    else if (t > 1.0F)
        return 1.0F;
    else
        return t;
}

//// Asserts if from is NULL or can't be cast to type To
//template<class To, class From> inline
//To assert_cast(From from)
//{
//    Assert(dynamic_cast<To>(from) != NULL);
//    return static_cast<To>(from);
//}
//
//// a good use of this is safe upcasting for type disambiguation. static_cast<> will work too, but it's too broad - it casts among "related types", which is often unsafe.
//template<typename TTo, typename TFrom>
//inline TTo implicit_cast(TFrom value)
//{
//    // a compile error here means that there is no automatic conversion TFrom -> TTo
//    return value;
//}
//
//inline float SmoothStep(float from, float to, float t)
//{
//    t = clamp01(t);
//    t = -2.0F * t * t * t + 3.0F * t * t;
//    return to * t + from * (1.0f - t);
//}
//
//// Rounds value down.
//// Note: base must be power of two value.
//inline UInt32 RoundDown(UInt32 value, SInt32 base)
//{
//    return value & (-base);
//}
//
//// Rounds value up.
//// Note: base must be power of two value.
//inline UInt32 RoundUp(UInt32 value, SInt32 base)
//{
//    return (value + base - 1) & (-base);
//}
//
//// Rounds value up.
//// Note: multiple does not need to be a power of two value.
//template<class DataType>
//inline DataType RoundUpMultiple(DataType value, DataType multiple)
//{
//    return ((value + multiple - 1) / multiple) * multiple;
//}
//
//template<class T>
//inline T* Stride(T* p, size_t offset)
//{
//    return reinterpret_cast<T*>((char*)p + offset);
//}
//
//template<typename T>
//bool AreAllObjectsInArrayNonNull(T* const * objects, int size)
//{
//    for (int i = 0; i < size; i++)
//    {
//        if (objects[i] == NULL)
//            return false;
//    }
//
//    return true;
//}
//
//int FindIndexOfValueInLargeArrayUniqueInt32Simd(const int* ptr, int first, int count, int value);
//
//// Searches for a given value in an unsorted array [ptr+first, ptr+last) without
//// duplicate elements. Returns the index of the found element or -1 if element
//// was not found.
//static inline int FindIndexOfValueInArrayUnique(const int* ptr, int first, int last, int value)
//{
//#ifdef MATH_HAS_SIMD_INT
//    int count = last - first;
//    if (count >= 8)
//        return FindIndexOfValueInLargeArrayUniqueInt32Simd(ptr, first, count, value);
//#endif
//
//    for (int i = first; i < last; ++i)
//        if (ptr[i] == value)
//            return i;
//    return -1;
//}
//
//template<typename T>
//inline void Swap(T& a, T& b)
//{
//    T temp;
//    temp = a;
//    a = b;
//    b = temp;
//}
//
//// Euclid's algorithm:
//// https://en.wikipedia.org/wiki/Euclidean_algorithm
//inline UInt32 GreatestCommonDenominator(UInt32 a, UInt32 b)
//{
//    for (;;)
//    {
//        if (a == 0)
//            return b;
//        b %= a;
//        if (b == 0)
//            return a;
//        a %= b;
//    }
//
//    return 0;
//}
//
//inline UInt32 LeastCommonMultiple(UInt32 a, UInt32 b)
//{
//    UInt32 temp = GreatestCommonDenominator(a, b);
//    return temp ? (a / temp * b) : 0;
//}
