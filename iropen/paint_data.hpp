#pragma once

#include <Windows.h>
#include <array>
#include "types.hpp"

struct BitPaintData {

	POINT point;
	u16_t pressure;

	BitPaintData(POINT point, u16_t pressure);
	BitPaintData();
};

constexpr u8_t RAW_PAINT_DATA_SET_SIZE = 4;
constexpr u8_t BEZIERED_PAINT_DATA_SET_SIZE = 10;
using RawPaintDataSet = std::array<BitPaintData, RAW_PAINT_DATA_SET_SIZE>;
using BezieredPaintDataSet = std::array<BitPaintData, BEZIERED_PAINT_DATA_SET_SIZE + 1>;
