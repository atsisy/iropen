#include <Windows.h>
#include "message_handler.hpp"

BezieredPaintDataSet bezier(RawPaintDataSet data_set);

void MessageHandler::pointer_down_handler(WPARAM w_param, LPARAM l_param, HWND h_wnd, bool *is_press, std::deque<BitPaintData> *queue)
{
	if (checker.is_pen(w_param))
	{
		POINT point;
		POINTER_PEN_INFO info;
		GetPointerPenInfo(GET_POINTERID_WPARAM(w_param), &info);

		*is_press = true;
		point.x = LOWORD(l_param);
		point.y = HIWORD(l_param);
		queue->push_back(BitPaintData(point, info.pressure));

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

void MessageHandler::pointer_update_handler(HWND h_wnd, WPARAM w_param, LPARAM l_param, bool is_press, std::deque<BitPaintData> *queue)
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

		queue->push_back(BitPaintData(point, pressure));

		if (queue->size() >= 4)
		{
			InvalidateRect(h_wnd, NULL, TRUE);
		}
	}
}

void MessageHandler::paint_handler(HWND h_wnd, bool is_press, std::deque<BitPaintData> *queue)
{

	static POINT last = { 0, 0 };

	if (is_press)
	{
		PAINTSTRUCT ps;
		HDC hdc;
		u8_t i, size;

		hdc = BeginPaint(h_wnd, &ps);

		RawPaintDataSet data_set;

		for (i = 0; i < 4; ++i)
		{
			data_set[i] = queue->front();
			queue->pop_front();
		}

		BezieredPaintDataSet &&paint_data = bezier(data_set);

		for (i = 1; i <= BEZIERED_PAINT_DATA_SET_SIZE; ++i)
		{
			if (i == 1)
			{
				MoveToEx(hdc, last.x, last.y, nullptr);
				LineTo(hdc, paint_data.at(0).point.x, paint_data.at(0).point.y);
				MoveToEx(hdc, paint_data.at(0).point.x, paint_data.at(0).point.y, nullptr);
			}
			else
			{
				LineTo(hdc, paint_data.at(i).point.x, paint_data.at(i).point.y);
				MoveToEx(hdc, paint_data.at(i).point.x, paint_data.at(i).point.y, nullptr);
			}
		}

		last = paint_data.back().point;

		EndPaint(h_wnd, &ps);
	}
}