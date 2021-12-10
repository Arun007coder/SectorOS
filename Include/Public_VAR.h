#ifndef __PUBLIC_VAR_H
#define __PUBLIC_VAR_H

inline char* OS_NAME = "SectorOS";

inline char* KERNEL_NAME = "SectorOS";
inline char* KERNEL_VERSION = "V2.1.6";
inline char* KERNEL_BUILD = "Build: 2021-12-10";
inline char* KERNEL_ARCH = "x86";

inline char* SHELL_NAME = "SOSH";
inline char* SHELL_VER = "V1.1.2";

// START Environment variables
inline char* SP[30]; // Shell Prompt
inline int SPIndex = 0; // Shell Prompt Index
// END Environment variables

// START Kernel variables
inline char *key_buffer[256]; // To Save the keystrokes in memory to use them later. Max number keystroke to save in the buffer is 256 Chars
inline int key_buffer_index = 0; // The index of the key_buffer
inline bool IsShellDisabled = false; // To disable the shell
// END Kernel variables

#endif