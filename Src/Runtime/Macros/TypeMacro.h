#pragma once

#ifndef GC_ID_TYPE
#define GC_ID_TYPE uint32_t
#endif 


#ifndef GC_NO_ATOMIC
#define GC_MAYBE_ATOMIC(Type) atomic<Type>
#else // GC_MAYBE_ATOMIC
#define GC_MAYBE_ATOMIC(Type) Type
#endif // GC_MAYBE_ATOMIC


#ifndef GC_HS_SUFFIX
#define GC_HS_SUFFIX _hs
#endif // GC_HS_SUFFIX


#ifndef GC_HWS_SUFFIX
#define GC_HWS_SUFFIX _hws
#endif // GC_HWS_SUFFIX