// Circle.cpp
#include "Circle.h"
#include "CircleDrawer.h"

// 构造函数
Circle::Circle(const QVector2D& center, int radius, const QColor& color)
	: m_center(center), m_radius(radius), m_color(color)
{
	// 确保半径非负
	if (m_radius < 0)
	{
		m_radius = 0;
	}
}

// 获取圆形边界上的所有像素点
QVector<QVector2D> Circle::getPixels(int windowWidth, int windowHeight) const
{
	int cx = static_cast<int>(m_center.x() + 0.5f);
	int cy = static_cast<int>(m_center.y() + 0.5f);
	return CircleDrawer::drawCircle(cx, cy, m_radius);
}