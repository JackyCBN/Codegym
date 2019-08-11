#pragma once

#include "Base.h"


namespace codegym::runtime {


/**
 * @brief Types identifiers.
 *
 * Variable template used to generate identifiers at compile-time for the given
 * types. Use the `get` member function to know what's the identifier associated
 * to the specific type.
 *
 * @note
 * Identifiers are constant expression and can be used in any context where such
 * an expression is required. As an example:
 * @code{.cpp}
 * using id = identifier<a_type, another_type>;
 *
 * switch(a_type_identifier) {
 * case id::type<a_type>:
 *     // ...
 *     break;
 * case id::type<another_type>:
 *     // ...
 *     break;
 * default:
 *     // ...
 * }
 * @endcode
 *
 * @tparam Types List of types for which to generate identifiers.
 */
template<typename... Types>
class identifier {
    using tuple_type = tuple<decay_t<Types>...>;

    template<typename Type, size_t... Indexes>
    static constexpr GC_ID_TYPE get(index_sequence<Indexes...>) CG_NOEXCEPT {
        static_assert(disjunction_v<is_same<Type, Types>...>);
        return (0 + ... + (is_same_v<Type, tuple_element_t<Indexes, tuple_type>> ? GC_ID_TYPE(Indexes) : GC_ID_TYPE{}));
    }

public:
    /*! @brief Unsigned integer type. */
    using identifier_type = GC_ID_TYPE;

    /*! @brief Statically generated unique identifier for the given type. */
    template<typename Type>
    static constexpr identifier_type type = get<decay_t<Type>>(make_index_sequence<sizeof...(Types)>{});
};


}
