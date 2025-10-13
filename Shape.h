// Shape.h
#pragma once
#include <QVector2D>
#include <QColor>
#include <QVector>

// 所有可绘制图元的抽象基类
class Shape
{
public:
	virtual ~Shape() = default;

	// 获取该图形的所有像素点（用于OpenGL绘制）
	// 参数: windowWidth, windowHeight - 窗口尺寸
	// 返回: 图形像素点集合
	virtual QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const = 0;

	// 获取图形颜色
	// 返回: 图形颜色
	virtual QColor color() const = 0;
};