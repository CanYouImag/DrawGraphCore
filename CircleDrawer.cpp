// CircleDrawer.cpp
#include "CircleDrawer.h"
#include <QStack>

QVector<QVector2D> CircleDrawer::drawCircle(int cx, int cy, int r)
{
	QVector<QVector2D> points;
	int x = 0;
	int y = r;
	int d = 1 - r;

	auto plotCirclePoints = [&](int cx, int cy, int x, int y) {
		points.append(QVector2D(cx + x, cy + y));
		points.append(QVector2D(cx - x, cy + y));
		points.append(QVector2D(cx + x, cy - y));
		points.append(QVector2D(cx - x, cy - y));
		points.append(QVector2D(cx + y, cy + x));
		points.append(QVector2D(cx - y, cy + x));
		points.append(QVector2D(cx + y, cy - x));
		points.append(QVector2D(cx - y, cy - x));
		};

	plotCirclePoints(cx, cy, x, y);

	while (x < y) {
		x++;
		if (d < 0) {
			d += 2 * x + 1;
		}
		else {
			y--;
			d += 2 * (x - y) + 1;
		}
		plotCirclePoints(cx, cy, x, y);
	}

	return points;
}