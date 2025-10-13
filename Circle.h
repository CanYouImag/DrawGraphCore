// Circle.h
#pragma once
#include "Shape.h"

// 圆形类：继承自Shape基类
class Circle : public Shape
{
public:
	Circle() = default;

	// 构造函数
	// 参数: center - 圆心坐标, radius - 半径, color - 颜色
	Circle(const QVector2D& center, int radius, const QColor& color);

	// 获取圆心坐标
	QVector2D center() const { return m_center; }

	// 获取半径
	int radius() const { return m_radius; }

	// 获取颜色
	QColor color() const override { return m_color; }

	// 获取圆形边界上的所有像素点
	// 参数: windowWidth, windowHeight - 窗口尺寸
	// 返回: 圆形边界像素点集合
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
	QVector2D m_center;  // 圆心坐标
	int m_radius;        // 半径
	QColor m_color;      // 颜色
};