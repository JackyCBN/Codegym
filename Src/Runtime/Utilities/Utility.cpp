//#include "UnityPrefix.h"
//#include "Configuration/UnityConfigure.h"
//#include "Runtime/Utilities/Utility.h"
//#include "Runtime/Math/Simd/vec-types.h"
//#include "Runtime/Math/Simd/vec-math.h"
//
//static FORCE_INLINE void FindIndexOfValueInInt32PerformIteration(
//    math::int4& currentIdx,
//    math::int4& foundIdx,
//    const math::int4& valueToSearch,
//    const int*& ptr)
//{
//    using namespace math;
//
//    int4 idxPerIter = int4(4, 4, 4, 4);
//
//    int4 values = vload4i(ptr);
//    foundIdx = bitselect(foundIdx, currentIdx, values == valueToSearch);
//
//    ptr += 4;
//    currentIdx += idxPerIter;
//}
//
//static FORCE_INLINE int FindIndexOfValueInInt32ExtractIndex(const math::int4& foundIdx)
//{
//    using namespace math;
//
//    // We want some way to extract found index without branches. We notice that
//    // since there can't be duplicates in m_Names, found_idx vector will only
//    // contain the values of the found index or -1 (0xffffffff). Thus we can
//    // just AND the vector to get the value or -1 if it's not found.
//    int2 foundIdx2 = foundIdx.xy & foundIdx.zw;
//    foundIdx2 = foundIdx2 & foundIdx2.yx;
//    return extract(foundIdx2, 0);
//}
//
//int FindIndexOfValueInLargeArrayUniqueInt32Simd(const int* ptr, int first, int count, int value)
//{
//    ptr += first;
//
//    using namespace math;
//    int4 valueToSearch = int4(value, value, value, value);
//
//    int4 currentIdx = int4(0, 1, 2, 3);
//    currentIdx += int4(first, first, first, first);
//    int4 foundIdx = int4(-1, -1, -1, -1);
//
//    while (count >= 32)
//    {
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        count -= 32;
//
//        if (!all(foundIdx == int4(-1, -1, -1, -1)))
//            return FindIndexOfValueInInt32ExtractIndex(foundIdx);
//    }
//    while (count >= 4)
//    {
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//        count -= 4;
//    }
//    if (count > 0)
//    {
//        int overlap = 4 - count;
//        ptr -= overlap;
//        currentIdx -= int4(overlap, overlap, overlap, overlap);
//        FindIndexOfValueInInt32PerformIteration(currentIdx, foundIdx, valueToSearch, ptr);
//    }
//
//    return FindIndexOfValueInInt32ExtractIndex(foundIdx);
//}
