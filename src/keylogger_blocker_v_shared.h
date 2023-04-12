#define UNICODE

#define VV_EXPORTED_SYMBOL extern __declspec(dllexport)

VV_EXPORTED_SYMBOL void blocker_toggle(bool);
VV_EXPORTED_SYMBOL void blocker_quit_app();
VV_EXPORTED_SYMBOL void blocker_show_app_info();
