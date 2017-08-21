#include <Windows.h>
#include "message_handler.hpp"

void MessageHandler::pointer_down_handler(WPARAM w_param, LPARAM l_param, HWND h_wnd, bool *is_press, POINT *touch_point, std::deque<uptr<BitPaintData>> *queue)
{
	if (checker.is_pen(w_param))
	{
		POINTER_PEN_INFO info;
		GetPointerPenInfo(GET_POINTERID_WPARAM(w_param), &info);

		*is_press = true;
		touch_point->x = LOWORD(l_param);
		touch_point->y = HIWORD(l_param);
		queue->push_back(uptr<BitPaintData>(new BitPaintData(*touch_point, info.pressure)));

		if (queue->size() >= 4)
		{
			InvalidateRect(h_wnd, NULL, TRUE);
		}
	}
}

LRESULT MessageHandler::gesture_handler(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	return gesture_decoder(h_wnd, message, w_param, l_param);
}

void MessageHandler::pointer_leave_or_up_handler(WPARAM w_param, bool *is_press)
{
	if (checker.is_pen(w_param))
	{
		is_press = false;
	}
}

void MessageHandler::create_handler(HWND h_wnd)
{
	int value = GetSystemMetrics(SM_DIGITIZER);
	if (value == 0)
	{
		MessageBoxW(h_wnd, L"タッチ機能に対応していません。", L"タッチ機能チェック", MB_OK);
		exit(0);
	}
	if (value & NID_READY)
	{

	}
	if (value & NID_MULTI_INPUT)
	{
		MessageBoxW(h_wnd, L"マルチタッチに対応しています。", L"タッチ機能チェック", MB_OK);
	}
	if (value & NID_INTEGRATED_TOUCH)
	{

	}
}

void MessageHandler::pointer_update_handler(HWND h_wnd, WPARAM w_param, LPARAM l_param, bool is_press, std::deque<uptr<BitPaintData>> *queue)
{
	if (is_press)
	{
		POINT point;
		u16_t pressure;
		POINTER_PEN_INFO info;
		GetPointerPenInfo(GET_POINTERID_WPARAM(w_param), &info);
		pressure = info.pressure;

		point.x = LOWORD(l_param);
		point.y = HIWORD(l_param);
		if (ScreenToClient(h_wnd, &point) == 0)
		{
			/*
			*エラー
			*/
		}

		queue->push_back(uptr<BitPaintData>(new BitPaintData(point, pressure)));
		InvalidateRect(h_wnd, NULL, TRUE);

	}
}

void MessageHandler::paint_handler(HWND h_wnd, bool is_press, std::deque<uptr<BitPaintData>> *queue)
{
	if (is_press)
	{
		PAINTSTRUCT ps;
		HDC hdc;

		hdc = BeginPaint(h_wnd, &ps);

		while (!queue->empty())
		{
			uptr<BitPaintData> bpd = std::move(*std::make_move_iterator(std::begin(*queue)));
			u8_t r = (bpd->pressure / 70) / 2;
			Ellipse(hdc, bpd->point.x - r, bpd->point.y - r, bpd->point.x + r, bpd->point.y + r);
			queue->pop_front();
		}

		EndPaint(h_wnd, &ps);
	}
}