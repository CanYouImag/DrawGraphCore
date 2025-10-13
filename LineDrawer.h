// LineDrawer.h
#pragma once
#include <QVector>
#include <QVector2D>

class LineDrawer
{
public:
	// 使用 Bresenham 算法生成从 (x0,y0) 到 (x1,y1) 的像素点
	static QVector<QVector2D> drawLine(int x0, int y0, int x1, int y1);

private:
	// 辅助函数：处理斜率 |k| <= 1 的情况
	static void drawLineLow(int x0, int y0, int x1, int y1, QVector<QVector2D>& points);
	// 辅助函数：处理斜率 |k| > 1 的情况
	static void drawLineHigh(int x0, int y0, int x1, int y1, QVector<QVector2D>& points);
};