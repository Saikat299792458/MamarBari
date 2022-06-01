/* A GUI editor for schedule.
 * That's it!
 * This code is not only ugly, but also toxic.
 * More formatting and abstraction is required. 
 */

#include "resource.h"

#include <bits/stdc++.h>
#include <windows.h>

using namespace std;

/****************What in the name of GOD are these???********************/
int n = 2, rdown, wx = 0, itemp = 0, init = 0, tx, ho = 0, mi = 0;
HWND hm = NULL, hx = NULL, ha = NULL, hc = NULL, cd = NULL, td = NULL;
HBITMAP cru1 = NULL, cru2 = NULL;
HDC wdc = NULL;
char font[] = "Arial Black";
COLORREF TDefault = RGB (200, 200, 200), TExited = RGB (150, 150, 150), txtbk = RGB (255, 255, 255);
HFONT hf = CreateFont (28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, font);
HFONT hop = CreateFont (15, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, font);
bool wt = false, tt = false, mousein = false, meridian = false, mov = false, issaved = false;
char chart[8][6][1000], ctemp[1000];
char path[MAX_PATH];
TRACKMOUSEEVENT tme;
/*****************Reduce # of Global variables**************************/

void GetOut ();

void save ()
{
    ofstream mq (path);
    mq << n << endl;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j <= n; j++)
            mq << chart[i][j] << endl;
    mq.close ();
}

void SetButtonLayer (int ind, HWND h, COLORREF cr, COLORREF tcr, HDC h1, UINT g)
{
    int x = ind / 10 - 1, y = ind % 10 - 1;
    RECT r1;
    GetClientRect (h, &r1);
    HFONT temp = CreateFont (28, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, font);
    SelectObject (h1, temp);
    SetBkMode (h1, TRANSPARENT);
    SetTextColor (h1, tcr);
    FillRect (h1, &r1, CreateSolidBrush (cr));
    DrawText (h1, chart[x][y], strlen (chart[x][y]), &r1, DT_SINGLELINE | DT_VCENTER | g);
    DeleteObject (temp);
}

void SetTimeUnitLayer (const char str[], COLORREF bk, HWND h)
{
    HDC h1 = GetDC (h);
    SelectObject (h1, hf);
    SetBkMode (h1, TRANSPARENT);
    RECT r1;
    GetClientRect (h, &r1);
    FillRect (h1, &r1, CreateSolidBrush (bk));
    DrawText (h1, str, strlen (str), &r1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
    ReleaseDC (h, h1);
}

void SetOptionLayer (const char str[], COLORREF bk, HWND h)
{
    HDC h1 = GetDC (h);
    SelectObject (h1, hop);
    SetBkMode (h1, TRANSPARENT);
    RECT r1;
    GetClientRect (h, &r1);
    FillRect (h1, &r1, CreateSolidBrush (bk));
    SetRect (&r1, 10, 0, r1.right, r1.bottom);
    DrawText (h1, str, strlen (str), &r1, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
    ReleaseDC (h, h1);
}

void SetOtherLayers (HWND h)
{
    HDC h1 = GetDC (h);
    HDC h2 = CreateCompatibleDC (h1);
    SelectObject (h2, cru2);
    BitBlt (h1, 0, 0, 25, 25, h2, 0, 0, SRCCOPY);
    DeleteDC (h2);
    ReleaseDC (h, h1);
}

void DestroyOption (HWND h)
{
    DestroyWindow (hx);
    DestroyWindow (ha);
    DestroyWindow (hc);
    if (wt == true)
        GetOut ();
    UpdateWindow (h);
}

void CreateChart ()
{
    CreateWindowEx (WS_EX_ACCEPTFILES, "deg", "deg",
                    WS_CHILD | WS_VISIBLE | WS_BORDER | WS_CLIPSIBLINGS, 0, 0, 0, 0, hm, HMENU (11),
                    GetModuleHandle (NULL), NULL);
    for (int i = 1; i <= 7; i++)
        CreateWindowEx (WS_EX_ACCEPTFILES, "day", "day", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0,
                        0, 0, 0, hm, HMENU ((i + 1) * 10 + 1), GetModuleHandle (NULL), NULL);
    for (int i = 1; i <= n; i++)
    {
        HWND omg = CreateWindowEx (WS_EX_ACCEPTFILES, "time", "time",
                                   WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, hm,
                                   HMENU (11 + i), GetModuleHandle (NULL), NULL);
        CreateWindowEx (WS_EX_ACCEPTFILES, "hour", "hour",
                        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, omg,
                        HMENU ((11 + i) * 10 + 1), GetModuleHandle (NULL), NULL);
        CreateWindowEx (WS_EX_ACCEPTFILES, "minute", "minute",
                        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, omg,
                        HMENU ((11 + i) * 10 + 2), GetModuleHandle (NULL), NULL);
        CreateWindowEx (WS_EX_ACCEPTFILES, "mer", "mer",
                        WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, omg,
                        HMENU ((11 + i) * 10 + 3), GetModuleHandle (NULL), NULL);
    }
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < n; j++)
            CreateWindowEx (WS_EX_ACCEPTFILES, "url", "url",
                            WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, hm,
                            HMENU ((i + 2) * 10 + j + 2), GetModuleHandle (NULL), NULL);
}

void ConfigureChart ()
{
    int d = n + 1, v = 140, w = (900 - 800) / 2;
    for (int i = 0; i < 8; i++)
        for (int j = 0; j <= d; j++)
        {
            RECT rt;
            SetWindowPos (GetDlgItem (hm, (i + 1) * 10 + (j + 1)), NULL, w + (800 / d) * j,
                          v + 40 * i, 800 / d, 40, SWP_NOZORDER);
            GetClientRect (GetDlgItem (hm, (i + 1) * 10 + (j + 1)), &rt);
            RedrawWindow (GetDlgItem (hm, (i + 1) * 10 + (j + 1)), &rt, NULL,
                          RDW_INVALIDATE | RDW_UPDATENOW);
        }
    for (int i = 1; i <= n; i++)
    {
        RECT r1;
        GetClientRect (GetDlgItem (hm, 11 + i), &r1);
        int g = (r1.right - 120) / 2;
        for (int j = 1; j < 4; j++)
        {
            RECT rt;
            HWND sp = GetDlgItem (GetDlgItem (hm, 11 + i), (11 + i) * 10 + j);
            SetWindowPos (sp, NULL, g + (j - 1) * 42, 0, 35, 40, SWP_NOZORDER);
            GetClientRect (sp, &rt);
            // RedrawWindow(sp,&rt,NULL,RDW_INVALIDATE|RDW_UPDATENOW);
        }
    }
}

void CreateColumn (int x)
{
    for (int i = n; i >= x; i--)
        for (int j = 0; j < 8; j++)
        {
            strcpy (chart[j][i + 1], chart[j][i]);
            chart[j][i + 1][strlen (chart[j][i])] = '\0';
            chart[j][i][0] = '\0';
        }
    strcpy (chart[0][x], "00:00A");
    HWND omg = CreateWindowEx (WS_EX_ACCEPTFILES, "time", "time",
                               WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0, 0, 0, hm,
                               HMENU (12 + n), GetModuleHandle (NULL), NULL);
    CreateWindowEx (WS_EX_ACCEPTFILES, "hour", "hour", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0,
                    0, 0, 0, omg, HMENU ((12 + n) * 10 + 1), GetModuleHandle (NULL), NULL);
    CreateWindowEx (WS_EX_ACCEPTFILES, "minute", "minute", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS,
                    0, 0, 0, 0, omg, HMENU ((12 + n) * 10 + 2), GetModuleHandle (NULL), NULL);
    CreateWindowEx (WS_EX_ACCEPTFILES, "mer", "mer", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0, 0,
                    0, 0, omg, HMENU ((12 + n) * 10 + 3), GetModuleHandle (NULL), NULL);
    for (int i = 0; i < 7; i++)
        CreateWindowEx (WS_EX_ACCEPTFILES, "url", "url", WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS, 0,
                        0, 0, 0, hm, HMENU ((i + 2) * 10 + n + 2), GetModuleHandle (NULL), NULL);
    n++;
}

void DeleteColumn (int x)
{
    for (int i = x; i <= n; i++)
        for (int j = 0; j < 8; j++)
        {
            strcpy (chart[j][i], chart[j][i + 1]);
            chart[j][i][strlen (chart[j][i])] = '\0';
        }
    for (int i = 1; i <= 8; i++)
        DestroyWindow (GetDlgItem (hm, i * 10 + n + 1));
    n--;
}

void GetOut ()
{
    wt = false;
    int temp = int (GetMenu (cd));
    DestroyCaret ();
    wx = 0;
    init = 0;
    RECT r1;
    GetClientRect (cd, &r1);
    FillRect (wdc, &r1, CreateSolidBrush (RGB (220, 240, 240)));
    DrawText (wdc, ctemp, strlen (ctemp), &r1, DT_SINGLELINE | DT_VCENTER);
    strcpy (chart[temp / 10 - 1][temp % 10 - 1], ctemp);
    ctemp[0] = '\0';
    cd = NULL;
    itemp = 0;
    ReleaseDC (cd, wdc);
}

LRESULT CALLBACK addpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                SetOptionLayer ("Add Column", RGB (200, 200, 255), h);
            }
            break;
        
        case WM_MOUSELEAVE:
            mousein = false;
            SetOptionLayer ("Add Column", RGB (255, 255, 255), h);
            break;
        
        case WM_LBUTTONDOWN:
            DestroyOption (NULL);
            mousein = false;
            if (n < 5)
            {
                issaved = false;
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE1));
                SetOtherLayers (GetDlgItem (hm, ID_SAVE));
                CreateColumn ((rdown % 10));
                ConfigureChart ();
            }
            else
                MessageBox (NULL, "Maximum meeting limit reached!", "Error!", MB_ICONERROR);
            break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT r1;
            GetClientRect (h, &r1);
            HDC h1 = BeginPaint (h, &ps);
            SelectObject (h1, hop);
            SetBkMode (h1, TRANSPARENT);
            SetTextColor (h1, RGB (0, 0, 0));
            FillRect (h1, &r1, CreateSolidBrush (RGB (255, 255, 255)));
            SetRect (&r1, 10, 0, r1.right, r1.bottom);
            DrawText (h1, "Add Column", 10, &r1, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK delpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                SetOptionLayer ("Delete Column", RGB (200, 200, 255), h);
            }
            break;
        
        case WM_MOUSELEAVE:
            mousein = false;
            SetOptionLayer ("Delete Column", RGB (255, 255, 255), h);
            break;
        
        case WM_LBUTTONDOWN:
            DestroyOption (NULL);
            mousein = false;
            if (n > 1)
            {
                issaved = false;
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE1));
                SetOtherLayers (GetDlgItem (hm, ID_SAVE));
                DeleteColumn ((rdown % 10) - 1);
                ConfigureChart ();
            }
            else
                MessageBox (NULL, "Can't delete anymore!", "Error!", MB_ICONERROR);
            break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT r1;
            GetClientRect (h, &r1);
            HDC h1 = BeginPaint (h, &ps);
            SelectObject (h1, hop);
            SetBkMode (h1, TRANSPARENT);
            SetTextColor (h1, RGB (0, 0, 0));
            FillRect (h1, &r1, CreateSolidBrush (RGB (255, 255, 255)));
            SetRect (&r1, 10, 0, r1.right, r1.bottom);
            DrawText (h1, "Delete Column", 13, &r1, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK clcpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                SetOptionLayer ("Clear Cell", RGB (200, 200, 255), h);
            }
            break;
        
        case WM_MOUSELEAVE:
            mousein = false;
            SetOptionLayer ("Clear Cell", RGB (255, 255, 255), h);
            break;
        
        case WM_LBUTTONDOWN:
            DestroyOption (NULL);
            issaved = false;
            cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE1));
            SetOtherLayers (GetDlgItem (hm, ID_SAVE));
            mousein = false;
            {
                HDC h1 = GetDC (GetDlgItem (hm, rdown));
                RECT r1;
                GetClientRect (GetDlgItem (hm, rdown), &r1);
                FillRect (h1, &r1, CreateSolidBrush (RGB (220, 240, 240)));
                chart[rdown / 10 - 1][rdown % 10 - 1][0] = '\0';
            }
            break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            RECT r1;
            GetClientRect (h, &r1);
            HDC h1 = BeginPaint (h, &ps);
            SelectObject (h1, hop);
            SetBkMode (h1, TRANSPARENT);
            SetTextColor (h1, RGB (0, 0, 0));
            FillRect (h1, &r1, CreateSolidBrush (RGB (255, 255, 255)));
            SetRect (&r1, 10, 0, r1.right, r1.bottom);
            DrawText (h1, "Clear Cell", 10, &r1, DT_SINGLELINE | DT_LEFT | DT_VCENTER);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK urlpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                if (cd != h)
                {
                    HDC h1 = GetDC (h);
                    SetButtonLayer (int (GetMenu (h)), h, RGB (255, 255, 255),
                                    RGB (0, 0, 0), h1, DT_LEFT);
                    ReleaseDC (h, h1);
                }
            }
            break;
        
        case WM_MOUSELEAVE:
            mousein = false;
            if (cd != h)
            {
                HDC h1 = GetDC (h);
                SetButtonLayer (int (GetMenu (h)), h, RGB (220, 240, 240), RGB (0, 0, 0), h1,
                                DT_LEFT);
                ReleaseDC (h, h1);
            }
            break;
        
        case WM_LBUTTONDOWN:
            DestroyOption (h);
            issaved = false;
            cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE1));
            SetOtherLayers (GetDlgItem (hm, ID_SAVE));
            if (wt == false)
            {
                wt = true;
                cd = h;
                wdc = GetDC (h);
                SelectObject (wdc, hf);
                SetBkMode (wdc, TRANSPARENT);
                SetTextColor (wdc, RGB (0, 0, 0));
                int temp = int (GetMenu (h)), i;
                RECT r1;
                GetClientRect (h, &r1);
                strcpy (ctemp, chart[temp / 10 - 1][temp % 10 - 1]);
                itemp = strlen (ctemp);
                for (i = strlen (ctemp) - 1; i >= 0; i--)
                {
                    int NO;
                    GetCharWidth32 (wdc, UINT (ctemp[i]), UINT (ctemp[i]), &NO);
                    if (wx + NO > r1.right - 2)
                        break;
                    wx += NO;
                }
                init = i + 1;
                char mtemp[100];
                for (i = init; i < int (strlen (ctemp)); i++)
                    mtemp[i - init] = ctemp[i];
                mtemp[i - init] = '\0';
                FillRect (wdc, &r1, CreateSolidBrush (txtbk));
                DrawText (wdc, mtemp, strlen (mtemp), &r1, DT_SINGLELINE | DT_VCENTER);
                CreateCaret (h, HBITMAP (1), 2, 30);
                SetCaretPos (wx, 0);
                ShowCaret (h);
            }
            else
                SendMessage (h, WM_LBUTTONDOWN, 0, 0);
            break;
        
        case WM_RBUTTONDOWN:
        {
            rdown = int (GetMenu (h));
            DestroyOption (h);
            POINT ps;
            RECT rf;
            GetWindowRect (hm, &rf);
            GetCursorPos (&ps);
            HWND hp = CreateWindowEx (WS_EX_ACCEPTFILES, "add", "add", WS_CHILD | WS_VISIBLE,
                                      ps.x - rf.left, ps.y - rf.top, 110, 20, hm,
                                      HMENU (ID_ADD), GetModuleHandle (NULL), NULL);
            HWND hq = CreateWindowEx (WS_EX_ACCEPTFILES, "del", "del", WS_CHILD | WS_VISIBLE,
                                      ps.x - rf.left, ps.y - rf.top + 20, 110, 20, hm,
                                      HMENU (ID_SAMPLE), GetModuleHandle (NULL), NULL);
            HWND hr = CreateWindowEx (WS_EX_ACCEPTFILES, "clc", "clc", WS_CHILD | WS_VISIBLE,
                                      ps.x - rf.left, ps.y - rf.top + 40, 110, 20, hm,
                                      HMENU (ID_CLC), GetModuleHandle (NULL), NULL);
            BringWindowToTop (hp);
            BringWindowToTop (hq);
            BringWindowToTop (hr);
            hx = hq;
            ha = hp;
            hc = hr;
        }
        break; // This wasn't here (in any procedure function, after WM_RBUTTONDOWN and before WM_PAINT)
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            SetButtonLayer (int (GetMenu (h)), h, RGB (220, 240, 240), RGB (0, 0, 0), h1,
                            DT_LEFT);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK timepro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_LBUTTONDOWN:
            DestroyOption (h);
            break;
        
        case WM_RBUTTONDOWN:
        {
            rdown = int (GetMenu (h));
            DestroyOption (h);
            POINT ps;
            RECT rf;
            GetWindowRect (hm, &rf);
            GetCursorPos (&ps);
            HWND hp = CreateWindowEx (WS_EX_ACCEPTFILES, "add", "add", WS_CHILD | WS_VISIBLE,
                                      ps.x - rf.left, ps.y - rf.top, 110, 20, hm,
                                      HMENU (ID_ADD), GetModuleHandle (NULL), NULL);
            HWND hq = CreateWindowEx (WS_EX_ACCEPTFILES, "del", "del", WS_CHILD | WS_VISIBLE,
                                      ps.x - rf.left, ps.y - rf.top + 20, 110, 20, hm,
                                      HMENU (ID_SAMPLE), GetModuleHandle (NULL), NULL);
            BringWindowToTop (hp);
            BringWindowToTop (hq);
            hx = hq;
            ha = hp;
        }
        break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            RECT r1;
            GetClientRect (h, &r1);
            FillRect (h1, &r1, CreateSolidBrush (RGB (200, 200, 200)));
            SelectObject (h1, hf);
            SetTextColor (h1, RGB (0, 0, 0));
            SetBkMode (h1, TRANSPARENT);
            SetRect (&r1, 0, 0, (r1.right - 120) / 2 + 42, 40);
            DrawText (h1, ":", 1, &r1, DT_SINGLELINE | DT_VCENTER | DT_RIGHT);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK daypro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                HDC h1 = GetDC (h);
                RECT r1;
                GetClientRect (h, &r1);
                DrawFocusRect (h1, &r1);
                ReleaseDC (h, h1);
            }
            break;
        
        case WM_MOUSELEAVE:
        {
            mousein = false;
            HDC h1 = GetDC (h);
            SetButtonLayer (int (GetMenu (h)), h, RGB (25, 25, 255), RGB (255, 255, 255), h1, DT_CENTER);
            ReleaseDC (h, h1);
        }
        break;
        
        case WM_LBUTTONDOWN:
            DestroyOption (h);
            break;
        
        case WM_RBUTTONDOWN:
        {
            rdown = int (GetMenu (h));
            DestroyOption (h);
            POINT ps;
            RECT rf;
            GetWindowRect (hm, &rf);
            GetCursorPos (&ps);
            HWND hp = CreateWindowEx (WS_EX_APPWINDOW, "add", "add", WS_CHILD | WS_VISIBLE,
                                      ps.x - rf.left, ps.y - rf.top, 100, 20, hm,
                                      HMENU (ID_ADD), GetModuleHandle (NULL), NULL);
            BringWindowToTop (hp);
            ha = hp;
        }
        break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            SetButtonLayer (int (GetMenu (h)), h, RGB (25, 25, 255), RGB (255, 255, 255), h1,
                            DT_CENTER);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK degpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_LBUTTONDOWN:
            DestroyOption (h);
            break;
        
        case WM_RBUTTONDOWN:
        {
            rdown = int (GetMenu (h));
            DestroyOption (h);
            POINT ps;
            RECT rf;
            GetWindowRect (hm, &rf);
            GetCursorPos (&ps);
            HWND hp = CreateWindowEx (WS_EX_APPWINDOW, "add", "add", WS_CHILD | WS_VISIBLE,
                                      ps.x - rf.left, ps.y - rf.top, 100, 20, hm,
                                      HMENU (ID_ADD), GetModuleHandle (NULL), NULL);
            BringWindowToTop (hp);
            ha = hp;
        }
        break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            SetButtonLayer (int (GetMenu (h)), h, RGB (230, 230, 230), RGB (0, 0, 0), h1,
                            DT_CENTER);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK bbpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                switch (int (GetMenu (h)))
                {
                    case ID_SAVE:
                        if (issaved == true)
                            return 0;
                        else
                            cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE3));
                        break;
                    case ID_SETTINGS:
                        cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SETTINGS2));
                        break;
                    case ID_INFO:
                        cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_INFO2));
                        break;
                    case ID_EXIT:
                        cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_EXIT2));
                        break;
                }
                SetOtherLayers (h);
            }
            break;
        
        case WM_MOUSELEAVE:
            mousein = false;
            switch (int (GetMenu (h)))
            {
                case ID_SAVE:
                    if (issaved == true)
                        cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE2));
                    else
                        cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE1));
                    break;
                case ID_SETTINGS:
                    cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SETTINGS1));
                    break;
                case ID_INFO:
                    cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_INFO1));
                    break;
                case ID_EXIT:
                    cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_EXIT1));
                    break;
            }
            SetOtherLayers (h);
            break;
        
        case WM_RBUTTONDOWN:
            DestroyOption (NULL);
            break;
        
        case WM_LBUTTONDOWN:
            DestroyOption (NULL);
            switch (int (GetMenu (h)))
            {
                case ID_SAVE:
                    issaved = true;
                    save ();
                    cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE2));
                    break;
                case ID_SETTINGS:
                    cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SETTINGS2));
                    break;
                case ID_INFO:
                    cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_INFO2));
                    break;
                case ID_EXIT:
                    if (issaved == false)
                    {
                        int bye = MessageBox (NULL, "Do you want to save the changes?",
                                              "Save Schedule", MB_YESNOCANCEL);
                        if (bye == 2)
                            return 0;
                        if (bye == 6)
                            save ();
                    }
                    PostQuitMessage (0);
                    return 0;
            }
            SetOtherLayers (h);
            break;
        
        case WM_PAINT:
        {
            switch (int (GetMenu (h)))
            {
            case ID_SAVE:
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE2));
                break;
            case ID_SETTINGS:
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SETTINGS1));
                break;
            case ID_INFO:
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_INFO1));
                break;
            case ID_EXIT:
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_EXIT1));
                break;
            }
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            HDC h2 = CreateCompatibleDC (h1);
            SelectObject (h2, cru2);
            BitBlt (h1, 0, 0, 25, 25, h2, 0, 0, SRCCOPY);
            DeleteDC (h2);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK hpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            DestroyOption (NULL);
            break;
        
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                int temp = int (GetMenu (h));
                char buf[3];
                temp /= 10;
                for (int i = 0; chart[temp / 10 - 1][temp % 10 - 1][i] != ':'; i++)
                {
                    buf[i] = chart[temp / 10 - 1][temp % 10 - 1][i];
                    buf[i + 1] = '\0';
                }
                ho = atoi (buf);
                SetTimeUnitLayer (buf, TExited, h);
            }
            break;
        
        case WM_MOUSELEAVE:
        {
            char buf[10];
            int temp = int (GetMenu (h)), x, s;
            temp /= 10;
            sprintf (buf, "%d", ho);
            SetTimeUnitLayer (buf, TDefault, h);
            s = strlen (buf);
            bool cop = false;
            for (unsigned int i = 0; i < strlen (chart[temp / 10 - 1][temp % 10 - 1]); i++)
            {
                if (chart[temp / 10 - 1][temp % 10 - 1][i + 1] == ':')
                {
                    cop = true;
                    x = i + 1;
                    continue;
                }
                if (cop == true)
                {
                    buf[i - x + s] = chart[temp / 10 - 1][temp % 10 - 1][i];
                    buf[i - x + s + 1] = '\0';
                }
            }
            strcpy (chart[temp / 10 - 1][temp % 10 - 1], buf);
            mousein = false;
        }
        break;
        
        case WM_MOUSEWHEEL:
        {
            DestroyOption (NULL);
            if (issaved == true)
            {
                issaved = false;
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE1));
                SetOtherLayers (GetDlgItem (hm, ID_SAVE));
            }
            ho += GET_WHEEL_DELTA_WPARAM (w) / 120;
            if (ho > 12)
                ho = 1;
            if (ho < 1)
                ho = 12;
            char buf[3];
            sprintf (buf, "%d", ho);
            SetTimeUnitLayer (buf, TExited, h);
        }
        break;
        
        case WM_PAINT:
        {
            int temp = int (GetMenu (h));
            temp /= 10;
            char buf[3];
            for (int i = 0; chart[temp / 10 - 1][temp % 10 - 1][i] != ':'; i++)
            {
                buf[i] = chart[temp / 10 - 1][temp % 10 - 1][i];
                buf[i + 1] = '\0';
            }
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            SelectObject (h1, hf);
            SetBkMode (h1, TRANSPARENT);
            RECT r1;
            GetClientRect (h, &r1);
            FillRect (h1, &r1, CreateSolidBrush (TDefault));
            DrawText (h1, buf, strlen (buf), &r1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK mpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
    {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            DestroyOption (NULL);
            break;
        
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                int temp = int (GetMenu (h)), x;
                char buf[3];
                bool cop = false;
                temp /= 10;
                for (int i = 0; chart[temp / 10 - 1][temp % 10 - 1][i] != 'A'
                                && chart[temp / 10 - 1][temp % 10 - 1][i] != 'P'; i++)
                {
                    if (chart[temp / 10 - 1][temp % 10 - 1][i] == ':')
                    {
                        cop = true;
                        x = i + 1;
                        continue;
                    }
                    if (cop == true)
                    {
                        buf[i - x] = chart[temp / 10 - 1][temp % 10 - 1][i];
                        buf[i - x + 1] = '\0';
                    }
                }
                mi = atoi (buf);
                SetTimeUnitLayer (buf, TExited, h);
            }
            break;
        
        case WM_MOUSELEAVE:
        {
            char buf[10], cut[10];
            int temp = int (GetMenu (h)), x;
            temp /= 10;
            if (mi < 10)
            {
                buf[0] = '0';
                buf[1] = '\0';
            }
            sprintf (cut, "%d", mi);
            strcat (buf, cut);
            SetTimeUnitLayer (buf, TDefault, h);
            bool cop = false;
            for (int i = 0; chart[temp / 10 - 1][temp % 10 - 1][i] != 'A'
                            && chart[temp / 10 - 1][temp % 10 - 1][i] != 'P'; i++)
            {
                if (chart[temp / 10 - 1][temp % 10 - 1][i] == ':')
                {
                    cop = true;
                    x = i + 1;
                    continue;
                }
                if (cop == true)
                {
                    chart[temp / 10 - 1][temp % 10 - 1][i] = buf[i - x];
                }
            }
            mousein = false;
        }
        break;
        
        case WM_MOUSEWHEEL:
        {
            DestroyOption (NULL);
            if (issaved == true)
            {
                issaved = false;
                cru2 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_SAVE1));
                SetOtherLayers (GetDlgItem (hm, ID_SAVE));
            }
            mi += GET_WHEEL_DELTA_WPARAM (w) / 120;
            if (mi > 59)
                mi = 0;
            if (mi < 0)
                mi = 59;
            char buf[3], cut[3];
            sprintf (cut, "%d", mi);
            buf[0] = '\0';
            if (mi < 10)
            {
                buf[0] = '0';
                buf[1] = '\0';
            }
            strcat (buf, cut);
            SetTimeUnitLayer (buf, TExited, h);
        }
        break;
        
        case WM_PAINT:
        {
            int temp = int (GetMenu (h)), x;
            temp /= 10;
            char buf[3];
            bool cop = false;
            for (int i = 0; chart[temp / 10 - 1][temp % 10 - 1][i] != 'A'
                            && chart[temp / 10 - 1][temp % 10 - 1][i] != 'P'; i++)
            {
                if (chart[temp / 10 - 1][temp % 10 - 1][i] == ':')
                {
                    cop = true;
                    x = i + 1;
                    continue;
                }
                if (cop == true)
                {
                    buf[i - x] = chart[temp / 10 - 1][temp % 10 - 1][i];
                    buf[i - x + 1] = '\0';
                }
            }
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            SelectObject (h1, hf);
            SetBkMode (h1, TRANSPARENT);
            RECT r1;
            GetClientRect (h, &r1);
            FillRect (h1, &r1, CreateSolidBrush (TDefault));
            DrawText (h1, buf, strlen (buf), &r1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint (h, &ps);
        }
        break;
        default:
            return DefWindowProc (h, u, w, l);
    }
    return 0;
}

LRESULT CALLBACK merpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    switch (u)
        {
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            DestroyOption (NULL);
            break;
        case WM_MOUSEMOVE:
            if (mousein == false)
            {
                tme.hwndTrack = h;
                TrackMouseEvent (&tme);
                mousein = true;
                int temp = int (GetMenu (h));
                temp /= 10;
                if (chart[temp / 10 - 1][temp % 10 - 1][strlen (chart[temp / 10 - 1][temp % 10 - 1]) - 1] == 'A')
                {
                    meridian = true;
                    SetTimeUnitLayer ("AM", TExited, h);
                }
                else
                {
                    meridian = false;
                    SetTimeUnitLayer ("PM", TExited, h);
                }
            }
            break;
        
        case WM_MOUSELEAVE:
        {
            int temp = int (GetMenu (h));
            temp /= 10;
            if (meridian == true)
            {
                chart[temp / 10 - 1][temp % 10 - 1][strlen (chart[temp / 10 - 1][temp % 10 - 1]) - 1] = 'A';
                SetTimeUnitLayer ("AM", TDefault, h);
            }
            else
            {
                chart[temp / 10 - 1][temp % 10 - 1][strlen (chart[temp / 10 - 1][temp % 10 - 1]) - 1] = 'P';
                SetTimeUnitLayer ("PM", TDefault, h);
            }
            mousein = false;
        }
        break;
        
        case WM_MOUSEWHEEL:
        {
            DestroyOption (NULL);
            int temp = GET_WHEEL_DELTA_WPARAM (w) / 120;
            if (temp < 0 && meridian == false)
            {
                if (issaved == true)
                {
                    issaved = false;
                    cru2 = LoadBitmap (GetModuleHandle (NULL),
                                       MAKEINTRESOURCE (ID_SAVE1));
                    SetOtherLayers (GetDlgItem (hm, ID_SAVE));
                }
                meridian = true;
                SetTimeUnitLayer ("AM", TExited, h);
            }
            if (temp > 0 && meridian == true)
            {
                if (issaved == true)
                {
                    issaved = false;
                    cru2 = LoadBitmap (GetModuleHandle (NULL),
                                       MAKEINTRESOURCE (ID_SAVE1));
                    SetOtherLayers (GetDlgItem (hm, ID_SAVE));
                }
                meridian = false;
                SetTimeUnitLayer ("PM", TExited, h);
            }
        }
        break;
        
        case WM_PAINT:
        {
            int temp = int (GetMenu (h));
            temp /= 10;
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            SelectObject (h1, hf);
            SetBkMode (h1, TRANSPARENT);
            RECT r1;
            GetClientRect (h, &r1);
            FillRect (h1, &r1, CreateSolidBrush (TDefault));
            if (chart[temp / 10 - 1][temp % 10 - 1][strlen (chart[temp / 10 - 1][temp % 10 - 1]) - 1] == 'A')
                DrawText (h1, "AM", 2, &r1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            else
                DrawText (h1, "PM", 2, &r1, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
            EndPaint (h, &ps);
        }
        break;
        
        default:
            return DefWindowProc (h, u, w, l);
        }
    return 0;
}

LRESULT CALLBACK wndpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    /* Main Window Procedure Function */
    switch (u)
        {
        case WM_TIMER:
            KillTimer (h, 9190);
            cru1 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (BD));
            RECT rt;
            GetClientRect (h, &rt);
            RedrawWindow (h, &rt, NULL, RDW_INVALIDATE | RDW_UPDATENOW);
            CreateChart ();
            ConfigureChart ();
            CreateWindowEx (WS_EX_ACCEPTFILES, "bb", "bb", WS_CHILD | WS_VISIBLE, 770, 10, 25, 25,
                            h, HMENU (ID_SAVE), NULL, NULL);
            CreateWindowEx (WS_EX_ACCEPTFILES, "bb", "bb", WS_CHILD | WS_VISIBLE, 800, 10, 25, 25,
                            h, HMENU (ID_INFO), NULL, NULL);
            CreateWindowEx (WS_EX_ACCEPTFILES, "bb", "bb", WS_CHILD | WS_VISIBLE, 830, 10, 25, 25,
                            h, HMENU (ID_SETTINGS), NULL, NULL);
            CreateWindowEx (WS_EX_ACCEPTFILES, "bb", "bb", WS_CHILD | WS_VISIBLE, 860, 10, 25, 25,
                            h, HMENU (ID_EXIT), NULL, NULL);
            break;
        
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
            DestroyOption (NULL);
            break;
        
        case WM_CREATE:
            hm = h;
            issaved = true;
            tme.cbSize = sizeof (tme);
            tme.dwFlags = TME_LEAVE;
            tme.dwHoverTime = HOVER_DEFAULT;
            cru1 = LoadBitmap (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_INTRO));
            SetTimer (h, 9190, 1000, NULL);
            break;
        
        case WM_CHAR:
            if (wt == false)
                break;
            {
                char x = w;
                HideCaret (cd);
                RECT r1;
                GetClientRect (cd, &r1);
                switch (w)
                {
                    case 0x0D:
                        GetOut ();
                        return 0;
                    
                    case 0x08:
                    {
                        if (ctemp[0] != '\0')
                        {
                            char mtemp[100];
                            int vip, mx = 0, i;
                            itemp--;
                            GetCharWidth32 (wdc, (UINT)ctemp[itemp], (UINT)ctemp[itemp], &vip);
                            ctemp[itemp] = '\0';
                            wx -= vip;
                            GetCharWidth32 (wdc, (UINT)ctemp[init - 1], (UINT)ctemp[init - 1], &mx);
                            while (mx + wx < r1.right - 2 && init > 0)
                            {
                                init--;
                                wx += mx;
                                GetCharWidth32 (wdc, (UINT)ctemp[init - 1], (UINT)ctemp[init - 1], &mx);
                            }
                            for (i = init; i < itemp; i++)
                                mtemp[i - init] = ctemp[i];
                            mtemp[i - init] = '\0';
                            FillRect (wdc, &r1, CreateSolidBrush (txtbk));
                            DrawText (wdc, mtemp, strlen (mtemp), &r1,
                                      DT_SINGLELINE | DT_VCENTER);
                        }
                    }
                    break;
                    
                    default:
                    {
                        int i, mx;
                        char mtemp[100];
                        if (int (x) == 22)
                        {
                            char ptemp[1000];
                            OpenClipboard (NULL);
                            HANDLE hcd = GetClipboardData (CF_TEXT);
                            char *pch = (char *)GlobalLock (hcd);
                            string a = pch;
                            strcpy (ptemp, a.c_str ());
                            GlobalUnlock (hcd);
                            CloseClipboard ();
                            for (unsigned int i = 0; i < strlen (ptemp); i++)
                            {
                                int no;
                                GetCharWidth32 (wdc, UINT (ptemp[i]), UINT (ptemp[i]), &no);
                                wx += no;
                                itemp++;
                            }
                            strcat (ctemp, ptemp);
                        }
                        else
                        {
                            int q;
                            ctemp[itemp] = x;
                            itemp++;
                            ctemp[itemp] = '\0';
                            GetCharWidth32 (wdc, (UINT)x, (UINT)x, &q);
                            wx += q;
                        }
                        while (wx > r1.right - 2)
                        {
                            GetCharWidth32 (wdc, (UINT)ctemp[init], (UINT)ctemp[init], &mx);
                            init++;
                            wx -= mx;
                        }
                        for (i = init; i < itemp; i++)
                            mtemp[i - init] = ctemp[i];
                        mtemp[i - init] = '\0';
                        FillRect (wdc, &r1, CreateSolidBrush (txtbk));
                        DrawText (wdc, mtemp, strlen (mtemp), &r1, DT_SINGLELINE | DT_VCENTER);
                    }
                }
                SetCaretPos (wx, 0);
                ShowCaret (cd);
            }
            break;
        
        case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC h1 = BeginPaint (h, &ps);
            HDC h2 = CreateCompatibleDC (h1);
            SelectObject (h2, cru1);
            BITMAP bmp1;
            GetObject (cru1, sizeof (bmp1), &bmp1);
            BitBlt (h1, 0, 0, bmp1.bmWidth, bmp1.bmHeight, h2, 0, 0, SRCCOPY);
            DeleteDC (h2);
            EndPaint (h, &ps);
        }
        break;
        
        case WM_DESTROY:
            PostQuitMessage (0);
            break;
        
        case WM_CLOSE:
            if (issaved == false)
            {
                int bye = MessageBox (NULL, "Do you want to save the changes?", "Save Schedule", MB_YESNOCANCEL);
                if (bye == 2)
                    return 0;
                if (bye == 6)
                    save ();
            }
            DestroyWindow (h);
            break;
        
        default:
            return DefWindowProc (h, u, w, l);
        }
    return 0;
}

APIENTRY WinMain (HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    /* Main funtion of the program */
    WNDCLASSEX e;
    e.cbClsExtra = 0;
    e.cbSize = sizeof (WNDCLASSEX);
    e.cbWndExtra = 0;
    e.hbrBackground = CreateSolidBrush (RGB (255, 255, 255));
    e.hCursor = LoadCursor (NULL, IDC_ARROW);
    e.hIcon = LoadIcon (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_ICON));
    e.hIconSm = (HICON)LoadImage (GetModuleHandle (NULL), MAKEINTRESOURCE (ID_ICON), IMAGE_ICON, 16, 16, 0);
    e.hInstance = a;
    e.lpfnWndProc = wndpro;
    e.lpszClassName = "ZoomBuddy";
    e.lpszMenuName = NULL;
    e.style = 0;
    RegisterClassEx (&e);
    e.lpfnWndProc = urlpro;
    e.hCursor = LoadCursor (NULL, IDC_IBEAM);
    e.lpszClassName = "url";
    RegisterClassEx (&e);
    e.lpfnWndProc = daypro;
    e.hCursor = LoadCursor (NULL, IDC_ARROW);
    e.lpszClassName = "day";
    RegisterClassEx (&e);
    e.lpfnWndProc = timepro;
    e.lpszClassName = "time";
    RegisterClassEx (&e);
    e.lpfnWndProc = degpro;
    e.lpszClassName = "deg";
    RegisterClassEx (&e);
    e.lpfnWndProc = bbpro;
    e.lpszClassName = "bb";
    RegisterClassEx (&e);
    e.lpfnWndProc = addpro;
    e.lpszClassName = "add";
    RegisterClassEx (&e);
    e.lpfnWndProc = delpro;
    e.lpszClassName = "del";
    RegisterClassEx (&e);
    e.lpfnWndProc = clcpro;
    e.lpszClassName = "clc";
    RegisterClassEx (&e);
    e.lpfnWndProc = hpro;
    e.lpszClassName = "hour";
    RegisterClassEx (&e);
    e.lpfnWndProc = mpro;
    e.lpszClassName = "minute";
    RegisterClassEx (&e);
    e.lpfnWndProc = merpro;
    e.lpszClassName = "mer";
    RegisterClassEx (&e);
    HWND h = CreateWindowEx (WS_EX_CLIENTEDGE, "ZoomBuddy", "", WS_OVERLAPPEDWINDOW,
                             (GetSystemMetrics (SM_CXSCREEN) - 900) / 2,
                             (GetSystemMetrics (SM_CYSCREEN) - 500) / 2, 900, 500, NULL, NULL, a,
                             NULL);
    SetWindowLong (h, GWL_STYLE, 0);
    SetWindowLong (h, GWL_EXSTYLE, 0);
    /***********************File Management Start*********************/
    string line;
    char xpath[MAX_PATH];
    GetModuleFileName (GetModuleHandle (NULL), path, MAX_PATH);
    bool printable = false;
    for (int i = strlen (path) - 1; i >= 0; i--)
        if (printable == true)
            xpath[i] = path[i];
        else if (path[i] == '\\')
        {
            printable = true;
            xpath[i] = '\0';
        }
    strcpy (path, xpath);
    strcat (path, "\\Runtime\\feeder.txt");
    ifstream mf (path);
    if (mf.is_open ())
    {
        getline (mf, line);
        int p;
        stringstream (line) >> p;
        n = p;
        int i = 0, j = -1;
        while (!mf.eof ())
        {
            j++;
            getline (mf, line);
            strcpy (chart[i][j], line.c_str ());
            if (j == n)
            {
                j = -1;
                i++;
            }
        }
        mf.close ();
    }
    else
    {
        MessageBox (NULL, "Couldn't Load Runtime Scripts!", "Error!", MB_ICONERROR);
        SendMessage (h, WM_CLOSE, 0, 0);
    }
    /***********************File Management End***********************/
    ShowWindow (h, d);
    UpdateWindow (h);
    MSG m;
    while (GetMessage (&m, NULL, 0, 0) > 0)
    {
        TranslateMessage (&m);
        DispatchMessage (&m);
    }
    return 0;
}
