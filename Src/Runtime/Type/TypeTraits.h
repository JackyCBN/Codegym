#pragma once


#include "Base.h"
#include "Core/HashString.h"

namespace codegym::runtime {


/**
 * @brief A class to use to push around lists of types, nothing more.
 * @tparam Type Types provided by the given type list.
 */
template<typename... Type>
struct type_list {
    /*! @brief Unsigned integer type. */
    static constexpr auto size = sizeof...(Type);
};


/*! @brief Primary template isn't defined on purpose. */
template<typename...>
struct type_list_cat;


/*! @brief Concatenates multiple type lists. */
template<>
struct type_list_cat<> {
    /*! @brief A type list composed by the types of all the type lists. */
    using type = type_list<>;
};


/**
 * @brief Concatenates multiple type lists.
 * @tparam Type Types provided by the first type list.
 * @tparam Other Types provided by the second type list.
 * @tparam List Other type lists, if any.
 */
template<typename... Type, typename... Other, typename... List>
struct type_list_cat<type_list<Type...>, type_list<Other...>, List...> {
    /*! @brief A type list composed by the types of all the type lists. */
    using type = typename type_list_cat<type_list<Type..., Other...>, List...>::type;
};


/**
 * @brief Concatenates multiple type lists.
 * @tparam Type Types provided by the type list.
 */
template<typename... Type>
struct type_list_cat<type_list<Type...>> {
    /*! @brief A type list composed by the types of all the type lists. */
    using type = type_list<Type...>;
};


/**
 * @brief Helper type.
 * @tparam List Type lists to concatenate.
 */
template<typename... List>
using type_list_cat_t = typename type_list_cat<List...>::type;


/*! @brief Primary template isn't defined on purpose. */
template<typename>
struct type_list_unique;


/**
 * @brief Removes duplicates types from a type list.
 * @tparam Type One of the types provided by the given type list.
 * @tparam Other The other types provided by the given type list.
 */
template<typename Type, typename... Other>
struct type_list_unique<type_list<Type, Other...>> {
    /*! @brief A type list without duplicate types. */
    using type = conditional_t<
        disjunction_v<is_same<Type, Other>...>,
        typename type_list_unique<type_list<Other...>>::type,
        type_list_cat_t<type_list<Type>, typename type_list_unique<type_list<Other...>>::type>
    >;
};


/*! @brief Removes duplicates types from a type list. */
template<>
struct type_list_unique<type_list<>> {
    /*! @brief A type list without duplicate types. */
    using type = type_list<>;
};


/**
 * @brief Helper type.
 * @tparam Type A type list.
 */
template<typename Type>
using type_list_unique_t = typename type_list_unique<Type>::type;


/*! @brief Traits class used mainly to push things across boundaries. */
template<typename>
struct named_type_traits;


/**
 * @brief Specialization used to get rid of constness.
 * @tparam Type Named type.
 */
template<typename Type>
struct named_type_traits<const Type>
        : named_type_traits<Type>
{};


/**
 * @brief Helper type.
 * @tparam Type Potentially named type.
 */
template<typename Type>
using named_type_traits_t = typename named_type_traits<Type>::type;


/**
 * @brief Provides the member constant `value` to true if a given type has a
 * name. In all other cases, `value` is false.
 */
template<typename, typename = void_t<>>
struct is_named_type: false_type {};


/**
 * @brief Provides the member constant `value` to true if a given type has a
 * name. In all other cases, `value` is false.
 * @tparam Type Potentially named type.
 */
template<typename Type>
struct is_named_type<Type, void_t<named_type_traits_t<decay_t<Type>>>>: true_type {};


/**
 * @brief Helper variable template.
 *
 * True if a given type has a name, false otherwise.
 *
 * @tparam Type Potentially named type.
 */
template<class Type>
constexpr auto is_named_type_v = is_named_type<Type>::value;


}


/**
 * @brief Utility macro to deal with an issue of MSVC.
 *
 * See _msvc-doesnt-expand-va-args-correctly_ on SO for all the details.
 *
 * @param args Argument to expand.
 */
#define GC_EXPAND(args) args


/**
 * @brief Makes an already existing type a named type.
 *
 * The current definition contains a workaround for Clang 6 because it fails to
 * deduce correctly the type to use to specialize the class template.<br/>
 * With a compiler that fully supports C++17 and works fine with deduction
 * guides, the following should be fine instead:
 *
 * @code{.cpp}
 * integral_constant<GC_ID_TYPE, basic_hashed_string{#type}>
 * @endcode
 *
 * In order to support even sligthly older compilers, I prefer to stick to the
 * implementation below.
 *
 * @param type Type to assign a name to.
 */
#define GC_NAMED_TYPE(type)\
    template<>\
    struct named_type_traits<type>\
        : integral_constant<GC_ID_TYPE, basic_hashed_string<remove_cv_t<remove_pointer_t<decay_t<decltype(#type)>>>>{#type}>\
    {\
        static_assert(is_same_v<decay_t<type>, type>);\
    };


/**
 * @brief Defines a named type (to use for structs).
 * @param clazz Name of the type to define.
 * @param body Body of the type to define.
 */
#define GC_NAMED_STRUCT_ONLY(clazz, body)\
    struct clazz body;\
    GC_NAMED_TYPE(clazz)


/**
 * @brief Defines a named type (to use for structs).
 * @param ns Namespace where to define the named type.
 * @param clazz Name of the type to define.
 * @param body Body of the type to define.
 */
#define GC_NAMED_STRUCT_WITH_NAMESPACE(ns, clazz, body)\
    namespace ns { struct clazz body; }\
    GC_NAMED_TYPE(ns::clazz)


/*! @brief Utility function to simulate macro overloading. */
#define GC_NAMED_STRUCT_OVERLOAD(_1, _2, _3, FUNC, ...) FUNC
/*! @brief Defines a named type (to use for structs). */
#define GC_NAMED_STRUCT(...) GC_EXPAND(GC_NAMED_STRUCT_OVERLOAD(__VA_ARGS__, GC_NAMED_STRUCT_WITH_NAMESPACE, GC_NAMED_STRUCT_ONLY,)(__VA_ARGS__))


/**
 * @brief Defines a named type (to use for classes).
 * @param clazz Name of the type to define.
 * @param body Body of the type to define.
 */
#define GC_NAMED_CLASS_ONLY(clazz, body)\
    class clazz body;\
    GC_NAMED_TYPE(clazz)


/**
 * @brief Defines a named type (to use for classes).
 * @param ns Namespace where to define the named type.
 * @param clazz Name of the type to define.
 * @param body Body of the type to define.
 */
#define GC_NAMED_CLASS_WITH_NAMESPACE(ns, clazz, body)\
    namespace ns { class clazz body; }\
    GC_NAMED_TYPE(ns::clazz)


/*! @brief Utility function to simulate macro overloading. */
#define GC_NAMED_CLASS_MACRO(_1, _2, _3, FUNC, ...) FUNC
/*! @brief Defines a named type (to use for classes). */
#define GC_NAMED_CLASS(...) GC_EXPAND(GC_NAMED_CLASS_MACRO(__VA_ARGS__, GC_NAMED_CLASS_WITH_NAMESPACE, GC_NAMED_CLASS_ONLY,)(__VA_ARGS__))

template <typename T>
using ParameterType = conditional<is_scalar_v<T>, T, add_lvalue_reference_t<add_const_t<T>>>;

template <typename T>
using ParameterType_t = typename ParameterType<T>::type;