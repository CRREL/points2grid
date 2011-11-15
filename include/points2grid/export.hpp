#ifndef P2G_EXPORT_HPP
#define P2G_EXPORT_HPP

#ifdef _WIN32
#define P2G_DLL __declspec(dllexport)
#else
#define P2G_DLL
#endif

#endif