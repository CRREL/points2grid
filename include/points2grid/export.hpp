#pragma once

#ifdef _WIN32
#if defined(P2G_DLL_EXPORT)
#   define P2G_DLL __declspec(dllexport)
#elif defined(P2G_DLL_IMPORT)
#   define P2G_DLL   __declspec(dllimport)
#else
#   define P2G_DLL
#endif
#endif