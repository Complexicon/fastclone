#pragma once
#include "stdio.h"
#include "windows.h"
#define println(msg) printf("%s\n", msg)
typedef const char* cstr;
inline int MsgBox(cstr msg, cstr title = "Message", DWORD style = 0) { return MessageBoxA(0, msg, title, style); }