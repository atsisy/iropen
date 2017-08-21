#include "gesture.hpp"

LRESULT GestureDecoder::operator()(HWND h_wnd, UINT message, WPARAM w_param, LPARAM l_param)
{
	GESTUREINFO gesture_info;
	ZeroMemory(&gesture_info, sizeof(GESTUREINFO));
	gesture_info.cbSize = sizeof(GESTUREINFO);

	bool success = GetGestureInfo((HGESTUREINFO)l_param, &gesture_info), catched = false;

	if (success) 
	{
		switch (gesture_info.dwID) 
		{
		case GID_ZOOM:
			/*
			*ズームのジェスチャーを認識
			*/
			catched = true;

			zoom_handler();
			break;
		case GID_ROTATE:
			/*
			*回転のジェスチャーを認識
			*/
			catched = true;
			break;
		case GID_PAN:
			/*
			*
			*/
			catched = true;
			break;
		case GID_TWOFINGERTAP:
			/*
			*二本指タップを認識
			*/
			catched = true;
			break;
		case GID_PRESSANDTAP:
			/*
			*
			*/
			catched = true;
			break;
		default:
			break;
		}
	}
	
	if (catched) 
	{
		return 0;
	}
	else {
		return DefWindowProc(h_wnd, message, w_param, l_param);
	}
}

void GestureDecoder::zoom_handler()
{}