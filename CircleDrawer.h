// CircleDrawer.h
#pragma once
#include <QVector>
#include <QVector2D>

class CircleDrawer
{
public:
	static QVector<QVector2D> drawCircle(int cx, int cy, int r);
};