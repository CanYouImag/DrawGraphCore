// LineDrawer.h
#pragma once
#include <QVector>
#include <QVector2D>

class LineDrawer
{
public:
	// ʹ�� Bresenham �㷨���ɴ� (x0,y0) �� (x1,y1) �����ص�
	static QVector<QVector2D> drawLine(int x0, int y0, int x1, int y1);

private:
	// ��������������б�� |k| <= 1 �����
	static void drawLineLow(int x0, int y0, int x1, int y1, QVector<QVector2D>& points);
	// ��������������б�� |k| > 1 �����
	static void drawLineHigh(int x0, int y0, int x1, int y1, QVector<QVector2D>& points);
};