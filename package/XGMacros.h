#ifndef __XG_MACROS_H__
#define __XG_MACROS_H__

#include <ctime>
#include <cstdio>
#include <cstring>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
#include <map>

#define CC_PLATFORM_WIN32 1

#define CC_PLATFORM_MAC 2



    // Apple: Mac and iOS
#if defined(__APPLE__) && !defined(ANDROID) // exclude android for binding generator.
#include <TargetConditionals.h>
#if TARGET_OS_MAC
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_MAC
#endif
#endif

// win32
#if defined(_WIN32) //&& defined(_WINDOWS)
#undef  CC_TARGET_PLATFORM
#define CC_TARGET_PLATFORM         CC_PLATFORM_WIN32
#endif


//#define CC_TARGET_PLATFORM CC_PLATFORM_WIN32

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32 
#define XG_OBLIQUE_LINE "\\"
#else
#define XG_OBLIQUE_LINE "/"
#endif

#include "tinyxml2.h"
typedef tinyxml2::XMLElement *xmlNodePtr;
typedef tinyxml2::XMLDocument *xmlDocPtr;



#define VX_FOR_EACH_XML_CHILDREN(_var, _root) \
	if(_root) for(xmlNodePtr _var = (_root)->FirstChildElement(); _var; _var = _var->NextSiblingElement())

#define VX_FOR_EACH_XML_NAMED_CHILDREN(_var, _root, _name) \
	if(_root) for(xmlNodePtr _var = (_root)->FirstChildElement(_name); _var; _var = _var->NextSiblingElement(_name))



#define XG_SINGLETON(T) static T *getInstance() \
{\
	static T * pInstance = NULL;\
	if(!pInstance)\
{\
	pInstance = new T();\
}\
	return pInstance;\
}


#define XG_RETURN_IF(a) \
	if(a) \
{\
	return; \
}


#define XG_RETURN_VALUE_IF(a,b) \
	if(a) \
{\
	return b; \
}


/** int type */
typedef unsigned int uint;


#ifdef _MSC_VER

#ifdef B2_SETTINGS_H

#else
typedef __int8  int8;
typedef __int16 int16;
typedef __int32 int32;
typedef unsigned __int8  uint8;
typedef unsigned __int16 uint16;
typedef unsigned __int32 uint32;
#endif

typedef __int64 int64;
typedef unsigned __int64 uint64;

#else

#ifdef B2_SETTINGS_H

#else
typedef int8_t  int8;
typedef int16_t int16;
typedef int32_t int32;
typedef uint8_t  uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
#endif

typedef int64_t int64;
typedef uint64_t uint64;
#endif


#endif /* __XG_DEFINE_H__ */
