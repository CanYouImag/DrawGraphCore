// LineDrawer.cpp
#include "LineDrawer.h"
#include <algorithm>

QVector<QVector2D> LineDrawer::drawLine(int x0, int y0, int x1, int y1)
{
	QVector<QVector2D> points;

	int dx = x1 - x0;
	int dy = y1 - y0;

	if (std::abs(dy) <= std::abs(dx)) {
		if (x0 > x1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		drawLineLow(x0, y0, x1, y1, points);
	}
	else {
		if (y0 > y1) {
			std::swap(x0, x1);
			std::swap(y0, y1);
		}
		drawLineHigh(x0, y0, x1, y1, points);
	}

	return points;
}

void LineDrawer::drawLineLow(int x0, int y0, int x1, int y1, QVector<QVector2D>& points)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int yi = dy > 0 ? 1 : -1;
	int D = 2 * std::abs(dy) - dx;
	int y = y0;

	for (int x = x0; x <= x1; ++x) {
		points.append(QVector2D(x, y));
		if (D > 0) {
			y += yi;
			D -= 2 * dx;
		}
		D += 2 * std::abs(dy);
	}
}

void LineDrawer::drawLineHigh(int x0, int y0, int x1, int y1, QVector<QVector2D>& points)
{
	int dx = x1 - x0;
	int dy = y1 - y0;
	int xi = dx > 0 ? 1 : -1;
	int D = 2 * std::abs(dx) - std::abs(dy);
	int x = x0;

	for (int y = y0; y <= y1; ++y) {
		points.append(QVector2D(x, y));
		if (D > 0) {
			x += xi;
			D -= 2 * std::abs(dy);
		}
		D += 2 * std::abs(dx);
	}
}