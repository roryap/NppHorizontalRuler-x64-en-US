﻿/*
This file is:
The zlib/libpng License 
Copyright (c) 2012 freesftys<freesftys@gmail.com>

This software is provided 'as-is', without any express or implied warranty. 
In no event will the authors be held liable for any damages arising from 
the use of this software.

Permission is granted to anyone to use this software for any purpose, including 
commercial applications, and to alter it and redistribute it freely, subject to 
the following restrictions:

    1. The origin of this software must not be misrepresented; you must not 
	   claim that you wrote the original software. If you use this software in 
	   a product, an acknowledgment in the product documentation would be 
	   appreciated but is not required.
	
    2. Altered source versions must be plainly marked as such, and must not be 
       misrepresented as being the original software.
	
    3. This notice may not be removed or altered from any source distribution.

原文
http://opensource.org/licenses/Zlib

日本語訳
http://sourceforge.jp/projects/opensource/wiki/licenses%2Fzlib_libpng_license
*/
#ifndef NPPNORIZONTALRULER_H
#define NPPNORIZONTALRULER_H

#include "dllmain.h"

void onHorizontalRulerBeNotified(SCNotification *notifyCode);
LRESULT onHorizontalRulerMessageProc(UINT Message, WPARAM wParam, LPARAM lParam);

void RulerWndProcSet();
void RulerWndProcUnset();

void RulerToggle();
void RulerVisible();

void RulerOption();
INT_PTR CALLBACK OptionDlgProc(HWND, UINT, WPARAM, LPARAM);


#endif