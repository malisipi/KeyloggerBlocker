#define UNICODE

#include <stdio.h>
#include <windows.h>
#include <winuser.h>
#include "keylogger_blocker_v_shared.h"

static void blocker_register_hotkey(){
    if (!RegisterHotKey(NULL, 1, 0x0008 | 0x4000, VK_ESCAPE)) {
        printf("[WARN]: HotKeys Crashed!");
        return;
    }

    MSG msg = {0};
    while (GetMessage(&msg, NULL, 0, 0) != 0){
        if (msg.message == WM_HOTKEY){
            blocker_toggle(FALSE);
        }
    }
}
