#define UNICODE
#define _UNICODE
#include <windows.h>
#include <tchar.h>
#include <stdio.h>

#define UNUSED(var) ((void)var)

/*
    Notes on the WIN32 API:

    Components in the win32 (and win16) api, uses a class system split up into
    static information structs and instance structs.
    This example show how to create a new component (a new window).

    All types and variables in the Windows API are typically named by a prefix and 
    a "concept name" (a name that tells us what things are).
    Concept names starts with a uppercase letter and follows the uppercase camel casing rule.
    The prefix is all lowercase and describes datatypes (e.g. whether it is char wchar a pointer
    of 32bit size etc..).

    An important concept in the windows API is the idea of a "handle" 
    (typically descibed by the "h" prefix). Handlers are unique data structures that usually performs
    some function in the WinAPI. A lot of the Handlers served by the WinAPI is opique, that is
    the implementation details are hidden. This has the advantage that the user of the API does not
    need to worry about the implementation as long as they follow the rules outline in the documentation.
 */

// classname; maps the static info to the instance 
const TCHAR CLSNAME[] = TEXT("helloworldClass");

/**
 * Procedure to program all window components
 */
LRESULT CALLBACK winproc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp);

int APIENTRY WinMain(HINSTANCE hInst, HINSTANCE hInstPrev, PSTR cmdline, int cmdshow) {
    UNUSED(hInstPrev);
    UNUSED(cmdline);

    WNDCLASSEX wc = {0};

    wc.cbSize = sizeof(WNDCLASSEX); // cbSize = c (char) b (byte) size (well... Size)
    wc.lpfnWndProc = winproc; // handle to program instances of this. lpfnWndProc 
                              // lp (long pointer) fn (function ?) WndProc (window procedure)
    wc.hInstance = hInst; // Instance handler (h for handler). Machinery that creates instances.
                          // As this is the top level component we uses the instance handler parsed to us
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // What icon the windows should have
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // mouse cursor IDC_ARROW is the normal cursor
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszClassName = CLSNAME;
    wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

    if ( !RegisterClassEx(&wc) ) {
        MessageBox(NULL, TEXT("Could not register window class"), NULL, MB_ICONERROR);
        return 0;
    }

    HWND hwnd;
    hwnd = CreateWindowEx(
        WS_EX_LEFT,
        CLSNAME,
        NULL,
        WS_OVERLAPPEDWINDOW,
        // size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
        NULL, // parent
        NULL, // Menu
        hInst, // instance handler
        NULL // additional app data
    );

    if (!hwnd) {
        MessageBox(NULL, TEXT("Could not create window"), NULL, MB_ICONERROR);
        return 0;
    }

    ShowWindow(hwnd, cmdshow);

    MSG msg = {0};
    while ( GetMessage(&msg, NULL, 0, 0)  ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK winproc(HWND hwnd, UINT wmsg, WPARAM wp, LPARAM lp) {
    static LPTCH text = NULL;

    // wmsg contains window event enum

    switch (wmsg) {
        case WM_CREATE: // Creation event when a window is created
            if (!text) text = malloc(sizeof(TCHAR) * 128);
            return 0;
        case WM_CLOSE: // clicked on close button
            switch(MessageBox(hwnd, TEXT("Are you sure??"), TEXT("Confirmation"), MB_YESNO)) {
                case IDYES:
                    DestroyWindow(hwnd);
                    break;
                case IDNO:
                    break;
            }
            return 0;
        case WM_DESTROY: // window is being destroyed, we can clean up stuff here
            free(text);
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: { // On update 
                PAINTSTRUCT ps;
                HDC hdc = BeginPaint(hwnd, &ps);
                FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 1));

                EndPaint(hwnd, &ps);
            }
            return 0;
    }


    return DefWindowProc(hwnd, wmsg, wp, lp);
}