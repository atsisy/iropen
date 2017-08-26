#include "paint_data.hpp"

BitPaintData::BitPaintData(POINT point, u16_t pressure)
{
	this->point = point;
	this->pressure = pressure;
}

BitPaintData::BitPaintData()
{}