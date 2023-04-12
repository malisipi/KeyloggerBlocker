#define UNICODE

#include <windows.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

const int keys_int[] = {0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A}; //36

static bool blocker_enabled = TRUE;

static void blocker_keybd_event(BYTE bVk, DWORD dwFlags){
#ifndef OLD_WINDOWS_MODE
    INPUT input = {0};
    input.type = INPUT_KEYBOARD;
    input.ki.wVk = bVk;
    input.ki.wScan = 0;
    input.ki.dwFlags = dwFlags;
    input.ki.dwExtraInfo = 0;
    SendInput(1, &input, sizeof(INPUT));
#else
    keybd_event(bVk, 0, dwFlags, 0);
#endif
}

static void blocker_press_key(int key){
    blocker_keybd_event(key, 0);
    Sleep(2);
    blocker_keybd_event(key, KEYEVENTF_KEYUP);
}

static void blocker_send_fake_keys(){
    for (int i=0; i<rand()%4+3; ++i){
        blocker_press_key(keys_int[rand()%36]);
    }
}

static void blocker_enable(){
    blocker_enabled=TRUE;
}

static void blocker_disable(){
    blocker_enabled=FALSE;
}

static int blocker_is_enabled(){
    return blocker_enabled;
}

static bool blocker_keystate = FALSE;

static LRESULT CALLBACK blocker_keyboard_hook(int nCode, WPARAM wParam, LPARAM lParam) {
    if (nCode >= 0) {
        if (wParam == WM_KEYDOWN) {
            KBDLLHOOKSTRUCT *pKeyBoard = (KBDLLHOOKSTRUCT *)lParam;
            if(!(GetKeyState(VK_CONTROL) & 0x8000 ||
                GetKeyState(VK_LWIN) & 0x8000 ||
                GetKeyState(VK_RWIN) & 0x8000 ||
                GetKeyState(VK_MENU) & 0x8000 ||
                pKeyBoard->vkCode == VK_LWIN ||
                pKeyBoard->vkCode == VK_RWIN

            )){
                    blocker_keystate = TRUE;
            }
        }
    }
    return CallNextHookEx(NULL, nCode, wParam, lParam);
}

static DWORD WINAPI blocker_keyboard_thread(void* data) {
    HHOOK test1 = SetWindowsHookEx(WH_KEYBOARD_LL, blocker_keyboard_hook, GetModuleHandle(NULL), 0);
    MSG msg;
    while (GetMessage(&msg, NULL, 0, 0)) { 
        TranslateMessage(&msg); 
        DispatchMessage(&msg); 
    }
    return 0;
}

static int blocker_init(){
    srand(time(NULL));
    HWND active_window;
    HWND alt_window = FindWindow(NULL, L"Keylogger Blocker_W");
    wchar_t active_title[256];
    CreateThread(NULL, 0, blocker_keyboard_thread, NULL, 0, NULL);
    if(alt_window){
        while(TRUE){
            while(TRUE){
                if(!blocker_keystate){
                    Sleep(15);
                } else {
                    break;
                }
                if(!blocker_enabled){
                    while(TRUE){
                        Sleep(500);
                        if(blocker_enabled){
                            break;
                        }
                    }
                }
            }
            active_window=GetForegroundWindow();
            GetWindowText(active_window, active_title, sizeof(active_title));
            SetWindowText(alt_window, active_title);
            if(SetForegroundWindow(alt_window)){
                blocker_send_fake_keys();
            }
            SetForegroundWindow(active_window);
            blocker_keystate = FALSE;
        }
    } else {
        printf("[ERR ]: Core Crashed!\n");
    }
    return 0;
}
