#pragma once


#include "Base.h"

namespace codegym::runtime {


/**
 * @cond TURN_OFF_DOXYGEN
 * Internal details not to be documented.
 */


namespace internal {


template<typename>
struct fnv1a_traits;


template<>
struct fnv1a_traits<uint32_t> {
    static constexpr uint32_t offset = 2166136261;
    static constexpr uint32_t prime = 16777619;
};


template<>
struct fnv1a_traits<uint64_t> {
    static constexpr uint64_t offset = 14695981039346656037ull;
    static constexpr uint64_t prime = 1099511628211ull;
};


}


/**
 * Internal details not to be documented.
 * @endcond TURN_OFF_DOXYGEN
 */


/**
 * @brief Zero overhead unique identifier.
 *
 * A hashed string is a compile-time tool that allows users to use
 * human-readable identifers in the codebase while using their numeric
 * counterparts at runtime.<br/>
 * Because of that, a hashed string can also be used in constant expressions if
 * required.
 *
 * @tparam Char Character type.
 */
template<typename Char>
class basic_hashed_string {
    using traits_type = internal::fnv1a_traits<GC_ID_TYPE>;

    struct const_wrapper {
        // non-explicit constructor on purpose
        constexpr const_wrapper(const Char *curr) CG_NOEXCEPT: str{curr} {}
        const Char *str;
    };

    // Fowler–Noll–Vo hash function v. 1a - the good
    static constexpr GC_ID_TYPE helper(GC_ID_TYPE partial, const Char *curr) CG_NOEXCEPT {
        return curr[0] == 0 ? partial : helper((partial^curr[0])*traits_type::prime, curr+1);
    }

public:
    /*! @brief Character type. */
    using value_type = Char;
    /*! @brief Unsigned integer type. */
    using hash_type = GC_ID_TYPE;

    /**
     * @brief Returns directly the numeric representation of a string.
     *
     * Forcing template resolution avoids implicit conversions. An
     * human-readable identifier can be anything but a plain, old bunch of
     * characters.<br/>
     * Example of use:
     * @code{.cpp}
     * const auto value = basic_hashed_string<char>::to_value("my.png");
     * @endcode
     *
     * @tparam N Number of characters of the identifier.
     * @param str Human-readable identifer.
     * @return The numeric representation of the string.
     */
    template<size_t N>
    static constexpr hash_type to_value(const value_type (&str)[N]) CG_NOEXCEPT {
        return helper(traits_type::offset, str);
    }

    /**
     * @brief Returns directly the numeric representation of a string.
     * @param wrapper Helps achieving the purpose by relying on overloading.
     * @return The numeric representation of the string.
     */
    static hash_type to_value(const_wrapper wrapper) CG_NOEXCEPT {
        return helper(traits_type::offset, wrapper.str);
    }

    /**
     * @brief Returns directly the numeric representation of a string view.
     * @param str Human-readable identifer.
     * @param size Length of the string to hash.
     * @return The numeric representation of the string.
     */
    static hash_type to_value(const value_type *str, size_t size) CG_NOEXCEPT {
        GC_ID_TYPE partial{traits_type::offset};
        while(size--) { partial = (partial^(str++)[0])*traits_type::prime; }
        return partial;
    }

    /*! @brief Constructs an empty hashed string. */
    constexpr basic_hashed_string() CG_NOEXCEPT
        : str{nullptr}, hash{}
    {}

    /**
     * @brief Constructs a hashed string from an array of const characters.
     *
     * Forcing template resolution avoids implicit conversions. An
     * human-readable identifier can be anything but a plain, old bunch of
     * characters.<br/>
     * Example of use:
     * @code{.cpp}
     * basic_hashed_string<char> hs{"my.png"};
     * @endcode
     *
     * @tparam N Number of characters of the identifier.
     * @param curr Human-readable identifer.
     */
    template<size_t N>
    constexpr basic_hashed_string(const value_type (&curr)[N]) CG_NOEXCEPT
        : str{curr}, hash{helper(traits_type::offset, curr)}
    {}

    /**
     * @brief Explicit constructor on purpose to avoid constructing a hashed
     * string directly from a `const value_type *`.
     * @param wrapper Helps achieving the purpose by relying on overloading.
     */
    explicit constexpr basic_hashed_string(const_wrapper wrapper) CG_NOEXCEPT
        : str{wrapper.str}, hash{helper(traits_type::offset, wrapper.str)}
    {}

    /**
     * @brief Returns the human-readable representation of a hashed string.
     * @return The string used to initialize the instance.
     */
    constexpr const value_type * data() const CG_NOEXCEPT {
        return str;
    }

    /**
     * @brief Returns the numeric representation of a hashed string.
     * @return The numeric representation of the instance.
     */
    constexpr hash_type value() const CG_NOEXCEPT {
        return hash;
    }

    /**
     * @brief Returns the human-readable representation of a hashed string.
     * @return The string used to initialize the instance.
     */
    constexpr operator const value_type *() const CG_NOEXCEPT { return str; }

    /*! @copydoc value */
    constexpr operator hash_type() const CG_NOEXCEPT { return hash; }

    /**
     * @brief Compares two hashed strings.
     * @param other Hashed string with which to compare.
     * @return True if the two hashed strings are identical, false otherwise.
     */
    constexpr bool operator==(const basic_hashed_string &other) const CG_NOEXCEPT {
        return hash == other.hash;
    }

private:
    const value_type *str;
    hash_type hash;
};


/**
 * @brief Deduction guide.
 *
 * It allows to deduce the character type of the hashed string directly from a
 * human-readable identifer provided to the constructor.
 *
 * @tparam Char Character type.
 * @tparam N Number of characters of the identifier.
 * @param str Human-readable identifer.
 */
template<typename Char, size_t N>
basic_hashed_string(const Char (&str)[N]) CG_NOEXCEPT
-> basic_hashed_string<Char>;


/**
 * @brief Compares two hashed strings.
 * @tparam Char Character type.
 * @param lhs A valid hashed string.
 * @param rhs A valid hashed string.
 * @return True if the two hashed strings are identical, false otherwise.
 */
template<typename Char>
constexpr bool operator!=(const basic_hashed_string<Char> &lhs, const basic_hashed_string<Char> &rhs) CG_NOEXCEPT {
    return !(lhs == rhs);
}


/*! @brief Aliases for common character types. */
using HashString = basic_hashed_string<char>;


/*! @brief Aliases for common character types. */
using HashWstring = basic_hashed_string<wchar_t>;


}


/**
 * @brief User defined literal for hashed strings.
 * @param str The literal without its suffix.
 * @return A properly initialized hashed string.
 */
constexpr codegym::runtime::HashString operator"" GC_HS_SUFFIX(const char *str, size_t) CG_NOEXCEPT {
    return codegym::runtime::HashString{str};
}


/**
 * @brief User defined literal for hashed wstrings.
 * @param str The literal without its suffix.
 * @return A properly initialized hashed wstring.
 */
constexpr codegym::runtime::HashWstring operator"" GC_HWS_SUFFIX(const wchar_t *str, size_t) CG_NOEXCEPT {
    return codegym::runtime::HashWstring{str};
}
