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

struct StateInfo { // Window state
    int anwser;
    LPTCH text;
};

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

    struct StateInfo a = {0};

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
        (void *) (&a)
        //NULL // additional app data
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

void onPaint(HWND hwnd);
void onCreate(HWND hwnd, CREATESTRUCT *createStruct);
void onDestroy(HWND hwnd);
void onSize(HWND hwnd, UINT flag, int width, int height);
void onRender(HWND hwnd);

LRESULT CALLBACK winproc(HWND hwnd, UINT wmsg, WPARAM wp, LPARAM lp) {

    // wmsg contains window event enum
    switch (wmsg) {
        case WM_CREATE: // Creation event when a window is created
            {
                CREATESTRUCT *createStruct = (CREATESTRUCT *) lp;
                onCreate(hwnd, createStruct);
            }
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
            onDestroy(hwnd);
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: // On update
            onPaint(hwnd);
            return 0;
        case WM_SIZE: // On resize
            {
                int w = LOWORD(lp);
                int h = HIWORD(lp);

                onSize(hwnd, (UINT) wp, w, h);
            }
            return 0;
    }
    // all messages needs to be handled so this function handles it
    return DefWindowProc(hwnd, wmsg, wp, lp);
}

/**
 * Resize dispatch
 */
void onSize(HWND hwnd, UINT flag, int width, int height) {
    UNUSED(hwnd);
    UNUSED(flag);
    printf("resizing (w: %i,h: %i)\n", width, height);
}

void onPaint(HWND hwnd) {
    PAINTSTRUCT ps;
    HDC hdc = BeginPaint(hwnd, &ps);
    FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW + 2));

    struct StateInfo *state = (struct StateInfo *) GetWindowLongPtr(hwnd, GWLP_USERDATA);

    wprintf(L"PAINTING %s\n", state->text);

    EndPaint(hwnd, &ps);
}

void onDestroy(HWND hwnd) {
    struct StateInfo *state = (struct StateInfo *) GetWindowLongPtr(hwnd, GWLP_USERDATA);
    free(state->text);
}

void onCreate(HWND hwnd, CREATESTRUCT *createStruct) {
    UNUSED(hwnd);
    struct StateInfo *c = createStruct->lpCreateParams;

    static size_t bufsiz = sizeof(TCHAR) * 128;
    c->anwser = 32;
    c->text = calloc(bufsiz + 1, sizeof(TCHAR));

    swprintf_s(c->text, bufsiz + 1, L"Hello there\n");

    SetWindowLongPtr(hwnd, GWLP_USERDATA, (LONG_PTR) c);
        // setting my data so that we can get it back any time via "GetWindowLongPtr"

}