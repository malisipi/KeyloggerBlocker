#define UNICODE

#include <stdio.h>
#include <string.h>

#include "keylogger_blocker_v_shared.h"
#include "thirdparty/tray.h"

static bool blocker_enabled; // from keylogger_blocker.h

static void blocker_tray_toggle_blocker(struct tray_menu *item);
static void blocker_tray_about(struct tray_menu *item);
static void blocker_tray_quit(struct tray_menu *item);
static int blocker_tray_menu_failed = 0;

static struct tray blocker_tray = {
    .icon = NULL,
    .tooltip = L"Keylogger Blocker",
    .class_name = L"KeyloggerBlockerTrayMenu",
    .menu =
        (struct tray_menu[]){
            {.text = L"Keylogger Blocker", .disabled = TRUE, .def = TRUE},
            {.text = L"-"},
            {.text = L"Enable", .checked = TRUE, .cb = blocker_tray_toggle_blocker},
            {.text = L"About", .cb = blocker_tray_about},
            {.text = L"-"},
            {.text = L"Quit", .cb = blocker_tray_quit},
            {.text = NULL}},
};

static void blocker_tray_toggle_blocker(struct tray_menu *item) {
  blocker_toggle(TRUE);
  tray_update(&blocker_tray);
}

static void blocker_tray_update(){
  blocker_tray.menu[2].checked = blocker_enabled;
  tray_update(&blocker_tray);
};

static void blocker_tray_about(struct tray_menu *item) {
  blocker_show_app_info();
}

static void blocker_tray_quit(struct tray_menu *item) {
  blocker_quit_app();
}

static void blocker_tray_exit(){
  tray_exit(&blocker_tray);
}

static void blocker_create_tray_menu() {
#ifndef OLD_WINDOWS_MODE
  blocker_tray.icon = LoadIcon(GetModuleHandle(NULL), MAKEINTRESOURCE(101));
#else
  blocker_tray.icon = LoadIcon(NULL, MAKEINTRESOURCE(32518));
#endif
  if (tray_init(&blocker_tray) < 0) {
    printf("[ERR ]: TrayMenu Crashed!\n");
    blocker_tray_menu_failed = 1;
    return;
  }
  while (tray_loop(1) == 0);
}
