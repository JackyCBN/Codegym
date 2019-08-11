#pragma once

#include "Base.h"

namespace codegym::runtime {


/**
 * @brief Dynamic identifier generator.
 *
 * Utility class template that can be used to assign unique identifiers to types
 * at runtime. Use different specializations to create separate sets of
 * identifiers.
 */
template<typename...>
class family {
    inline static GC_MAYBE_ATOMIC(GC_ID_TYPE) identifier;

    template<typename...>
    inline static const auto inner = identifier++;

public:
    /*! @brief Unsigned integer type. */
    using family_type = GC_ID_TYPE;

    /*! @brief Statically generated unique identifier for the given type. */
    template<typename... Type>
    // at the time I'm writing, clang crashes during compilation if auto is used in place of family_type here
    inline static const family_type type = inner<decay_t<Type>...>;
};


}
