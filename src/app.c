/* It's legacy launcher for KeyloggerBlocker.
 * It can work under Windows 2000.
 */
#define UNICODE

#include <windows.h>

#include "keylogger_blocker.h"
#include "keylogger_blocker_window.h"
#include "keylogger_blocker_hotkey.h"
#include "keylogger_blocker_tray.h"

#define ID_APPNAME_LABEL 101
#define ID_ACTIVATE_BUTTON 102
#define ID_ABOUT_BUTTON 103

HWND APP_WINDOW;
HWND ACTIVE_BUTTON;

VV_EXPORTED_SYMBOL void blocker_show_app_info(){
	MessageBox(APP_WINDOW, L"KeyloggerBlocker v1.0\nhttps://github.com/malisipi/KeyloggerBlocker", L"About:", MB_ICONINFORMATION | MB_OK);
}

VV_EXPORTED_SYMBOL void blocker_toggle(BOOL is_gui){
	if(blocker_is_enabled()){
		blocker_disable();
		SetWindowText(ACTIVE_BUTTON, L"Enable");
	} else {
		blocker_enable();
		SetWindowText(ACTIVE_BUTTON, L"Disable");
	}
	blocker_tray_update();
}

VV_EXPORTED_SYMBOL void blocker_quit_app(){
	SendMessage(APP_WINDOW, WM_CLOSE, 0, 0);
}

DWORD WINAPI tray_menu_thread(void* data) {
	blocker_create_tray_menu();
	return 0;
}

DWORD WINAPI register_hotkey_thread(void* data) {
	blocker_register_hotkey();
	return 0;
}

DWORD WINAPI loop_window_thread(void* data) {
	blocker_loop_window();
	exit(0);
}

DWORD WINAPI blocker_init_thread(void* data) {
	blocker_init();
	exit(0);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam);

#ifndef OLD_WINDOWS_MODE
	int wWinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nShowCmd){
#else
	int WINAPI WinMain (HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nShowCmd){
#endif
	CreateThread(NULL, 0, tray_menu_thread, NULL, 0, NULL);
	CreateThread(NULL, 0, register_hotkey_thread, NULL, 0, NULL);
	blocker_init_window();
	CreateThread(NULL, 0, loop_window_thread, NULL, 0, NULL);
	CreateThread(NULL, 0, blocker_init_thread, NULL, 0, NULL);

	WNDCLASSEX wc;
	wc.cbSize = sizeof(WNDCLASSEX);
	wc.style = 0;
	wc.lpfnWndProc = WndProc;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = hInstance;
	wc.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.lpszMenuName = NULL;
	wc.lpszClassName = L"KeyloggerBlocker";
	wc.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	if (!RegisterClassEx(&wc)){
		return 0;
	}

	APP_WINDOW = CreateWindow(
		L"KeyloggerBlocker",
		L"KeyloggerBlocker",
		WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, CW_USEDEFAULT, 300, 175,
		NULL, NULL, hInstance, NULL);

	if (APP_WINDOW == NULL){
		return 0;
	}

	ShowWindow(APP_WINDOW, nShowCmd);
	UpdateWindow(APP_WINDOW);

	blocker_set_icon(APP_WINDOW);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0){
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}
	return msg.wParam;
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT msg, WPARAM wParam, LPARAM lParam){
	switch (msg){
		case WM_CREATE:
			CreateWindow(L"STATIC", L"KeyloggerBlocker", WS_VISIBLE | WS_CHILD | SS_CENTER, 20, 20, 260, 25, hwnd, (HMENU)ID_APPNAME_LABEL, NULL, NULL);
			ACTIVE_BUTTON = CreateWindow(L"BUTTON", L"Disable", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 75, 65, 150, 35, hwnd, (HMENU)ID_ACTIVATE_BUTTON, NULL, NULL);
			CreateWindow(L"BUTTON", L"About", WS_VISIBLE | WS_CHILD | BS_DEFPUSHBUTTON, 205, 115, 80, 25, hwnd, (HMENU)ID_ABOUT_BUTTON, NULL, NULL);
			break;
		case WM_COMMAND:
			switch (LOWORD(wParam)){
				case ID_ACTIVATE_BUTTON:
				blocker_toggle(TRUE);
				break;
				case ID_ABOUT_BUTTON:
				blocker_show_app_info();
				break;
			}
			break;
		case WM_CLOSE:
			blocker_tray_exit();
			DestroyWindow(hwnd);
			break;
		case WM_DESTROY:
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hwnd, msg, wParam, lParam);
	}
	return 0;
}
