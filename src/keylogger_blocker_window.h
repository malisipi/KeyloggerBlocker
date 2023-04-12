#define UNICODE

#include <stdio.h>
#include <windows.h>

static void blocker_set_icon(HWND hwnd){
#ifndef OLD_WINDOWS_MODE
    HICON hIcon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
#else
    HICON hIcon = LoadIcon(NULL, MAKEINTRESOURCE(32518));
#endif
    SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)hIcon);
	SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
}

static LRESULT CALLBACK blocker_window_proc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam){
    switch (uMsg){
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;
    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            EndPaint(hwnd, &ps);
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

static void blocker_init_window(){
    const wchar_t CLASS_NAME[] = L"Keylogger Blocker_W";
    WNDCLASS wc = {};
    wc.lpfnWndProc   = blocker_window_proc;
    wc.lpszClassName = CLASS_NAME;
    RegisterClass(&wc);

    HWND blocker_window_hwnd = CreateWindowEx(
        0,
        CLASS_NAME,
        L"Keylogger Blocker_W",
        WS_EX_TOOLWINDOW,
        -32000, -32000, 0, 0,
        NULL, NULL, NULL, NULL);

    SetWindowLongPtr(blocker_window_hwnd, GWLP_HWNDPARENT, (LONG_PTR)GetDesktopWindow());

    if (blocker_window_hwnd == NULL){
        printf("[ERR ]: Keylogger Blocker_W Crashed!\n");
        return;
    }

    ShowWindow(blocker_window_hwnd, TRUE);
}

static void blocker_loop_window(){
    MSG msg = {};
    while (GetMessage(&msg, NULL, 0, 0) > 0){
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }
}
