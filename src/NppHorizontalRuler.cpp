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

#include "NppHorizontalRuler.h"
#include "HorizontalRuler.h"
#include "resource.h"

#include <Tchar.h>
#include <Commctrl.h>

extern HINSTANCE g_DllhInst;
extern NppData nppData;
extern FuncItem funcItem[];

HorizontalRuler mainHRuler;
HorizontalRuler subHRuler;

HWND mainTabHwnd;
WNDPROC mainOldWndProc;
LRESULT CALLBACK RulerMainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );

HWND subTabHwnd;
WNDPROC subOldWndProc;
LRESULT CALLBACK RulerSubWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam );


void onHorizontalRulerBeNotified(SCNotification *notifyCode)
{
	static int nHscrollPos;
	SCROLLINFO si;
	switch(notifyCode->nmhdr.code)
	{
	case SCN_UPDATEUI:
		if(notifyCode->nmhdr.hwndFrom == nppData._scintillaMainHandle)
		{
			if( mainHRuler.IsInit() )
			{
				Dprintf("SCN_UPDATEUI\n");
				//現在のスクロール場所の取得
				memset(&si, 0, sizeof(SCROLLINFO));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;// SIF_TRACKPOS POSと値が一緒だった
				GetScrollInfo(nppData._scintillaMainHandle, SB_HORZ, &si);
				nHscrollPos = si.nPos;
				mainHRuler.PaintRuler();
			}
		}
		else if(notifyCode->nmhdr.hwndFrom == nppData._scintillaSecondHandle)
		{
			if( subHRuler.IsInit() )
			{
				Dprintf("SCN_UPDATEUI\n");
				//現在のスクロール場所の取得
				memset(&si, 0, sizeof(SCROLLINFO));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;// SIF_TRACKPOS POSと値が一緒だった
				GetScrollInfo(nppData._scintillaSecondHandle, SB_HORZ, &si);
				nHscrollPos = si.nPos;
				subHRuler.PaintRuler();
			}
		}
		break;
	case SCN_SCROLLED:
		if(notifyCode->nmhdr.hwndFrom == nppData._scintillaMainHandle)
		{
			if( mainHRuler.IsInit() )
			{
				Dprintf("main SCN_SCROLLED\n");
				//現在のスクロール場所の取得
				memset(&si, 0, sizeof(SCROLLINFO));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;// SIF_TRACKPOS POSと値が一緒だった
				GetScrollInfo(nppData._scintillaMainHandle, SB_HORZ, &si);
				if(si.nPos != nHscrollPos)
				{
					nHscrollPos = si.nPos;
					mainHRuler.PaintRuler();
				}
			}
		}
		else if(notifyCode->nmhdr.hwndFrom == nppData._scintillaSecondHandle)
		{
			if( subHRuler.IsInit() )
			{
				Dprintf("sub SCN_SCROLLED\n");
				//現在のスクロール場所の取得
				memset(&si, 0, sizeof(SCROLLINFO));
				si.cbSize = sizeof(SCROLLINFO);
				si.fMask = SIF_POS;// SIF_TRACKPOS POSと値が一緒だった
				GetScrollInfo(nppData._scintillaSecondHandle, SB_HORZ, &si);
				if(si.nPos != nHscrollPos)
				{
					nHscrollPos = si.nPos;
					subHRuler.PaintRuler();
				}
			}
		}
		break;
	case NPPN_WORDSTYLESUPDATED:
		Dprintf("main NPPN_WORDSTYLESUPDATED\n");
		if( mainHRuler.IsInit() )
		{
			mainHRuler.GetRuleArea();
			mainHRuler.SecureArea();
			mainHRuler.PaintRuler();
		}
		Dprintf("sub NPPN_WORDSTYLESUPDATED\n");
		if( subHRuler.IsInit() )
		{
			subHRuler.GetRuleArea();
			subHRuler.SecureArea();
			subHRuler.PaintRuler();
		}
		break;
	case NPPN_READY:
		//	Dprintf("NPPN_READY\n");
		RulerWndProcSet();
		mainHRuler.Init(nppData._nppHandle, nppData._scintillaMainHandle, mainTabHwnd);
		subHRuler.Init(nppData._nppHandle, nppData._scintillaSecondHandle, subTabHwnd);
		if( mainHRuler.GetEnable() )
			SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[DllMainNamespace::RULERVISIBLE]._cmdID, TRUE);
		else
			SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[DllMainNamespace::RULERVISIBLE]._cmdID, FALSE);

		SendMessage(nppData._nppHandle, WM_SIZE, 0, 0);
		break;
	case NPPN_SHUTDOWN:
		RulerWndProcUnset();
		break;
	//default:
	//	Dprintf("0x%x     %d %hx %hu  %d\n",notifyCode->nmhdr.code, notifyCode->nmhdr.code, notifyCode->nmhdr.code, notifyCode->nmhdr.code, notifyCode->message);
	//	break;
	}
	return;
}

LRESULT onHorizontalRulerMessageProc(UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
	case WM_SIZE:
		if( wParam == SIZE_MINIMIZED )
			break;

		Dprintf("WM_SIZE\n");
		if( mainHRuler.IsInit() )
		{
			Dprintf(" main-\n");
			mainHRuler.GetInitPos();
			mainHRuler.GetRuleArea();
			mainHRuler.SecureArea();
			mainHRuler.PaintRuler();
		}
		if( subHRuler.IsInit() )
		{
			Dprintf(" sub-\n");
			subHRuler.GetInitPos();
			subHRuler.GetRuleArea();
			subHRuler.SecureArea();
			subHRuler.PaintRuler();
		}
		return TRUE;
	}
	return TRUE;
}


void RulerWndProcSet()
{	
	mainTabHwnd = FindWindowEx(nppData._nppHandle, NULL, (LPCWSTR)L"systabcontrol32", NULL);
	mainOldWndProc = (WNDPROC)SetWindowLongPtr(mainTabHwnd, GWLP_WNDPROC, (LONG_PTR)RulerMainWndProc);
	
	subTabHwnd = FindWindowEx(nppData._nppHandle, mainTabHwnd, (LPCWSTR)L"systabcontrol32", NULL);
	subOldWndProc = (WNDPROC)SetWindowLongPtr(subTabHwnd, GWLP_WNDPROC, (LONG_PTR)RulerSubWndProc);

	return;
}
void RulerWndProcUnset()
{
	SetWindowLongPtr(mainTabHwnd, GWLP_WNDPROC, (LONG_PTR)mainOldWndProc);
	SetWindowLongPtr(subTabHwnd, GWLP_WNDPROC, (LONG_PTR)subOldWndProc);

	return;
}
LRESULT CALLBACK RulerMainWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	if(hwnd == subTabHwnd)
		return CallWindowProc(mainOldWndProc, hwnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_DRAWITEM:
		Dprintf("main WM_DRAWITEM \n");
	case WM_PAINT:
		Dprintf("main WM_PAINT\n");
		if( mainHRuler.IsInit() )
			mainHRuler.PaintRuler();
		break;
	case WM_NCLBUTTONDBLCLK:
		Dprintf("main WM_NCLBUTTONDBLCLK x %d y %d\n", LOWORD(lParam), HIWORD(lParam));
		if( mainHRuler.IsInit() )
			mainHRuler.EdgeLine(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_NCHITTEST:
		Dprintf("main WM_NCHITTEST\n");
		if( mainHRuler.IsInit() )
		{
			if( mainHRuler.HitDrawArea(LOWORD(lParam), HIWORD(lParam)) )
				return HTBORDER;
		}
		break;
	//ドッキングボックスの枠の移動時の再描画で有効なメッセージがなくしょうがないので
	//下記の GETCURSEL GETIMAGELIST GETITEMWの内一つを利用するとりあえず GETIMAGELIST
	case TCM_GETIMAGELIST:
		Dprintf("main TCM_GETIMAGELIST\n");
		if( mainHRuler.IsInit() )
		{
			mainHRuler.GetInitPos();
			mainHRuler.GetRuleArea();
			mainHRuler.SecureArea();
			mainHRuler.PaintRuler();
		}
		break;
	default:
		Dprintf("main wnd 0x%x %d\n", uMsg, uMsg );
		break;
	}
	return CallWindowProc(mainOldWndProc, hwnd, uMsg, wParam, lParam);
}

LRESULT CALLBACK RulerSubWndProc( HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam )
{	
	if(hwnd == mainTabHwnd)
		return CallWindowProc(subOldWndProc, hwnd, uMsg, wParam, lParam);

	switch(uMsg)
	{
	case WM_DRAWITEM:
		Dprintf("sub WM_DRAWITEM \n");
	case WM_PAINT:
		Dprintf("sub WM_PAINT\n");
		if( subHRuler.IsInit() )
			subHRuler.PaintRuler();
		break;
	case WM_NCLBUTTONDBLCLK:
		if( subHRuler.IsInit() )
			subHRuler.EdgeLine(LOWORD(lParam), HIWORD(lParam));
		break;
	case WM_NCHITTEST:
		if( subHRuler.IsInit() )
		{
			if( subHRuler.HitDrawArea(LOWORD(lParam), HIWORD(lParam)) )
				return HTBORDER;
		}
		break;
	case WM_MOUSEACTIVATE:
		return MA_ACTIVATE;
		break;
	case TCM_GETIMAGELIST:
		Dprintf("sub TCM_GETIMAGELIST\n");
		if( subHRuler.IsInit() )
		{
			subHRuler.GetInitPos();
			subHRuler.GetRuleArea();
			subHRuler.SecureArea();
			subHRuler.PaintRuler();
		}
		break;
	}
	return CallWindowProc(subOldWndProc, hwnd, uMsg, wParam, lParam);
}



void RulerToggle()
{
	mainHRuler.SetEnable( !mainHRuler.GetEnable());
	subHRuler.SetEnable( !subHRuler.GetEnable());

	if( mainHRuler.GetEnable() == 1 )
	{
		SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[DllMainNamespace::RULERVISIBLE]._cmdID, TRUE);
	}
	else
	{
		SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[DllMainNamespace::RULERVISIBLE]._cmdID, FALSE);
	}
	mainHRuler.SendSizeToMain();
	return;
}
void RulerVisible()
{
	mainHRuler.SetEnable(1);
	subHRuler.SetEnable(1);
	mainHRuler.SendSizeToMain();
	SendMessage(nppData._nppHandle, NPPM_SETMENUITEMCHECK, funcItem[DllMainNamespace::RULERVISIBLE]._cmdID, TRUE);
	return;
}


void RulerOption()
{
	if( DialogBox(g_DllhInst, MAKEINTRESOURCE(IDD_DIALOG_RULER_OPTION), nppData._nppHandle, OptionDlgProc) == 1)
	{
		if( mainHRuler.IsInit() )
		{
			mainHRuler.GetRuleArea();
			mainHRuler.SecureArea();
			mainHRuler.PaintRuler();
		}
		if( subHRuler.IsInit() )
		{
			subHRuler.GetRuleArea();
			subHRuler.SecureArea();
			subHRuler.PaintRuler();
		}
	}
	return ;
}
INT_PTR CALLBACK OptionDlgProc(HWND hWndDlg, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch(uMsg)
	{
	case WM_INITDIALOG:
		HWND comboHwnd;
		TCHAR tsSize[5];

		comboHwnd = GetDlgItem(hWndDlg, IDC_COMBO_FIX_FONT_SIZE);
		for(int i=8; i<25; i++)
		{
			_itot_s(i, tsSize, 5, 10);
			SendMessage(comboHwnd, CB_ADDSTRING, 0, (LPARAM)tsSize);
			if( mainHRuler.nFontSize == i)
			{
				int index;

				index = (int)SendMessage(comboHwnd, CB_GETCOUNT, 0, 0);
				SendMessage(comboHwnd, CB_SETCURSEL, --index, 0);
			}
		}

		if( mainHRuler.bFontFix == true )
		{
			SendMessage(GetDlgItem(hWndDlg, IDC_RADIO_FIX_FONT_SIZE), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			EnableWindow( GetDlgItem(hWndDlg, IDC_COMBO_FIX_FONT_SIZE), TRUE);
		}
		else
		{
			SendMessage(GetDlgItem(hWndDlg, IDC_RADIO_FONT_EDITOR_SIZE), BM_SETCHECK, (WPARAM)BST_CHECKED, 0);
			EnableWindow( GetDlgItem(hWndDlg, IDC_COMBO_FIX_FONT_SIZE), FALSE);
		}

		break;
	case WM_COMMAND:
		switch(LOWORD(wParam))
		{
		case IDOK:
			int radio;
			radio = (int)SendMessage(GetDlgItem(hWndDlg, IDC_RADIO_FIX_FONT_SIZE), BM_GETCHECK, 0, 0);
			if( radio == TRUE )
			{

				mainHRuler.bFontFix = true;
				subHRuler.bFontFix = true;
			}
			else
			{
				mainHRuler.bFontFix = false;
				subHRuler.bFontFix = false;
			}
			int nComboIndex;
			int nSize;
			TCHAR buf[5];
			nComboIndex = (int)SendMessage( GetDlgItem(hWndDlg, IDC_COMBO_FIX_FONT_SIZE), CB_GETCURSEL, 0, 0);
			SendMessage( GetDlgItem(hWndDlg, IDC_COMBO_FIX_FONT_SIZE), CB_GETLBTEXT, nComboIndex, (LPARAM)buf);
			nSize = _ttoi(buf);
			mainHRuler.nFontSize = nSize;
			subHRuler.nFontSize = nSize;

			EndDialog(hWndDlg, 1);
			break;

		case IDCANCEL:
			EndDialog(hWndDlg, 0);
			break;

		case IDC_RADIO_FIX_FONT_SIZE:
			EnableWindow( GetDlgItem(hWndDlg, IDC_COMBO_FIX_FONT_SIZE), TRUE);
			break;
		case IDC_RADIO_FONT_EDITOR_SIZE:
			EnableWindow( GetDlgItem(hWndDlg, IDC_COMBO_FIX_FONT_SIZE), FALSE);
			break;
		}
		break;
	case WM_CLOSE:
		EndDialog(hWndDlg, 0);
		return TRUE;
	}
	return FALSE;
}