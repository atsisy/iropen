#include <tchar.h>
#include <Windows.h>
#include <deque>
#include <algorithm>
#include "checker.hpp"
#include "types.hpp"
#include "main_window.hpp"
#include "paint_data.hpp"

uptr<MainWindow> g_main_window;

LRESULT CALLBACK mainWindowProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	static POINT touch_point;
	static std::deque<uptr<BitPaintData>> paint_data_queue;
	static bool is_press;
	static InputChecker checker;

	switch (message) {

	case WM_POINTERDOWN:
		if (checker.is_pen(w_param))
		{
			int pressure;
			POINTER_PEN_INFO info;
			GetPointerPenInfo(GET_POINTERID_WPARAM(w_param), &info);
			pressure = info.pressure;

			is_press = true;
			touch_point.x = LOWORD(l_param);
			touch_point.y = HIWORD(l_param);
			paint_data_queue.push_back(uptr<BitPaintData>(new BitPaintData(touch_point, pressure)));
			InvalidateRect(h_wnd, NULL, TRUE);
		}
		break;
	case WM_POINTERUPDATE:
		if (is_press)
		{
			u16_t pressure;
			POINTER_PEN_INFO info;
			GetPointerPenInfo(GET_POINTERID_WPARAM(w_param), &info);
			pressure = info.pressure;

			touch_point.x = LOWORD(l_param);
			touch_point.y = HIWORD(l_param);
			if (ScreenToClient(h_wnd, &touch_point) == 0)
			{
				/*
				*エラー
				*/
			}

			paint_data_queue.push_back(uptr<BitPaintData>(new BitPaintData(touch_point, pressure)));
			InvalidateRect(h_wnd, NULL, TRUE);

		}
		break;
	case WM_POINTERUP:
		is_press = false;
		break;
	case WM_ERASEBKGND:
		return 1;
	case WM_CREATE:
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

		RegisterTouchWindow(h_wnd, 0);
	}

	break;
	
	case WM_PAINT:
	{
		if (is_press)
		{
			PAINTSTRUCT ps;
			HDC hdc;
			
			hdc = BeginPaint(h_wnd, &ps);
			
			while (!paint_data_queue.empty())
			{
				uptr<BitPaintData> bpd = std::move(*std::make_move_iterator(std::begin(paint_data_queue)));
				u8_t r = (bpd->pressure / 70) / 2;
				Ellipse(hdc, bpd->point.x - r, bpd->point.y - r, bpd->point.x + r, bpd->point.y + r);
				paint_data_queue.pop_front();
			}

			EndPaint(h_wnd, &ps);
		}
	}
	break;

	case WM_SIZE:
	case WM_MOVE:
		g_main_window->update_window_info(h_wnd);
		break;
	case WM_CLOSE:

		DestroyWindow(h_wnd);
		break;

	case WM_DESTROY:

		PostQuitMessage(0);
		break;
	}

	return DefWindowProc(h_wnd, message, w_param, l_param);

}

static ATOM register_window_class(HINSTANCE hInstance, LPCTSTR class_name)
{
	WNDCLASSEX window_class_ex = { 0 };

	window_class_ex.cbSize = sizeof(WNDCLASSEX);
	window_class_ex.style = (CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS);
	window_class_ex.lpfnWndProc = mainWindowProc;
	window_class_ex.cbClsExtra = 0;
	window_class_ex.cbWndExtra = 0;
	window_class_ex.hInstance = hInstance;
	window_class_ex.hIcon = nullptr;
	window_class_ex.hIconSm = nullptr;
	window_class_ex.hCursor = LoadCursor(hInstance, IDC_ARROW);
	window_class_ex.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	window_class_ex.lpszMenuName = NULL;
	window_class_ex.lpszClassName = class_name;
	
	return RegisterClassEx(&window_class_ex);
}

static HWND create_window(HINSTANCE hInstance, LPCTSTR class_name, LPCTSTR title, int nCmdShow)
{
	HWND hWnd = CreateWindowEx(
		0,                      //ウインドウスタイル(拡張)
		class_name,             //クラス名
		title,                  //タイトル
		WS_OVERLAPPEDWINDOW,    //ウインドウスタイル
		CW_USEDEFAULT,          //X座標
		CW_USEDEFAULT,          //Y座標
		CW_USEDEFAULT,          //幅
		CW_USEDEFAULT,          //高さ
		NULL,                   //親のハンドル
		NULL,                   //メニューのハンドル
		hInstance,              //インスタンスのハンドル
		NULL                    //ウインドウ作成データ
	);

	if (hWnd != nullptr) 
	{
		/*
		*CreateWindowに成功
		*/
		ShowWindow(hWnd, nCmdShow);
		UpdateWindow(hWnd);
	}

	g_main_window->update_window_info(hWnd);

	return hWnd;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	g_main_window.reset(new MainWindow(hInstance, TEXT("iropen"), TEXT("iropen")));
	MSG message;

	if (!register_window_class(g_main_window->get_hinstance(), g_main_window->get_class_name()))
	{
		return -1;
	}
	if (!create_window(hInstance, g_main_window->get_class_name(), g_main_window->get_title(), nCmdShow))
	{
		return -2;
	}

	while (GetMessage(&message, nullptr, 0, 0) > 0)
	{
		TranslateMessage(&message);
		DispatchMessage(&message);
	}

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);
	return message.wParam;
}