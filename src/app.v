module main

import malisipi.mui
import malisipi.mui.window

// windres app_info.rc -O coff -o app_info.res
#flag "@VMODROOT/bin/temp/app_info.res"
#include "@VMODROOT/src/keylogger_blocker.h"
#include "@VMODROOT/src/keylogger_blocker_window.h"
#include "@VMODROOT/src/keylogger_blocker_hotkey.h"
#include "@VMODROOT/src/keylogger_blocker_tray.h"

fn C.blocker_enable()
fn C.blocker_disable()
fn C.blocker_is_enabled() bool
fn C.blocker_init_window()
fn C.blocker_loop_window()
fn C.blocker_init()
fn C.blocker_set_icon(window voidptr)
fn C.blocker_register_hotkey()
fn C.blocker_create_tray_menu()
fn C.blocker_tray_update()
fn C.blocker_tray_exit()

__global (
	gapp	&mui.Window
)

[export: "blocker_show_app_info"]
fn show_app_info(){
	mui.messagebox("About:", "KeyloggerBlocker v1.0\nhttps://github.com/malisipi/KeyloggerBlocker", "ok", "info")
}

fn blocker_gui_show_app_info (event_details mui.EventDetails, mut app &mui.Window, mut app_data voidptr){
	show_app_info()
}

[export: "blocker_toggle"]
fn blocker_toggle (is_gui bool){
	unsafe {
		if C.blocker_is_enabled() {
			C.blocker_disable()
			gapp.get_object_by_id("active")[0]["text"].str = "Enable"
			if !is_gui { go mui.notifypopup("Disabled", "You was disabled the Keylogger Blocker.", "warning") }
		} else {
			C.blocker_enable()
			gapp.get_object_by_id("active")[0]["text"].str = "Disable"
			if !is_gui { go mui.notifypopup("Enabled", "You was enabled the Keylogger Blocker.", "info") }
		}
		C.blocker_tray_update()
	}
}

[export: "blocker_quit_app"]
fn blocker_quit_app (){
	gapp.destroy()
}

fn blocker_gui_toggle (event_details mui.EventDetails, mut app &mui.Window, mut app_data voidptr){
	blocker_toggle(true)
}

fn main(){
	go fn (){
		C.blocker_create_tray_menu()
		if C.blocker_tray_menu_failed == 1{ // It's not critical process; so if it's fails, program will still work
			println("[WARN]: TrayMenu Thread Failed!")
		}
	}()
	go fn (){
		C.blocker_register_hotkey()
		println("[WARN]: HotKeys Thread Failed!")
		// It's not critical process; so if it's fails, program will still work
	}()
	C.blocker_init_window()
	go fn (){
		C.blocker_loop_window()
		println("[ERR ]: Keylogger Blocker_W Thread Failed!")
		exit(0)
	}()
	go fn (){
		C.blocker_init()
		println("[ERR ]: Core Thread Failed!")
		exit(0)
	}()

	gapp = mui.create(title:"Keylogger Blocker", width: 400, height: 300, draw_mode:.system_native, 
		init_fn: fn (event_details mui.EventDetails, mut app &mui.Window, mut app_data voidptr){
			window.set_opacity(app.window_handle(), 95)
			if !mui.user_light_theme {
				window.prefer_dark_titlebar(app.window_handle(), true)
			}
			C.blocker_set_icon(app.window_handle())
		},
		quit_fn: fn (event_details mui.EventDetails, mut app &mui.Window, mut app_data voidptr){
			C.blocker_tray_exit()
		}
	)

	gapp.image(id:"app_icon", x:"50%x -150", y:"50%y -100", width:"100", height:"100")
	$if !clang { // clang crashs with images @ windows. See the bug @ github: https://github.com/vlang/v/issues/17248
		gapp.get_object_by_id("app_icon")[0]["image"].img = gapp.gg.create_image_from_byte_array($embed_file("../assets/icon.png").to_bytes())!
	}
	gapp.label(id:"app_name", text_align: 0, text_size:48, text:"Keylogger\nBlocker", x:"50%x -30", y:"50%y -100", width:"180", height:"100", text_multiline:true)
	gapp.button(id:"active", text:"Disable", x:"50%x -120", y:"50%y 40", width:"240", height:"40", onclick:blocker_gui_toggle)
	gapp.button(id:"info", text:"ℹ️", icon:true, x:"# 10", y:"# 10", width:"40", height:"40", onclick:blocker_gui_show_app_info)

	gapp.run()
}
