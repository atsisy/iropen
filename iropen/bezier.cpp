#include "paint_data.hpp"

static BitPaintData bezier_sub(RawPaintDataSet data_set, double t)
{
	BitPaintData result;

	double val0x = (1 - t) * (1 - t) * (1 - t) * data_set[0].point.x;
	double val0y = (1 - t) * (1 - t) * (1 - t) * data_set[0].point.y;
	double val1x = 3 * t * (1 - t) * (1 - t) * data_set[1].point.x;
	double val1y = 3 * t * (1 - t) * (1 - t) * data_set[1].point.y;
	double val2x = 3 * t * t * (1 - t) * data_set[2].point.x;
	double val2y = 3 * t * t * (1 - t) * data_set[2].point.y;
	double val3x = t * t * t * data_set[3].point.x;
	double val3y = t * t * t * data_set[3].point.y;

	POINT p;
	p.x = (val0x + val1x + val2x + val3x);
	p.y = (val0y + val1y + val2y + val3y);
	return BitPaintData(p, (data_set.front().pressure + data_set.back().pressure) >> 1);
}

BezieredPaintDataSet bezier(RawPaintDataSet data_set)
{
	u8_t i;
	double t, px0, py0, px1, py1;
	BezieredPaintDataSet bezier_set;
	BitPaintData p1, p2;
	constexpr double d_t = 1.0 / BEZIERED_PAINT_DATA_SET_SIZE;
	
	for (i = 0; i <= BEZIERED_PAINT_DATA_SET_SIZE; ++i)
	{
		t = (double)i / BEZIERED_PAINT_DATA_SET_SIZE;
		bezier_set[i] = bezier_sub(data_set, t);
	}

	return bezier_set;
}