#pragma once

#define WIN32_LEAN_AND_MEAN             // Escludere gli elementi usati raramente dalle intestazioni di Windows
// File di intestazione di Windows
#include <windows.h>

#ifdef SMALLOBJECTALLOCATOR_EXPORTS
#define SMALLOBJECTALLOCATOR_API __declspec(dllexport)
#else
#define SMALLOBJECTALLOCATOR_API __declspec(dllimport)
#endif