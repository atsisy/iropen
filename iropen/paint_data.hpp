#pragma once

#include <Windows.h>
#include "types.hpp"

struct BitPaintData {

	POINT point;
	u16_t pressure;

	BitPaintData(POINT point, u16_t pressure);

};