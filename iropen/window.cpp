#include "main_window.hpp"

MainWindow::MainWindow()
{}

MainWindow::MainWindow(HINSTANCE h_instance, LPCTSTR class_name, LPCTSTR window_title)
{
	this->h_instance = h_instance;
	this->class_name = class_name;
	this->window_title = window_title;
}

LPCTSTR MainWindow::get_class_name()
{
	return class_name;
}

LPCTSTR MainWindow::get_title()
{
	return window_title;
}

HINSTANCE MainWindow::get_hinstance()
{
	return h_instance;
}

void MainWindow::update_window_info(HWND h_wnd)
{
	GetWindowRect(h_wnd, &window_rect);
	GetClientRect(h_wnd, &client_rect);
	
	/*
	*ウィンドウ幅、高さ計算
	*/
	window_width = window_rect.right - window_rect.left;
	window_height = window_rect.bottom - window_rect.top;

	/*
	*クライアント領域幅、高さ計算
	*/
	client_width = client_rect.right - client_rect.left;
	client_height = client_rect.bottom - client_rect.top;
}

i16_t MainWindow::get_window_x()
{
	return window_rect.left;
}

i16_t MainWindow::get_window_y()
{
	return window_rect.top;
}

u16_t MainWindow::get_window_width()
{
	return window_width;
}

u16_t MainWindow::get_window_height()
{
	return window_height;
}

u16_t MainWindow::get_client_width()
{
	return client_width;
}

u16_t MainWindow::get_client_height()
{
	return client_height;
}