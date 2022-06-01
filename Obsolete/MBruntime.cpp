/*
 * Background Process for Mamarbari.
 * Reads schedule and launches meeting in time
 * A few features could not be implemented like overriding fullscreen windows,
 * direct launch (without the help of a third party software, i.e. Internet Browser) etc.
 */

#include "resource.h"
#include <bits/stdc++.h>
#include <windows.h>

#define TIMERID 1000

using namespace std;

int p, freq, CurrentTimeIndex = 0, x;
char chart[8][5][100], url[1000], mrl[1000];
HANDLE ht = NULL;

DWORD WINAPI launch (LPVOID lp)
{
    /* Launch the Meeting */
    system (mrl);
    return 0;
}

bool VerifyTime (int n)
{
    /* Check if it's time to launch meeting */
    SYSTEMTIME st;
    GetLocalTime (&st);
    int lt = 0, ct = 0;
    lt += (int (st.wHour) * 3600000);
    lt += (int (st.wMinute) * 60000);
    lt += (int (st.wSecond) * 1000);
    char hr[3], mn[3];
    int j;
    for (j = 0; chart[0][n][j] != ':'; j++)
    {
        hr[j] = chart[0][n][j];
    }
    hr[j] = '\0';
    j++;
    int lim = j;
    while (chart[0][n][j] != 'A' && chart[0][n][j] != 'P')
    {
        mn[j - lim] = chart[0][n][j];
        j++;
    }
    mn[j - lim] = '\0';
    int thr = atoi (hr);
    if (chart[0][n][j] == 'A')
    {
        if (thr == 12)
        {
            thr = 0;
        }
    }
    else
    {
        if (thr != 12)
        {
            thr += 12;
        }
    }
    ct += (thr * 3600000);
    ct += (atoi (mn) * 60000);
    if (ct - lt <= 0 || chart[int (st.wDayOfWeek) + 1][n][0] == '\0')
    {
        return false;
    }
    else
    {
        url[strlen (url) - 1] = '\0';
        strcpy (url, "start ");
        strcat (url, chart[int (st.wDayOfWeek) + 1][n]);
        freq = ct - lt;
        return true;
    }
}

LRESULT CALLBACK windpro (HWND h, UINT u, WPARAM w, LPARAM l)
{
    /* Main Window Procedure funciton */
    switch (u)
    {
        case WM_CREATE:
        {
            string line;
            ifstream mf ("feeder.txt");
            if (mf.is_open ())
            {
                getline (mf, line);
                int p;
                stringstream (line) >> p;
                x = p;
                int i = 0, j = -1;
                while (!mf.eof ())
                {
                    j++;
                    getline (mf, line);
                    strcpy (chart[i][j], line.c_str ());
                    if (j == x)
                    {
                        j = -1;
                        i++;
                    }
                }
                mf.close ();
            }
            else
            {
                MessageBox (NULL, "Couldn't Start Meeting Track!", "Error!", MB_ICONERROR);
                SendMessage (h, WM_CLOSE, 0, 0);
            }
            for (int i = 1; i <= x; i++)
                if (VerifyTime (i))
                {
                    CurrentTimeIndex = i;
                    SetTimer (h, TIMERID, freq, NULL);
                    return 0;
                }
            PostQuitMessage (0);
        }
        break;

        case WM_TIMER:
        {
            KillTimer (h, TIMERID);
            strcpy (mrl, url);
            ht = CreateThread (NULL, 0, launch, NULL, 0, NULL);
            MessageBox (NULL, "Your Meeting is Launching...", "Scheduled Meeting",
                       MB_SERVICE_NOTIFICATION | MB_ICONINFORMATION);
        }
        for (int i = CurrentTimeIndex + 1; i <= x; i++)
            if (VerifyTime (i))
            {
                CurrentTimeIndex = i;
                SetTimer (h, TIMERID, freq, NULL);
                return 0;
            }

        case WM_CLOSE:
            KillTimer (h, TIMERID);
            CloseHandle (ht);
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

APIENTRY WinMain (HINSTANCE a, HINSTANCE b, LPSTR c, int d)
{
    /* Main function of the program */
    WNDCLASSEX e;
    e.cbClsExtra = 0;
    e.cbSize = sizeof (WNDCLASSEX);
    e.cbWndExtra = 0;
    e.hbrBackground = (HBRUSH)CreateSolidBrush (RGB (255, 255, 255));
    e.hCursor = LoadCursor (NULL, IDC_ARROW);
    e.hIcon = LoadIcon (NULL, IDI_APPLICATION);
    e.hIconSm = LoadIcon (NULL, IDI_APPLICATION);
    e.hInstance = a;
    e.lpfnWndProc = windpro;
    e.lpszClassName = "MBRUNTIME";
    e.lpszMenuName = NULL;
    e.style = 0;
    RegisterClassEx (&e);
    CreateWindowEx (0, "MBRUNTIME", "", 0, 0, 0, 0, 0, NULL, NULL, a, NULL);
    MSG m;
    while (GetMessage (&m, NULL, 0, 0) > 0)
    {
        TranslateMessage (&m);
        DispatchMessage (&m);
    }
    return 0;
}
