#include <tchar.h>
#include <Windows.h>
#include <deque>
#include <algorithm>
#include "types.hpp"
#include "main_window.hpp"
#include "paint_data.hpp"
#include "resource.h"
#include "message_handler.hpp"

uptr<MainWindow> g_main_window;

LRESULT CALLBACK mainWindowProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	static POINT touch_point;
	static std::deque<uptr<BitPaintData>> paint_data_queue;
	static bool is_press;
	static MessageHandler message_handler;

	switch (message) {

	case WM_GESTURE:
		return message_handler.gesture_handler(h_wnd, message, w_param, l_param);
	case WM_POINTERDOWN:
		message_handler.pointer_down_handler(
			w_param,
			l_param,
			h_wnd,
			&is_press,
			&touch_point,
			&paint_data_queue
		);
		break;

	case WM_POINTERUPDATE:
		message_handler.pointer_update_handler(
			h_wnd,
			w_param,
			l_param,
			is_press,
			&paint_data_queue
		);
		break;

	case WM_ERASEBKGND:
		return 1;
	case WM_CREATE:
		message_handler.create_handler(h_wnd);
		break;
	
	case WM_PAINT:
		message_handler.paint_handler(h_wnd, is_press, &paint_data_queue);
		break;

	case WM_POINTERLEAVE:
	case WM_POINTERUP:
		message_handler.pointer_leave_or_up_handler(w_param, &is_press);
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
	window_class_ex.lpszMenuName = TEXT("IROPEN_MENU");
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