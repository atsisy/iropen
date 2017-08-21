#pragma once

#include <Windows.h>

class GestureDecoder {

private:
	void zoom_handler();

public:
	LRESULT operator()(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);

};