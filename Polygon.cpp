// Polygon.cpp
#include "Polygon.h"
#include "LineDrawer.h"
#include <algorithm>

// 构造函数
Polygon::Polygon(const QVector<QVector2D>& vertices, const QColor& borderColor, const QColor& idColor)
	: m_vertices(vertices.size() >= 3 ? vertices : QVector<QVector2D>()),
	m_borderColor(borderColor), m_idColor(idColor)
{
}

// 获取多边形的像素点（边界或填充）
QVector<QVector2D> Polygon::getPixels(int windowWidth, int windowHeight) const
{
	if (m_vertices.isEmpty()) return {};

	// 如果启用了填充，则使用填充算法（含学号）
	if (m_fillMode != FillMode::None)
	{
		return Polygon::getFilledPixels(windowWidth, windowHeight);
	}

	// 否则只绘制边界
	return getBorderPixels();
}

// 获取边界像素点
QVector<QVector2D> Polygon::getBorderPixels() const
{
	QVector<QVector2D> borderPixels;
	int vertexCount = m_vertices.size();
	if (vertexCount < 3) return borderPixels;

	for (int i = 0; i < vertexCount; ++i)
	{
		QVector2D pCurrent = m_vertices[i];
		QVector2D pNext = m_vertices[(i + 1) % vertexCount];
		int x1 = static_cast<int>(pCurrent.x() + 0.5f);
		int y1 = static_cast<int>(pCurrent.y() + 0.5f);
		int x2 = static_cast<int>(pNext.x() + 0.5f);
		int y2 = static_cast<int>(pNext.y() + 0.5f);
		borderPixels += LineDrawer::drawLine(x1, y1, x2, y2);
	}

	return borderPixels;
}

// 获取填充像素点
QVector<QVector2D> Polygon::getFilledPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> filledPixels;
	int vertexCount = m_vertices.size();
	if (vertexCount < 3 || windowWidth <= 0 || windowHeight <= 0)
		return filledPixels;

	bool** edgeFlag = new bool* [windowHeight];
	for (int y = 0; y < windowHeight; ++y)
		edgeFlag[y] = new bool[windowWidth]();

	for (int i = 0; i < vertexCount; ++i)
	{
		QVector2D p1 = m_vertices[i];
		QVector2D p2 = m_vertices[(i + 1) % vertexCount];
		int x1 = static_cast<int>(p1.x() + 0.5f);
		int y1 = static_cast<int>(p1.y() + 0.5f);
		int x2 = static_cast<int>(p2.x() + 0.5f);
		int y2 = static_cast<int>(p2.y() + 0.5f);

		if (y1 == y2) continue;

		if (y1 > y2) {
			std::swap(x1, x2);
			std::swap(y1, y2);
		}

		float dx = x2 - x1;
		float dy = y2 - y1;
		float x = x1;
		float stepX = dx / dy;

		for (int y = y1; y < y2; ++y)
		{
			int xInt = static_cast<int>(x + 0.5f);
			if (xInt >= 0 && xInt < windowWidth)
				edgeFlag[y][xInt] = !edgeFlag[y][xInt];
			x += stepX;
		}
	}

	for (int y = 0; y < windowHeight; ++y)
	{
		bool isInside = false;
		for (int x = 0; x < windowWidth; ++x)
		{
			if (edgeFlag[y][x]) isInside = !isInside;
			if (isInside)
				filledPixels.append(QVector2D(x, y));
		}
	}

	addStudentId(filledPixels, windowWidth, windowHeight);

	for (int y = 0; y < windowHeight; ++y)
		delete[] edgeFlag[y];
	delete[] edgeFlag;

	return filledPixels;
}

// 添加学号填充
void Polygon::addStudentId(QVector<QVector2D>& filledPixels, int windowWidth, int windowHeight) const {
	// 实现学号填充逻辑
}

// 绘制数字
void Polygon::drawDigit(QVector<QVector2D>& filledPixels, const int digit[16][8], int startX, int startY) const {
	// 实现数字绘制逻辑
}

// 获取颜色（返回边界颜色）
QColor Polygon::color() const
{
	return m_borderColor;
}