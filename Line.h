// Line.h
#pragma once
#include "Shape.h"

// 直线类：继承自Shape基类
class Line : public Shape
{
public:
	Line() = default;

	// 构造函数
	// 参数: p1, p2 - 端点坐标, color - 颜色
	Line(const QVector2D& p1, const QVector2D& p2, const QColor& color);

	// 获取起点坐标
	QVector2D p1() const { return m_p1; }

	// 获取终点坐标
	QVector2D p2() const { return m_p2; }

	// 获取颜色
	QColor color() const override { return m_color; }

	// 获取线段上所有像素点
	// 参数: windowWidth, windowHeight - 窗口尺寸
	// 返回: 直线上的像素点集合
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
	QVector2D m_p1;     // 起点坐标
	QVector2D m_p2;     // 终点坐标
	QColor m_color;     // 颜色
};