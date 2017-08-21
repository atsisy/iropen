#pragma once

#include <deque>
#include "checker.hpp"
#include "gesture.hpp"
#include "paint_data.hpp"

class MessageHandler {

private:
	InputChecker checker;
	GestureDecoder gesture_decoder;

public:
	void paint_handler(HWND h_wnd, bool is_press, std::deque<uptr<BitPaintData>> *queue);
	void pointer_update_handler(HWND h_wnd, WPARAM w_param, LPARAM l_param, bool is_press, std::deque<uptr<BitPaintData>> *queue);
	void create_handler(HWND h_wnd);
	void pointer_down_handler(WPARAM w_param, LPARAM l_param, HWND h_wnd, bool *is_press, POINT *touch_point, std::deque<uptr<BitPaintData>> *queue);
	LRESULT gesture_handler(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param);
	void pointer_leave_or_up_handler(WPARAM w_param, bool *is_press);

};