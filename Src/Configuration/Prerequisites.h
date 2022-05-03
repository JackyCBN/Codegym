#ifndef CG_CORE_PREREQUISITES_H_
#define CG_CORE_PREREQUISITES_H_

namespace CodeGym
{

#define CG_PLATFORM_WINDOWS 1
#define CG_PLATFORM_LINUX 2
#define CG_PLATFORM_APPLE 3

#define CG_COMPILER_MSVC 1
#define CG_COMPILER_GNUC 2
#define CG_COMPILER_CLANG 3
#define CG_COMPILER_APPLECLANG 4

#define CG_ENDIAN_LITTLE 1
#define CG_ENDIAN_BIG 2

#define CG_ARCH_32 1
#define CG_ARCH_64 2

/////////////////////////////////////////////////////////////////////////////////////////
// Platform
/////////////////////////////////////////////////////////////////////////////////////////
#if defined( __WIN32__ ) || defined( _WIN32 )
#   define CG_PLATFORM CG_PLATFORM_WINDOWS
#   define CG_WINDOWS_DEVELOP
#elif defined( __APPLE_CC__)
#   define CG_PLATFORM CG_PLATFORM_APPLE
#   define CG_APPLE_DEVELOP
#else
#   define CG_PLATFORM CG_PLATFORM_LINUX
#   define CG_LINUX_DEVELOP
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Compiler
/////////////////////////////////////////////////////////////////////////////////////////
#if defined( __clang__ )

#if defined __apple_build_version__
#   define CG_COMPILER CG_COMPILER_APPLECLANG
#else
#   define CG_COMPILER CG_COMPILER_CLANG
#endif
#   define CG_COMP_VER (((__clang_major__)*100) + \
                         (__clang_minor__*10) + \
                         __clang_patchlevel__)
#elif defined( __GNUC__ )
#   define CG_COMPILER CG_COMPILER_GNUC
#   define CG_COMP_VER (((__GNUC__)*100) + \
                         (__GNUC_MINOR__*10) + \
                          __GNUC_PATCHLEVEL__)
#elif defined( _MSC_VER )
#   define CG_COMPILER CG_COMPILER_MSVC
#   define CG_COMP_VER _MSC_VER
#else
#   error "No known compiler. Abort! Abort!"
#endif


/////////////////////////////////////////////////////////////////////////////////////////
// Architecture
/////////////////////////////////////////////////////////////////////////////////////////
#if defined(__x86_64__) || defined(_M_X64) || defined(__powerpc64__) || defined(__alpha__) ||\
    defined(__ia64__) || defined(__s390__) || defined(__s390x__)
#   define CG_ARCH_TYPE CG_ARCH_64
#else
#   define CG_ARCH_TYPE CG_ARCH_32
#endif

#if CG_COMPILER == CG_COMPILER_MSVC
#   define CG_INLINE          inline
#   define CG_FORCE_INLINE    __forceinline
#elif CG_COMPILER == CG_COMPILER_GNUC
#   define CG_INLINE          inline
#   define CG_FORCE_INLINE    inline  __attribute__((always_inline))
#elif CG_COMPILER == CG_COMPILER_CLANG || CG_COMPILER == CG_COMPILER_APPLECLANG
#   define CG_INLINE          inline
#   define CG_FORCE_INLINE    inline  __attribute__((always_inline))
#else
#   define CG_INLINE          inline
#   define CG_FORCE_INLINE    inline // no force inline for other platforms possible
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Compiler specific cmds for export and import code to DLL
/////////////////////////////////////////////////////////////////////////////////////////
#if CG_COMPILER == CG_COMPILER_MSVC || __MINGW32__ || __CYGWIN__
#     define CG_HELPER_DLL_IMPORT __declspec( dllimport )
#     define CG_HELPER_DLL_EXPORT __declspec( dllexport )
#     define CG_HELPER_DLL_LOCAL
#elif CG_COMPILER == CG_COMPILER_GNUC
#   if CG_COMP_VER >= 400
#       define CG_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#       define CG_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#       define CG_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#   else
#       define CG_HELPER_DLL_IMPORT
#       define CG_HELPER_DLL_EXPORT
#       define CG_HELPER_DLL_LOCAL
#   endif
#elif CG_COMPILER == CG_COMPILER_CLANG || CG_COMPILER == CG_COMPILER_APPLECLANG
#       define CG_HELPER_DLL_IMPORT __attribute__ ((visibility ("default")))
#       define CG_HELPER_DLL_EXPORT __attribute__ ((visibility ("default")))
#       define CG_HELPER_DLL_LOCAL  __attribute__ ((visibility ("hidden")))
#else
#   error "Do not know how to export classes for this platform"
#endif

#ifdef CG_DLL // compiled as a DLL
#   ifdef CG_DLL_EXPORTS // defined if we are building the DLL
#       define CG_API CG_HELPER_DLL_EXPORT
#   else
#       define CG_API CG_HELPER_DLL_IMPORT
#   endif
#   define CG_LOCAL CG_HELPER_DLL_LOCAL
#else // it's a static lib.
#   define CG_API
#   define CG_LOCAL
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Macros for some C++11 features, which are not available on every compiler
/////////////////////////////////////////////////////////////////////////////////////////


#if CG_COMPILER == CG_COMPILER_MSVC
#   if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#       define CG_NO_CXX11_CONSTEXPR
#       define CG_NO_CXX14_CONSTEXPR
#   endif
#   if !defined(_MSVC_LANG) || (_MSVC_LANG < 201703L)
#       define CG_NO_CXX17_NOEXCEPT_FUNC_TYPE
#   endif
#endif

#if CG_COMPILER == CG_COMPILER_GNUC ||  CG_COMPILER == CG_COMPILER_CLANG || CG_COMPILER == CG_COMPILER_APPLECLANG
#   if !defined(__cpp_constexpr) || (__cpp_constexpr < 201304)
#       define CG_NO_CXX14_CONSTEXPR
#   endif
#   if !defined(cpp_noexcept)
#       define CG_NO_CXX11_NOEXCEPT
#   endif
#   if !defined(__cpp_noexcept_function_type) || (__cpp_noexcept_function_type < 201510)
#       define CG_NO_CXX17_NOEXCEPT_FUNC_TYPE
#   endif
#endif

/////////////////////////////////////////////////////////////////////////////////////////

#if defined(CG_NO_CXX11_CONSTEXPR)
#   define CG_CONSTEXPR
#   define CG_CONSTEXPR_OR_CONST const
#else
#   define CG_CONSTEXPR constexpr
#   define CG_CONSTEXPR_OR_CONST constexpr
#endif


#if defined(CG_NO_CXX14_CONSTEXPR)
#   define CG_CXX14_CONSTEXPR
#else
#   define CG_CXX14_CONSTEXPR constexpr
#endif

#ifdef CG_NO_CXX11_NOEXCEPT
#   define CG_NOEXCEPT
#   define CG_NOEXCEPT_OR_NOTHROW throw()
#else
#   define CG_NOEXCEPT noexcept
#   define CG_NOEXCEPT_OR_NOTHROW noexcept
#endif


#define CG_STATIC_CONSTEXPR static CG_CONSTEXPR_OR_CONST

/////////////////////////////////////////////////////////////////////////////////////////
// deprecated macro
#if CG_COMPILER == CG_COMPILER_GNUC || CG_COMPILER == CG_COMPILER_CLANG || CG_COMPILER == CG_COMPILER_APPLECLANG
#   define CG_DEPRECATED __attribute__ ((deprecated))
#   define CG_DEPRECATED_WITH_MSG(msg) __attribute__ ((deprecated(msg)))
#elif CG_COMPILER == CG_COMPILER_MSVC
#   define CG_DEPRECATED __declspec(deprecated)
#   define CG_DEPRECATED_WITH_MSG(msg) __declspec(deprecated(msg))
#else
#   pragma message("WARNING: You need to implement DEPRECATED for this compiler")
#   define CG_DEPRECATED(func) func
#   define CG_DEPRECATED_MSG(func) func
#endif

/////////////////////////////////////////////////////////////////////////////////////////
// Disable some MSVC compile warnings
/////////////////////////////////////////////////////////////////////////////////////////
#if  CG_COMPILER == CG_COMPILER_MSVC
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

#if CG_COMPILER == CG_COMPILER_GNUC
#   define CG_BEGIN_DISABLE_DEPRECATED_WARNING        _Pragma ("GCC diagnostic push") \
                                                        _Pragma ("GCC diagnostic ignored \"-Wdeprecated-declarations\"")
#   define CG_END_DISABLE_DEPRECATED_WARNING          _Pragma ("GCC diagnostic pop")

#   define CG_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING
#   define CG_END_DISABLE_CONDITIONAL_EXPR_WARNING
#if CG_COMP_VER >= 700
    #define CG_BEGIN_DISABLE_EXCEPT_TYPE_WARNING      _Pragma ("GCC diagnostic push") \
                                                        _Pragma ("GCC diagnostic ignored \"-Wnoexcept-type\"")
    #define CG_END_DISABLE_EXCEPT_TYPE_WARNING        _Pragma ("GCC diagnostic pop")
#else
    #define CG_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
    #define CG_END_DISABLE_EXCEPT_TYPE_WARNING
#endif

#if CG_COMP_VER >= 510
#   define CG_BEGIN_DISABLE_OVERRIDE_WARNING        _Pragma ("GCC diagnostic push") \
                                                      _Pragma ("GCC diagnostic ignored \"-Wsuggest-override\"")
#   define CG_END_DISABLE_OVERRIDE_WARNING          _Pragma ("GCC diagnostic pop")
# else
#   define CG_BEGIN_DISABLE_OVERRIDE_WARNING
#   define CG_END_DISABLE_OVERRIDE_WARNING
#endif

#   define CG_DECLARE_PLUGIN_CTOR       __attribute__((constructor))
#   define CG_DECLARE_PLUGIN_DTOR       __attribute__((destructor))

#elif CG_COMPILER == CG_COMPILER_CLANG || CG_COMPILER == CG_COMPILER_APPLECLANG

#if defined(__has_warning) && __has_warning("-Wdeprecated-declarations")
#   define CG_BEGIN_DISABLE_DEPRECATED_WARNING        _Pragma ("clang diagnostic push") \
                                                        _Pragma ("clang diagnostic ignored \"-Wdeprecated-declarations\"")
#   define CG_END_DISABLE_DEPRECATED_WARNING          _Pragma ("clang diagnostic pop")
#else
#   define CG_BEGIN_DISABLE_DEPRECATED_WARNING
#   define CG_END_DISABLE_DEPRECATED_WARNING
#endif

#   define CG_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING
#   define CG_END_DISABLE_CONDITIONAL_EXPR_WARNING

#if defined(__has_warning) && __has_warning("-Wnoexcept-type")
#       define CG_BEGIN_DISABLE_EXCEPT_TYPE_WARNING   _Pragma ("clang diagnostic push") \
                                                        _Pragma ("clang diagnostic ignored \"-Wnoexcept-type\"")
#       define CG_END_DISABLE_EXCEPT_TYPE_WARNING     _Pragma ("clang diagnostic pop")
#else
#       define CG_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
#       define CG_END_DISABLE_EXCEPT_TYPE_WARNING
#endif

#if defined(__has_warning) && __has_warning("-Winconsistent-missing-override")
#   define CG_BEGIN_DISABLE_OVERRIDE_WARNING          _Pragma ("clang diagnostic push") \
                                                        _Pragma ("clang diagnostic ignored \"-Winconsistent-missing-override\"")
#   define CG_END_DISABLE_OVERRIDE_WARNING            _Pragma ("clang diagnostic pop")
#else
#   define CG_BEGIN_DISABLE_OVERRIDE_WARNING
#   define CG_END_DISABLE_OVERRIDE_WARNING
#endif

#   define CG_DECLARE_PLUGIN_CTOR        __attribute__((__constructor__))
#   define CG_DECLARE_PLUGIN_DTOR        __attribute__((__destructor__))

#elif CG_COMPILER == CG_COMPILER_MSVC
#   define CG_BEGIN_DISABLE_DEPRECATED_WARNING        __pragma( warning( push )) \
                                                        __pragma( warning( disable: 4996))
#   define CG_END_DISABLE_DEPRECATED_WARNING          __pragma( warning( pop ))


#   define CG_BEGIN_DISABLE_CONDITIONAL_EXPR_WARNING  __pragma( warning( push )) \
                                                        __pragma( warning( disable: 4127))
#   define CG_END_DISABLE_CONDITIONAL_EXPR_WARNING    __pragma( warning( pop ))

#   define CG_BEGIN_DISABLE_EXCEPT_TYPE_WARNING
#   define CG_END_DISABLE_EXCEPT_TYPE_WARNING
#   define CG_DECLARE_PLUGIN_CTOR
#   define CG_DECLARE_PLUGIN_DTOR
#   define CG_BEGIN_DISABLE_OVERRIDE_WARNING
#   define CG_END_DISABLE_OVERRIDE_WARNING

#else
#   pragma message("WARNING: unknown compiler, don't know how to disable deprecated warnings")
#endif

} // end namespace rttr

#endif // CG_CORE_PREREQUISITES_H_
