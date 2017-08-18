#pragma once

#include <Windows.h>
#include "int_types.hpp"

class InputChecker {

private:

	POINTER_INPUT_TYPE type;

public:

	InputChecker();

	bool is_pen(const WPARAM w_param)
	{
		GetPointerType(GET_POINTERID_WPARAM(w_param), &type);
		return type == PT_PEN;
	}
	bool is_finger(const WPARAM w_param)
	{
		GetPointerType(GET_POINTERID_WPARAM(w_param), &type);
		return type == PT_TOUCH;
	}
	bool is_mouse(const WPARAM w_param)
	{
		GetPointerType(GET_POINTERID_WPARAM(w_param), &type);
		return type == PT_MOUSE;
	}
	bool is_touchpad(const WPARAM w_param)
	{
		GetPointerType(GET_POINTERID_WPARAM(w_param), &type);
		return type == PT_TOUCHPAD;
	}
};