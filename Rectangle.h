// Rectangle.h
#pragma once
#include "Shape.h"

// 矩形类：继承自Shape基类
class Rectangle : public Shape
{
public:
	Rectangle() = default;

	// 构造函数
	// 参数: p1, p2 - 对角点坐标, color - 颜色
	Rectangle(const QVector2D& p1, const QVector2D& p2, const QColor& color);

	// 获取第一个角点坐标
	QVector2D p1() const { return m_p1; }

	// 获取第二个角点坐标
	QVector2D p2() const { return m_p2; }

	// 获取颜色
	QColor color() const override { return m_color; }

	// 获取矩形四条边的像素点
	// 参数: windowWidth, windowHeight - 窗口尺寸
	// 返回: 矩形边界的像素点集合
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
	QVector2D m_p1;     // 第一个角点
	QVector2D m_p2;     // 第二个角点
	QColor m_color;     // 颜色
};