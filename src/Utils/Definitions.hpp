#ifndef UTILS__DEFINITIONS_HPP_
#define UTILS__DEFINITIONS_HPP_


/**
 *@cond
 */
    
/**
 *@brief Recognize operating system: Linux 
 */
#if defined (linux) || defined (__linux__)
#define OS_LINUX
#endif
    
/**
 *@brief Recognize operating system: Microsoft Windows
 */
#if defined (WINDOWS) || defined (_WIN32) || defined (__WIN32__)
#define OS_WINDOWS 
#endif
    
/**
 *@brief Recognize operating system: Macintosh OS
 */
#ifdef __APPLE__
#define OS_MAC
#endif
    
/**
 *@brief Recognize operating system: SunOS/Solaris
 */
#if defined(sun) || defined(__sun)
#define OS_SUNOS
#endif
    
/**
 *@brief Recognize compiler: GNU C/C++
 */
#ifdef __GNUC__
#define COMPILER_GNU
#endif
    
   
/**
 *@brief Recognize compiler: Microsoft Visual Studio 
 */
#ifdef _MSC_VER
#define COMPILER_VISUAL_STUDIO
#endif
    
    
/**
 *@brief Make Visual Studio happy: avoid warnings/errors about
 *       <em>strdup</em> and <em>inline</em> 
 */
#ifdef COMPILER_VISUAL_STUDIO
#define _USE_MATH_DEFINES
#define strdup _strdup
#define inline __inline
#include <cstdlib>
#include <cmath>

#ifdef _MSC_VER >= 1400
  #define _CRT_SECURE_NO_DEPRECATE
  #define COMPILER_VISUAL_STUDIO_2005_OR_LATER
#else
  #define NO_VARIADIC_MACROS
#endif

#endif
    
    
/**
 *@endcond
 */



#endif
