#include <tchar.h>
#include <Windows.h>
#include "checker.hpp"

LRESULT CALLBACK mainWindowProc(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	static i16_t x, y, result;
	static bool is_press;
	static InputChecker checker;

	switch (message) {

	case WM_POINTERDOWN:
		if (checker.is_pen(w_param))
		{
			is_press = true;
			x = LOWORD(l_param);
			y = HIWORD(l_param);
			InvalidateRect(h_wnd, NULL, TRUE);
		}
		break;
	case WM_MOUSEMOVE:
		if (is_press)
		{
			x = LOWORD(l_param);
			y = HIWORD(l_param);
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
			SetPixel(hdc, x, y, RGB(20, 20, 20));
			EndPaint(h_wnd, &ps);
		}
	}
	break;
	case WM_CLOSE:

		DestroyWindow(h_wnd);

	case WM_DESTROY:

		PostQuitMessage(0);

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

	return hWnd;
}

extern int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPTSTR lpCmdLine, int nCmdShow)
{
	LPCTSTR class_name = TEXT("iropen");
	LPCTSTR window_title = TEXT("iropen");
	MSG message;

	if (!register_window_class(hInstance, class_name)) {
		return -1;
	}
	if (!create_window(hInstance, class_name, window_title, nCmdShow))
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