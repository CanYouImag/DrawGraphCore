// Rectangle.cpp
#include "Rectangle.h"
#include "LineDrawer.h"
#include <algorithm>

// 构造函数
Rectangle::Rectangle(const QVector2D& p1, const QVector2D& p2, const QColor& color)
	: m_color(color)
{
	// 确保p1是左下角，p2是右上角
	m_p1.setX(std::min(p1.x(), p2.x()));
	m_p1.setY(std::min(p1.y(), p2.y()));
	m_p2.setX(std::max(p1.x(), p2.x()));
	m_p2.setY(std::max(p1.y(), p2.y()));
}

// 获取矩形四条边的像素点
QVector<QVector2D> Rectangle::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> allPixels;
	int x1 = static_cast<int>(m_p1.x() + 0.5f);
	int y1 = static_cast<int>(m_p1.y() + 0.5f);
	int x2 = static_cast<int>(m_p2.x() + 0.5f);
	int y2 = static_cast<int>(m_p2.y() + 0.5f);

	allPixels += LineDrawer::drawLine(x1, y1, x2, y1); // 上边
	allPixels += LineDrawer::drawLine(x2, y1, x2, y2); // 右边
	allPixels += LineDrawer::drawLine(x2, y2, x1, y2); // 下边
	allPixels += LineDrawer::drawLine(x1, y2, x1, y1); // 左边

	return allPixels;
}