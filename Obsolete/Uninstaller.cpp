#include<windows.h>
#include<shlobj.h>
#include<shlwapi.h>
#include"resource.h"
#include<bits/stdc++.h>
using namespace std;
char path[MAX_PATH],defpath[]="C:\\Program Files\\MamarBari";
char rpath[]="%appdata%\\Microsoft\\Windows\\Program Files";
void destroyer(HWND h)
{
    DestroyWindow(GetDlgItem(h,ID_UNINSTALL));
    DestroyWindow(GetDlgItem(h,ID_STATIC));
    DestroyWindow(GetDlgItem(h,ID_PROGRESS));
}
bool uninstall(HWND h)
{
    SendMessage(GetDlgItem(h,ID_PROGRESS),PBM_SETSTEP,20,0);
    char pupdate[250],mpath[MAX_PATH];
    GetCurrentDirectory(MAX_PATH,mpath);
    if(RemoveDirectory(mpath))
        cout<<"Yes\n";
    /*strcat(mpath,"\\Assets");
    strcpy(pupdate,path);
    MoveFile(mpath,pupdate);
    SendMessage(GetDlgItem(h,ID_PROGRESS),PBM_STEPIT,0,0);
    GetCurrentDirectory(MAX_PATH,mpath);
    strcat(mpath,"\\Runtime");
    strcat(pupdate,"\\Runtime");
    MoveFile(mpath,pupdate);
    SendMessage(GetDlgItem(h,ID_PROGRESS),PBM_STEPIT,0,0);
    HKEY hk=NULL;
    strcat(pupdate,"\\MBruntime.exe");
    RegCreateKey(HKEY_CURRENT_USER,"Software\\Microsoft\\Windows\\CurrentVersion\\Run",&hk);
    RegSetValueEx(hk,"MBRuntime",0,REG_SZ,(BYTE*)pupdate,(strlen(pupdate)+1)*2);*/
    SendMessage(GetDlgItem(h,ID_PROGRESS),PBM_STEPIT,0,0);
    return true;
}
LRESULT CALLBACK windpro(HWND h,UINT u,WPARAM w,LPARAM l)
{
    switch(u)
    {
    case WM_CREATE:
        {
        CreateWindowEx(WS_EX_WINDOWEDGE,
                                   "Static","Are you sure to remove MamarBari1.0 permanently?",
                                   WS_CHILD|WS_VISIBLE,
                                   10,20,500,40,
                                   h,(HMENU)ID_STATIC,
                                   NULL,NULL);
        CreateWindowEx(WS_EX_WINDOWEDGE,
                                   "Button","Uninstall",
                                   WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON,
                                   300,120,80,25,
                                   h,(HMENU)ID_UNINSTALL,
                                   NULL,NULL);
        CreateWindowEx(WS_EX_WINDOWEDGE,
                                   "Button","Cancel",
                                   WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON,
                                   380,120,80,25,
                                   h,(HMENU)ID_CANCEL,
                                   NULL,NULL);
        }
        break;
    case WM_COMMAND:
        switch(LOWORD(w))
        {
        case ID_UNINSTALL:
            destroyer(h);
            CreateWindowEx(WS_EX_WINDOWEDGE,
                                   "Static","Unnstallation in Progress:",
                                   WS_CHILD|WS_VISIBLE,
                                   10,20,500,20,
                                   h,(HMENU)ID_STATIC,
                                   NULL,NULL);
            CreateWindowEx(WS_EX_WINDOWEDGE,
                                   PROGRESS_CLASS,"",
                                   WS_CHILD|WS_VISIBLE,
                                   10,50,470,25,
                                   h,(HMENU)ID_PROGRESS,
                                   NULL,NULL);
            uninstall(h);
            break;
        case ID_CANCEL:
            break;
        }
        break;
    case WM_KEYDOWN:
        {
            RECT r1;
            switch(w)
            {
            case VK_RIGHT:
                SetWindowLong(GetDlgItem(h,ID_UNINSTALL),GWL_STYLE,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON);
                SetWindowLong(GetDlgItem(h,ID_CANCEL),GWL_STYLE,WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON);
                GetClientRect(GetDlgItem(h,ID_UNINSTALL),&r1);
                InvalidateRect(GetDlgItem(h,ID_UNINSTALL),&r1,false);
                GetClientRect(GetDlgItem(h,ID_CANCEL),&r1);
                InvalidateRect(GetDlgItem(h,ID_CANCEL),&r1,false);
                break;
            case VK_LEFT:
                SetWindowLong(GetDlgItem(h,ID_UNINSTALL),GWL_STYLE,WS_CHILD|WS_VISIBLE|BS_DEFPUSHBUTTON);
                SetWindowLong(GetDlgItem(h,ID_CANCEL),GWL_STYLE,WS_CHILD|WS_VISIBLE|BS_PUSHBUTTON);
                GetClientRect(GetDlgItem(h,ID_UNINSTALL),&r1);
                InvalidateRect(GetDlgItem(h,ID_UNINSTALL),&r1,false);
                GetClientRect(GetDlgItem(h,ID_CANCEL),&r1);
                InvalidateRect(GetDlgItem(h,ID_CANCEL),&r1,false);
                break;
            }
        }
            break;
    case WM_CLOSE:
        //RemoveDirectory(path);
        DestroyWindow(h);
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(h,u,w,l);
    }
    return 0;
}
APIENTRY WinMain(HINSTANCE a,HINSTANCE b,LPSTR c,int d)
{
WNDCLASSEX e;
e.cbClsExtra=0;
e.cbSize=sizeof(WNDCLASSEX);
e.cbWndExtra=0;
e.hbrBackground=(HBRUSH)COLOR_WINDOW;
e.hCursor=LoadCursor(NULL,IDC_ARROW);
e.hIcon=LoadIcon(NULL,IDI_WARNING);
e.hIconSm=LoadIcon(NULL,IDI_WARNING);
e.hInstance=a;
e.lpfnWndProc=windpro;
e.lpszClassName="UNINSTALLER";
e.lpszMenuName=NULL;
e.style=0;
RegisterClassEx(&e);
int x=GetSystemMetrics(SM_CXSCREEN),y=GetSystemMetrics(SM_CYSCREEN);
HWND h=CreateWindowEx(WS_EX_CLIENTEDGE,
                      "UNINSTALLER",
                      "Uninstall MamarBari",
                      WS_OVERLAPPEDWINDOW^WS_THICKFRAME,
                      (x-500)/2,
                      (y-300)/2,
                      500,
                      200,
                      NULL,NULL,a,NULL);
ShowWindow(h,d);
UpdateWindow(h);
MSG m;
while(GetMessage(&m,NULL,0,0)>0)
{
    TranslateMessage(&m);
    DispatchMessage(&m);
}
return 0;
}

