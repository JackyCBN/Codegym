#pragma once

#include "Base.h"

namespace codegym::runtime {


/**
 * @brief Minimal implementation of the monostate pattern.
 *
 * A minimal, yet complete configuration system built on top of the monostate
 * pattern. Thread safe by design, it works only with basic types like `int`s or
 * `bool`s.<br/>
 * Multiple types and therefore more than one value can be associated with a
 * single key. Because of this, users must pay attention to use the same type
 * both during an assignment and when they try to read back their data.
 * Otherwise, they can incur in unexpected results.
 */
template<GC_ID_TYPE>
struct monostate {
    /**
     * @brief Assigns a value of a specific type to a given key.
     * @tparam Type Type of the value to assign.
     * @param val User data to assign to the given key.
     */
    template<typename Type>
    void operator=(Type val) const CG_NOEXCEPT {
        value<Type> = val;
    }

    /**
     * @brief Gets a value of a specific type for a given key.
     * @tparam Type Type of the value to get.
     * @return Stored value, if any.
     */
    template<typename Type>
    operator Type() const CG_NOEXCEPT {
        return value<Type>;
    }

private:
    template<typename Type>
    inline static GC_MAYBE_ATOMIC(Type) value{};
};


/**
 * @brief Helper variable template.
 * @tparam Value Value used to differentiate between different variables.
 */
template<GC_ID_TYPE Value>
inline monostate<Value> monostate_v = {};


}