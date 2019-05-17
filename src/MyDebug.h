/*
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

#ifndef MYDEBUG_H
#define MYDEBUG_H

#ifndef _DEBUG
	// __noop にて無効化　引数ありの場合は何も書かなくても良い？
	#define StartConsole __noop
	#define EndConsole __noop
	#define Dprintf __noop
#else
	#define StartConsole _StartConsole
	#define EndConsole _EndConsole
	#define Dprintf _Dprintf

	//メモリリーク検査
	#define _CRTDBG_MAP_ALLOC
	#include <stdlib.h>
	#include <crtdbg.h>
	#include <locale.h>
	#include <stdio.h>
	#include <tchar.h>
	
	//AllocConsoleなど用
	#include <wincon.h>

	#define BUFFER_SIZE 256


	inline void _StartConsole()
	{
		//セキュリティ強化用
		FILE *fp;

		//メモリリーク検知
		_CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );

		//ロケールの設定
		setlocale( LC_ALL, "English" );
		
		//コンソール呼び出し
		::AllocConsole();
		//標準入出力設定
		freopen_s(&fp, "CON", "r", stdin);
		freopen_s(&fp, "CON", "w", stdout);
		//freopen("CON", "r", stdin);
		//freopen("CON", "w", stdout);

	}
	inline void _EndConsole()
	{
		//コンソール消去
		::FreeConsole();
	}
	
	//インライン関数を使うかどうか　INLINEDPRINTが定義されていたら使用する
	#if !defined(INLINEDPRINT)
		#if defined(UNICODE) || defined(_UNICODE)
			#define _Dprintf(format, ...) wprintf(L ## format, __VA_ARGS__)
		#else
			#define _Dprintf(format, ...) printf(format, __VA_ARGS__)
		#endif
	#else
		#if defined(UNICODE) || defined(_UNICODE)
			//#define _Dprintf __Dprintf
			#define _Dprintf(format, ...) __Dprintf(L ## format, __VA_ARGS__)
			inline void __Dprintf(const wchar_t* format, ...)
			{
				wchar_t buf[BUFFER_SIZE];
				va_list args;
				va_start(args, format);
				_vsnwprintf_s(buf, BUFFER_SIZE, _TRUNCATE, format, args);
				va_end(args);
				wprintf(L"%s", buf);
			}
		#else
			//#define _Dprintf(format, ...) __Dprintf(format, __VA_ARGS__)
			#define _Dprintf __Dprintf
			inline void __Dprintf(const char* format, ...)
			{
				char buf[BUFFER_SIZE];
				va_list args;
				va_start(args, format);
				vsnprintf_s(buf, BUFFER_SIZE, _TRUNCATE, format, args);
				va_end(args);
				printf("%s", buf);
			}
		#endif
	#endif
#endif

#endif