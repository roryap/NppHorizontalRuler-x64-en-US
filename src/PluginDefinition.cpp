//this file is part of notepad++
//Copyright (C)2003 Don HO <donho@altern.org>
//
//This program is free software; you can redistribute it and/or
//modify it under the terms of the GNU General Public License
//as published by the Free Software Foundation; either
//version 2 of the License, or (at your option) any later version.
//
//This program is distributed in the hope that it will be useful,
//but WITHOUT ANY WARRANTY; without even the implied warranty of
//MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//GNU General Public License for more details.
//
//You should have received a copy of the GNU General Public License
//along with this program; if not, write to the Free Software
//Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.

#include "PluginDefinition.h"
#include "menuCmdID.h"

#include "dllmain.h"
#include "NppHorizontalRuler.h"
//
// The plugin data that Notepad++ needs
//
FuncItem funcItemPD[nbFunc];

//
// The data of Notepad++ that you can use in your plugin commands
//
NppData nppDataPD;

//
// Initialize your plugin data here
// It will be called while plugin loading   
void pluginInitPD(HANDLE hModule)
{
	StartConsole();
}

//
// Here you can do the clean up, save the parameters (if any) for the next session
//
void pluginCleanUp()
{
	EndConsole();
}


//
// This function help you to initialize your plugin commands
//
bool setCommandPD(size_t index, TCHAR *cmdName, PFUNCPLUGINCMD pFunc, ShortcutKey *sk, bool check0nInit)
{
	if (index >= nbFunc)
		return false;

	if (!pFunc)
		return false;

	lstrcpy(funcItemPD[index]._itemName, cmdName);
	funcItemPD[index]._pFunc = pFunc;
	funcItemPD[index]._init2Check = check0nInit;
	funcItemPD[index]._pShKey = sk;

	return true;
}

void aboutDlgPD()
{
	//int which;
	//which = SendMessage(nppData._nppHandle, NPPM_GETPOSFROMBUFFERID, -1, 0);
	//Dprintf("main %d sub %d\n", nppData._scintillaMainHandle, nppData._scintillaSecondHandle);
	//Dprintf("sub %d\n", IsWindowVisible(nppData._scintillaSecondHandle));
	//hRuler.GetRuleArea();
	//hRuler.SecureArea();
	//::MessageBox(NULL, TEXT("Hello, Notepad++!"), TEXT("Notepad++ Plugin Template"), MB_OK);
	//Dprintf("%d\n", nCharWidth);
	//Dprintf("%d", nppData._scintillaMainHandle);
	::MessageBox(NULL,
		TEXT("The author is Japanese.\n")
		TEXT("Unfortunately not in English.\n")
		TEXT("This translation also will make bing.\n")
		TEXT("\n")
		TEXT("Bugs and wishes there was other please email until freesftys@yahoo.co.jp.\n"),
		TEXT("Horizontal Ruler Plugins"),
		MB_OK);

}

//
// Initialization of your plugin commands
// You should fill your plugins commands here
void commandMenuInitPD()
{

    //--------------------------------------------//
    //-- STEP 3. CUSTOMIZE YOUR PLUGIN COMMANDS --//
    //--------------------------------------------//
    // with function :
    // setCommand(int index,                      // zero based number to indicate the order of command
    //            TCHAR *commandName,             // the command name that you want to see in plugin menu
    //            PFUNCPLUGINCMD functionPointer, // the symbol of function (function pointer) associated with this command. The body should be defined below. See Step 4.
    //            ShortcutKey *shortcut,          // optional. Define a shortcut to trigger this command
    //            bool check0nInit                // optional. Make this menu item be checked visually
    //            );
    //setCommand(0, TEXT("Hello Notepad++"), hello, NULL, false);
    //setCommand(1, TEXT("Hello (with dialog)"), helloDlg, NULL, false);
	setCommandPD(DllMainNamespace::ABOUT, TEXT("About"), aboutDlgPD, NULL, false);
	setCommandPD(DllMainNamespace::RULERTOGGLE, TEXT("RulerToggle"), RulerToggle, NULL, false);
	setCommandPD(DllMainNamespace::RULERVISIBLE, TEXT("RulerVisible"), RulerVisible, NULL, false);
	setCommandPD(DllMainNamespace::RULEROPTION, TEXT("RulerOption"), RulerOption, NULL, false);
	//setCommand(1, TEXT("test"), test, NULL, false);
}

//
// Here you can do the clean up (especially for the shortcut)
//
void commandMenuCleanUpPD()
{
	// Don't forget to deallocate your shortcut here
}



////----------------------------------------------//
////-- STEP 4. DEFINE YOUR ASSOCIATED FUNCTIONS --//
////----------------------------------------------//
//void hello()
//{
//    // Open a new document
//    ::SendMessage(nppData._nppHandle, NPPM_MENUCOMMAND, 0, IDM_FILE_NEW);
//
//    // Get the current scintilla
//    int which = -1;
//    ::SendMessage(nppData._nppHandle, NPPM_GETCURRENTSCINTILLA, 0, (LPARAM)&which);
//    if (which == -1)
//        return;
//    HWND curScintilla = (which == 0)?nppData._scintillaMainHandle:nppData._scintillaSecondHandle;
//
//    // Say hello now :
//    // Scintilla control has no Unicode mode, so we use (char *) here
//    ::SendMessage(curScintilla, SCI_SETTEXT, 0, (LPARAM)"Hello, Notepad++!");
//}


//void test()
//{
//	//hRuler.GetInitPos();
//}