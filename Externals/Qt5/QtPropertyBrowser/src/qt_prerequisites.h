
#ifndef QT_CORE_PREREQUISITES_H_
#define QT_CORE_PREREQUISITES_H_

#define QT_PLATFORM_WINDOWS 1
#define QT_PLATFORM_LINUX 2
#define QT_PLATFORM_APPLE 3

#define QT_COMPILER_MSVC 1
#define QT_COMPILER_GNUC 2
#define QT_COMPILER_CLANG 3
#define QT_COMPILER_APPLECLANG 4

#define QT_ENDIAN_LITTLE 1
#define QT_ENDIAN_BIG 2

#define QT_ARCH_32 1
#define QT_ARCH_64 2

/////////////////////////////////////////////////////////////////////////////////////////
// Platform
/////////////////////////////////////////////////////////////////////////////////////////
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define QT_PLATFORM QT_PLATFORM_WINDOWS
#elif defined( __APPLE_CC__)
#   define QT_PLATFORM QT_PLATFORM_APPLE
#else
#   define QT_PLATFORM QT_PLATFORM_LINUX
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Compiler
/////////////////////////////////////////////////////////////////////////////////////////
#if defined( __clang__ )

#if defined __apple_build_version__
#   define QT_COMPILER QT_COMPILER_APPLECLANG
#else
#   define QT_COMPILER QT_COMPILER_CLANG
#endif
#   define QT_COMP_VER (((__clang_major__)*100) + \
                         (__clang_minor__*10) + \
                         __clang_patchlevel__)
#elif defined( __GNUC__ )
#   define QT_COMPILER QT_COMPILER_GNUC
#   define QT_COMP_VER (((__GNUC__)*100) + \
                         (__GNUC_MINOR__*10) + \
                          __GNUC_PATCHLEVEL__)
#elif defined( _MSC_VER )
#   define QT_COMPILER QT_COMPILER_MSVC
#   define QT_COMP_VER _MSC_VER
#else
#   error "No known compiler. Abort! Abort!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////
// Architecture
/////////////////////////////////////////////////////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) ||\
    defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define QT_ARCH_TYPE QT_ARCH_64
#else
#   define QT_ARCH_TYPE QT_ARCH_32
#endif

#if QT_COMPILER == QT_COMPILER_MSVC
#   define QT_INLINE          inline
#   define QT_FORCE_INLINE    __forceinline
#elif QT_COMPILER == QT_COMPILER_GNUC
#   define QT_INLINE          inline
#   define QT_FORCE_INLINE    inline  __attribute__((always_inline))
#elif QT_COMPILER == QT_COMPILER_CLANG || QT_COMPILER == QT_COMPILER_APPLECLANG
#   define QT_INLINE          inline
#   define QT_FORCE_INLINE    inline  __attribute__((always_inline))
#else
#   define QT_INLINE          inline
#   define QT_FORCE_INLINE    inline // no force inline for other platforms possible
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Compiler specific cmds for export and import code to DLL
/////////////////////////////////////////////////////////////////////////////////////////
#if QT_COMPILER == QT_COMPILER_MSVC || __MINGW32__ || __CYGWIN__
#     define QT_HELPER_DLL_IMPORT __declspec( dllimport )
#     define QT_HELPER_DLL_EXPORT __declspec( dllexport )
#     define QT_HELPER_DLL_LOCAL
#elif QT_COMPILER == QT_COMPILER_GNUC
#   if QT_COMP_VER >= 400
#       define QT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#       define QT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#       define QT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#   else
#       define QT_HELPER_DLL_IMPORT
#       define QT_HELPER_DLL_EXPORT
#       define QT_HELPER_DLL_LOCAL
#   endif
#elif QT_COMPILER == QT_COMPILER_CLANG || QT_COMPILER == QT_COMPILER_APPLECLANG
#       define QT_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#       define QT_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#       define QT_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#   error "Do not know how to export classes for this platform"
#endif

#ifdef QT_QTPROPERTYBROWSER_DLL // compiled as a DLL
#   ifdef QT_DLL_EXPORTS // defined if we are building the DLL
#       define QT_QTPROPERTYBROWSER_EXPORT QT_HELPER_DLL_EXPORT
#   else
#       define QT_QTPROPERTYBROWSER_EXPORT QT_HELPER_DLL_IMPORT
#   endif
#   define QT_LOCAL QT_HELPER_DLL_LOCAL
#else // it's a static lib.
#   define QT_QTPROPERTYBROWSER_EXPORT
#   define QT_LOCAL
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Macros for some C++11 features, which are not available on every compiler
/////////////////////////////////////////////////////////////////////////////////////////


#if QT_COMPILER == QT_COMPILER_MSVC
#   if QT_COMP_VER <= 1800
#       define QT_NO_CXX11_NOEXCEPT
#       define QT_NO_CXX17_NOEXCEPT_FUNC_TYPE
#   endif
#   if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#       define QT_NO_CXX11_CONSTEXPR
#       define QT_NO_CXX14_CONSTEXPR
#   endif
#   if !defined(_MSVC_LANG) || (_MSVC_LANG < 201703L)
#       define QT_NO_CXX17_NOEXCEPT_FUNC_TYPE
#   endif
#endif

#if QT_COMPILER == QT_COMPILER_GNUC ||  QT_COMPILER == QT_COMPILER_CLANG || QT_COMPILER == QT_COMPILER_APPLECLANG
#   if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#       define QT_NO_CXX14_CONSTEXPR
#   endif
#   if !defined(cpp_noexcept)
#       define QT_NO_CXX11_NOEXCEPT
#   endif
#   if !defined(__cpp_noexcept_function_type) || (__cpp_noexcept_function_type < 201510)
#       define QT_NO_CXX17_NOEXCEPT_FUNC_TYPE
#   endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////

#if defined(QT_NO_CXX11_CONSTEXPR)
#   define QT_CONSTEXPR
#   define QT_CONSTEXPR_OR_CONST const
#else
#   define QT_CONSTEXPR constexpr
#   define QT_CONSTEXPR_OR_CONST constexpr
#endif


#if defined(QT_NO_CXX14_CONSTEXPR)
#   define QT_CXX14_CONSTEXPR
#else
#   define QT_CXX14_CONSTEXPR constexpr
#endif

#ifdef QT_NO_CXX11_NOEXCEPT
#   define QT_NOEXCEPT
#   define QT_NOEXCEPT_OR_NOTHROW throw()
#else
#   define QT_NOEXCEPT noexcept
#   define QT_NOEXCEPT_OR_NOTHROW noexcept
#endif


#define QT_STATIC_CONSTEXPR static QT_CONSTEXPR_OR_CONST

/////////////////////////////////////////////////////////////////////////////////////////
// deprecated macro
/*
#if QT_COMPILER == QT_COMPILER_GNUC || QT_COMPILER == QT_COMPILER_CLANG || QT_COMPILER == QT_COMPILER_APPLECLANG
#   define QT_DEPRECATED __attribute__ ((deprecated))
#   define QT_DEPRECATED_WITH_MSG(msg) __attribute__ ((deprecated(msg)))
#elif QT_COMPILER == QT_COMPILER_MSVC
#   define QT_DEPRECATED __declspec(deprecated)
#   define QT_DEPRECATED_WITH_MSG(msg) __declspec(deprecated(msg))
#else
#   pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#   define QT_DEPRECATED(func) func
#   define QT_DEPRECATED_MSG(func) func
#endif
*/
/////////////////////////////////////////////////////////////////////////////////////////
// Disable some MSVC compile warnings
/////////////////////////////////////////////////////////////////////////////////////////
#if  QT_COMPILER == QT_COMPILER_MSVC
// Save warnings state
#   pragma warning (push)
// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4786)

// Turn off warnings generated by long std templates
// This warns about truncation to 255 characters in debug/browse info
#   pragma warning (disable : 4503)

// disable: "<type> needs to have dll-interface to be used by clients'
// Happens on STL member variables which are not public therefore is ok
#   pragma warning (disable : 4251)

// disable: "non dll-interface class used as base for dll-interface class"
// Happens when deriving from Singleton because bug in compiler ignores
// template export
#   pragma warning (disable : 4275)

// disable: "C++ Exception Specification ignored"
// This is because MSVC 6 did not implement all the C++ exception
// specifications in the ANSI C++ draft.
#   pragma warning( disable : 4290 )

// disable: "no suitable definition provided for explicit template
// instantiation request" Occurs in VC7 for no justifiable reason on all
// #includes of Singleton
#   pragma warning( disable: 4661)

// disable: "unreferenced formal parameter"
// Many versions of VC have bugs which generate this error in cases where they shouldn't
#   pragma warning (disable : 4100)
#endif

#if QT_COMPILER == QT_COMPILER_GNUC
#   define QT_BEGIN_DISABLE_DEPRECATED_WARNING        _Pragma ("GCC diagnostic push") \
                                                        _Pragma ("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#   define QT_END_DISABLE_DEPRECATED_WARNING          _Pragma ("GCC diagnostic pop")

#   define QT_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING
#   define QT_END_DISABLE_CONDITIONAL_EXPR_WARNING
#if QT_COMP_VER >= 700

    #define QT_BEGIN_DISABLE_EXCEPT_TYPE_WARNING      _Pragma ("GCC diagnostic push") \
                                                        _Pragma ("GCC diagnostic ignored \"-Wnoexcept-type\"")
    #define QT_END_DISABLE_EXCEPT_TYPE_WARNING        _Pragma ("GCC diagnostic pop")
#else

    #define QT_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
    #define QT_END_DISABLE_EXCEPT_TYPE_WARNING

#endif

#   define QT_DECLARE_PLUGIN_CTOR       __attribute__((constructor))
#   define QT_DECLARE_PLUGIN_DTOR       __attribute__((destructor))

#elif QT_COMPILER == QT_COMPILER_CLANG || QT_COMPILER == QT_COMPILER_APPLECLANG

#if defined(__has_warning) && __has_warning("-Wdeprecated-declarations")
#   define QT_BEGIN_DISABLE_DEPRECATED_WARNING        _Pragma ("clang diagnostic push") \
                                                        _Pragma ("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#   define QT_END_DISABLE_DEPRECATED_WARNING          _Pragma ("clang diagnostic pop")
#else
#   define QT_BEGIN_DISABLE_DEPRECATED_WARNING
#   define QT_END_DISABLE_DEPRECATED_WARNING
#endif

#   define QT_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING
#   define QT_END_DISABLE_CONDITIONAL_EXPR_WARNING

#if defined(__has_warning) && __has_warning("-Wnoexcept-type")
#       define QT_BEGIN_DISABLE_EXCEPT_TYPE_WARNING   _Pragma ("clang diagnostic push") \
                                                        _Pragma ("clang diagnostic ignored \"-Wnoexcept-type\"")
#       define QT_END_DISABLE_EXCEPT_TYPE_WARNING     _Pragma ("clang diagnostic pop")
#else
#       define QT_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
#       define QT_END_DISABLE_EXCEPT_TYPE_WARNING
#endif

#   define QT_DECLARE_PLUGIN_CTOR        __attribute__((__constructor__))
#   define QT_DECLARE_PLUGIN_DTOR        __attribute__((__destructor__))

#elif QT_COMPILER == QT_COMPILER_MSVC
#   define QT_BEGIN_DISABLE_DEPRECATED_WARNING        __pragma( warning( push )) \
                                                        __pragma( warning( disable: 4996))
#   define QT_END_DISABLE_DEPRECATED_WARNING          __pragma( warning( pop ))


#   define QT_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING  __pragma( warning( push )) \
                                                        __pragma( warning( disable: 4127))
#   define QT_END_DISABLE_CONDITIONAL_EXPR_WARNING    __pragma( warning( pop ))

#   define QT_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
#   define QT_END_DISABLE_EXCEPT_TYPE_WARNING
#   define QT_DECLARE_PLUGIN_CTOR
#   define QT_DECLARE_PLUGIN_DTOR

#else
#   pragma message("WARNING: unknown compiler, don't know how to disable deprecated warnings")
#endif

#endif // QT_CORE_PREREQUISITES_H_
