#ifndef _DEFINES_H_
#define _DEFINES_H_


#ifdef _DLLEXPORT
#define DLLEXPORT __declspec(dllexport)
#else
#define DLLEXPORT __declspec(dllimport)
#endif



#endif