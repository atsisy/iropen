#pragma once

#include <Windows.h>
#include "types.hpp"

class MainWindow {

private:
	LPCTSTR class_name;
	LPCTSTR window_title;
	HINSTANCE h_instance;
	RECT window_rect;
	RECT client_rect;
	u16_t window_width;
	u16_t window_height;
	u16_t client_width;
	u16_t client_height;
	
public:
	MainWindow();
	MainWindow(HINSTANCE h_instance, LPCTSTR class_name, LPCTSTR window_title);
	LPCTSTR get_class_name();
	LPCTSTR get_title();
	HINSTANCE get_hinstance();
	void update_window_info(HWND h_wnd);
	i16_t get_window_x();
	i16_t get_window_y();
	u16_t get_window_width();
	u16_t get_window_height();
	u16_t get_client_width();
	u16_t get_client_height();
};
