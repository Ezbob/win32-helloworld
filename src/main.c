#define UNICODE
#define _UNICODE
#include <windows.h>
#include <tchar.h>

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

int g_RUNNING = 1;

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
    wc.cbClsExtra = 0;
        // extra bytes for the static class, once again c (char) b (byte)
    wc.cbWndExtra = 0;
        // extra bytes for any instance of the class, once again c (char) b (byte)
    wc.hInstance = hInst; // Instance handler (h for handler). Machinery that creates instances.
                          // As this is the top level component we uses the instance handler parsed to us
    wc.hIcon = LoadIcon(NULL, IDI_APPLICATION); // What icon the windows should have
    wc.hCursor = LoadCursor(NULL, IDC_ARROW); // mouse cursor IDC_ARROW is the normal cursor
    wc.hbrBackground = (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName = NULL;
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
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        CW_USEDEFAULT,
        NULL,
        NULL,
        hInst,
        NULL
    );

    if (!hwnd) {
        MessageBox(NULL, TEXT("Could not create window"), NULL, MB_ICONERROR);
        return 0;
    }

    MSG msg;
    ShowWindow(hwnd, cmdshow);
    UpdateWindow(hwnd);
    while ( g_RUNNING && GetMessage(&msg, NULL, 0, 0)  ) {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
    return msg.wParam;
}

LRESULT CALLBACK winproc(HWND hwnd, UINT wm, WPARAM wp, LPARAM lp) {

    return DefWindowProc(hwnd, wm, wp, lp);
}