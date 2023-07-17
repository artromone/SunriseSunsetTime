#pragma once

#define SGP4_DYNLINK 0

#if defined( _MSC_VER ) || defined( __MINGW32__ ) || defined( __MINGW64__ )
#    if SGP4_DYNLINK
#        if defined( SGP4_SOURCE )
#            define SGP4_DECL __declspec( dllexport )
#        else
#            define SGP4_DECL __declspec( dllimport )
#        endif
#    else
#        define SGP4_DECL
#    endif
#elif defined( __GNUC__ ) || defined( __clang__ )
#    define SGP4_DECL __attribute__( ( visibility( "default" ) ) )
#else
#    define SGP4_DECL
#endif
