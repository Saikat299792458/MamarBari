/*
 * Installer Program for Mamarbari.
 * The package comes with a products directory containing the main executables, assist files etc.
 * This program moves the file to a desired directory.
 * It also adds the runtime daemon program to startup registry, to ensure continuity.
 * This is just a test program to explore how installation works.
 * In newer programs I use Innosetup Compiler for making the installer and uninstaller programs
 */

#include "resource.h"

#include <bits/stdc++.h>
#include <shlobj.h>
#include <shlwapi.h>
#include <windows.h>

using namespace std;

char path[MAX_PATH], defpath[] = "C:\\Program Files (x86)\\MamarBari";
char rpath[] = "C:\\Program Files\\MamarBari";
bool run = false;
HANDLE htn;
HWND hmain = NULL;

DWORD WINAPI launchapp (LPVOID lp)
{
    /* Launches the Main Program */
    char gpath[MAX_PATH];
    strcpy (gpath, path);
    strcat (gpath, "\\Runtime\\feeder.txt");
    if (PathFileExists (gpath))
        cout << "Feeder Exists.\n";
    strcat (path, "\\MamarBari.exe");
    ShellExecute (NULL, "open", path, gpath, NULL, SW_SHOWNORMAL);
    PostQuitMessage (0);
    return 0;
}

void destroyer (HWND h)
{
    /* Destroyes any open child window */
    DestroyWindow (GetDlgItem (h, ID_BROWSE));
    DestroyWindow (GetDlgItem (h, ID_EDITBOX));
    DestroyWindow (GetDlgItem (h, ID_INSTALL));
    DestroyWindow (GetDlgItem (h, ID_STATIC));
    DestroyWindow (GetDlgItem (h, ID_PROGRESS));
}

bool install (HWND h)
{
    /* Worker function for installation */
    SendMessage (GetDlgItem (h, ID_PROGRESS), PBM_SETSTEP, 20, 0); // Create and update Progress Bar
    // Move desired files to selected directory
    char pupdate[250], mpath[MAX_PATH];
    GetCurrentDirectory (MAX_PATH, mpath);
    strcat (mpath, "\\Products");
    strcpy (pupdate, path);
    MoveFile (mpath, pupdate);
    // Add runtime program to registry
    HKEY hk = NULL;
    strcat (pupdate, "\\Runtime\\MBruntime.exe");
    RegCreateKey (HKEY_CURRENT_USER, "Software\\Microsoft\\Windows\\CurrentVersion\\Run", &hk);
    RegSetValueEx (hk, "MBRuntime", 0, REG_SZ, (BYTE *)pupdate, (strlen (pupdate) + 1) * 2);
    SendMessage (GetDlgItem (h, ID_PROGRESS), PBM_SETPOS, 100, 0);
    return true;
}

LRESULT CALLBACK windpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    /* Main Window Procedure Function */
    switch (u)
    {
        case WM_CREATE:
            {
                CreateWindowEx (
                    WS_EX_WINDOWEDGE, "Static",
                    "Select a Destination folder of installation:", WS_CHILD | WS_VISIBLE, 10, 20,
                    500, 40, h, (HMENU)ID_STATIC, NULL, NULL);
                HWND he = CreateWindowEx (WS_EX_CLIENTEDGE, "edit", "Browse",
                                          WS_CHILD | WS_VISIBLE | ES_NOHIDESEL | ES_AUTOHSCROLL, 75,
                                          102, 250, 26, h, (HMENU)ID_EDITBOX, NULL, NULL);
                strcpy (path, defpath);
                SetWindowText (he, path);
                SendMessage (he, EM_SETSEL, 0, 100);
                CreateWindowEx (WS_EX_WINDOWEDGE, "Button", "Browse...", WS_CHILD | WS_VISIBLE,
                                75 + 250, 100, 100, 30, h, (HMENU)ID_BROWSE, NULL, NULL);
                CreateWindowEx (WS_EX_WINDOWEDGE, "Button", "Install",
                                WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 300, 200, 80, 25, h,
                                (HMENU)ID_INSTALL, NULL, NULL);
                CreateWindowEx (WS_EX_WINDOWEDGE, "Button", "Cancel",
                                WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON, 380, 200, 80, 25, h,
                                (HMENU)ID_CANCEL, NULL, NULL);
            }
            break;
        case WM_COMMAND:
            switch (LOWORD (w))
            {
                case ID_BROWSE:
                {
                    char expath[MAX_PATH];
                    BROWSEINFO bi = { 0 };
                    LPITEMIDLIST imi = { 0 };
                    bi.lpszTitle = ("Select Destination");
                    bi.ulFlags = BIF_NEWDIALOGSTYLE | BIF_RETURNONLYFSDIRS | BIF_EDITBOX;
                    imi = SHBrowseForFolder (&bi);
                    SHGetPathFromIDList (imi, expath);
                    if (expath[0] != '\0')
                    {
                        strcpy (path, expath);
                        strcat (path, "\\MamarBari");
                    }
                    SetWindowText (GetDlgItem (h, ID_EDITBOX), path);
                    SendMessage (GetDlgItem (h, ID_EDITBOX), EM_SETSEL, 0, 250);
                }
                break;
                
                case ID_INSTALL:
                    destroyer (h);
                    CreateWindowEx (WS_EX_WINDOWEDGE, "Static",
                                    "Installation in Progress:", WS_CHILD | WS_VISIBLE, 10, 20, 500,
                                    20, h, (HMENU)ID_STATIC, NULL, NULL);
                    CreateWindowEx (WS_EX_WINDOWEDGE, "Button", "Finish",
                                    WS_CHILD | WS_VISIBLE | WS_DISABLED, 300, 200, 80, 25, h,
                                    (HMENU)ID_FINISH, NULL, NULL);
                    CreateWindowEx (WS_EX_WINDOWEDGE, PROGRESS_CLASS, "", WS_CHILD | WS_VISIBLE, 10,
                                    50, 470, 25, h, (HMENU)ID_PROGRESS, NULL, NULL);
                    install (h); // Get administrative access for file copy win32
                    CreateWindowEx (WS_EX_WINDOWEDGE, "Button", "Run MamarBari",
                                    WS_CHILD | WS_VISIBLE | BS_CHECKBOX, 10, 90, 250, 25, h,
                                    (HMENU)ID_RUN, NULL, NULL);
                    SetWindowText (GetDlgItem (h, ID_STATIC), "Installation Completed");
                    SendMessage (GetDlgItem (h, ID_RUN), BM_SETCHECK, BST_CHECKED, 0);
                    RECT r1;
                    GetClientRect (GetDlgItem (h, ID_FINISH), &r1);
                    SetWindowLong (GetDlgItem (h, ID_FINISH), GWL_STYLE,
                                   WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON);
                    SetWindowLong (GetDlgItem (h, ID_CANCEL), GWL_STYLE,
                                   WS_CHILD | WS_VISIBLE | WS_DISABLED);
                    InvalidateRect (GetDlgItem (h, ID_FINISH), &r1, false);
                    InvalidateRect (GetDlgItem (h, ID_CANCEL), &r1, false);
                    break;

                case ID_CANCEL:
                    PostQuitMessage (0);
                    break;
                
                case ID_RUN:
                    if (run == true)
                        SendMessage (GetDlgItem (h, ID_RUN), BM_SETCHECK, BST_CHECKED, 0);
                    else
                        SendMessage (GetDlgItem (h, ID_RUN), BM_SETCHECK, BST_UNCHECKED, 0);
                    run = !run;
                    break;
                
                case ID_FINISH:
                    if (run == false)
                        CreateThread (NULL, 0, launchapp, NULL, 0, NULL);
                    else
                        PostQuitMessage (0);
                    break;
            }
            break;
        case WM_KEYDOWN:
        {
            RECT r1;
            switch (w)
            {
                case VK_RIGHT:
                    SetWindowLong (GetDlgItem (h, ID_INSTALL), GWL_STYLE,
                                   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);
                    SetWindowLong (GetDlgItem (h, ID_CANCEL), GWL_STYLE,
                                   WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON);
                    GetClientRect (GetDlgItem (h, ID_INSTALL), &r1);
                    InvalidateRect (GetDlgItem (h, ID_INSTALL), &r1, false);
                    GetClientRect (GetDlgItem (h, ID_CANCEL), &r1);
                    InvalidateRect (GetDlgItem (h, ID_CANCEL), &r1, false);
                    break;
                
                case VK_LEFT:
                    SetWindowLong (GetDlgItem (h, ID_INSTALL), GWL_STYLE,
                                   WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON);
                    SetWindowLong (GetDlgItem (h, ID_CANCEL), GWL_STYLE,
                                   WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON);
                    GetClientRect (GetDlgItem (h, ID_INSTALL), &r1);
                    InvalidateRect (GetDlgItem (h, ID_INSTALL), &r1, false);
                    GetClientRect (GetDlgItem (h, ID_CANCEL), &r1);
                    InvalidateRect (GetDlgItem (h, ID_CANCEL), &r1, false);
                    break;
            }
        }
        break;
        
        case WM_CLOSE:
            CloseHandle (htn);
            DestroyWindow (h);
            break;
        
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

DWORD WINAPI launchadmin (LPVOID lp)
{
    /* Relaunch the program as admin */
    char curpath[MAX_PATH];
    GetModuleFileName (NULL, curpath, MAX_PATH);
    ShellExecute (NULL, "runas", curpath, "", NULL, SW_SHOWNORMAL);
    SendMessage (hmain, WM_CLOSE, 0, 0);
    return 0;
}

APIENTRY WinMain (HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    /* Main function of the program */
    WNDCLASSEX e;
    e.cbClsExtra = 0;
    e.cbSize = sizeof (WNDCLASSEX);
    e.cbWndExtra = 0;
    e.hbrBackground = (HBRUSH)COLOR_WINDOW;
    e.hCursor = LoadCursor (NULL, IDC_ARROW);
    e.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    e.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    e.hInstance = a;
    e.lpfnWndProc = windpro;
    e.lpszClassName = "INSTALLER";
    e.lpszMenuName = NULL;
    e.style = 0;
    RegisterClassEx (&e);
    int x = GetSystemMetrics (SM_CXSCREEN), y = GetSystemMetrics (SM_CYSCREEN);
    HWND h = CreateWindowEx (WS_EX_CLIENTEDGE, "INSTALLER", "MamarBari Setup",
                             WS_OVERLAPPEDWINDOW ^ WS_THICKFRAME, (x - 500) / 2, (y - 300) / 2, 500,
                             300, NULL, NULL, a, NULL);
    
    bool isadmin = false;
    ifstream mf ("AdminAccess.txt");
    if (mf.is_open ())
        {
            string number;
            getline (mf, number);
            stringstream (number) >> isadmin;
            mf.close ();
        }
    ofstream mc ("AdminAccess.txt");
    if (mc.is_open ())
        {
            mc << !isadmin;
            mc.close ();
        }
    if (isadmin == false)
        {
            hmain = h;
            htn = CreateThread (NULL, 0, launchadmin, NULL, 0, NULL);
        }
    else
        {
            ShowWindow (h, d);
            UpdateWindow (h);
        }
    
    MSG m;
    while (GetMessage (&m, NULL, 0, 0) > 0)
        {
            TranslateMessage (&m);
            DispatchMessage (&m);
        }
    return 0;
}
