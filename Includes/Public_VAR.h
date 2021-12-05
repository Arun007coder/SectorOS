#ifndef __PUBLIC_VAR_H
#define __PUBLIC_VAR_H

inline char* OS_NAME = "SectorOS";

inline char* KERNEL_NAME = "SectorOS";
inline char* KERNEL_VERSION = "V2.1.4";
inline char* KERNEL_BUILD = "Build: 2021-12-05";
inline char* KERNEL_ARCH = "x86";

inline char* SHELL_NAME = "SOSH";
inline char* SHELL_VER = "V1.1.2";

// START Environment variables
inline char* SP[30]; // Shell Prompt
inline int SPIndex = 0; // Shell Prompt Index
// END Environment variables

#endif