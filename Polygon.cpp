#include "Polygon.h"
#include "LineDrawer.h"
#include <algorithm>
#include <QDebug>

// 数字模板定义
namespace DigitTemplates {
	// 数字 0
	const int digit0[16][8] = {
		{0,1,1,1,1,1,0,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,1,1,1,0},
		{1,1,0,1,1,1,1,0},
		{1,1,1,1,0,1,1,0},
		{1,1,1,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{0,1,1,1,1,1,0,0}
	};

	// 数字 1
	const int digit1[16][8] = {
		{0,0,1,1,0,0,0,0},
		{0,1,1,1,0,0,0,0},
		{1,1,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,0,1,1,0,0,0,0},
		{1,1,1,1,1,1,0,0}
	};

	// 数字 2
	const int digit2[16][8] = {
		{0,1,1,1,1,1,0,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,1,1,0,0},
		{0,0,0,1,1,0,0,0},
		{0,0,1,1,0,0,0,0},
		{0,1,1,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,0}
	};

	// 数字 3
	const int digit3[16][8] = {
		{0,1,1,1,1,1,0,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,1,1,0,0},
		{0,0,1,1,1,0,0,0},
		{0,0,0,0,1,1,0,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{0,0,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{1,1,0,0,0,1,1,0},
		{0,1,1,1,1,1,0,0}
	};
}

// 构造函数
Polygon::Polygon(const QVector<QVector2D>& vertices, const QColor& borderColor, const QColor& idColor)
	: m_vertices(vertices.size() >= 3 ? vertices : QVector<QVector2D>()),
	m_borderColor(borderColor), m_idColor(idColor),
	m_fillMode(FillMode::WithId)  // 默认使用学号填充
{
	// 简单验证顶点坐标
	for (int i = 0; i < m_vertices.size(); ++i) {
		if (m_vertices[i].x() < 0 || m_vertices[i].y() < 0) {
			qDebug() << "Warning: Polygon vertex has negative coordinates";
		}
	}
}

// 获取多边形的像素点（边界和学号数字）
QVector<QVector2D> Polygon::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	if (m_vertices.isEmpty()) return pixels;

	// 1. 绘制多边形边界
	pixels += getBorderPixels();

	// 2. 用学号数字填充多边形内部
	if (m_fillMode != FillMode::None)
	{
		fillWithStudentId(pixels, windowWidth, windowHeight);
	}

	return pixels;
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

// 用学号数字填充多边形区域
void Polygon::fillWithStudentId(QVector<QVector2D>& pixels, int windowWidth, int windowHeight) const
{
	if (m_vertices.size() < 3) return;

	// 计算多边形的包围盒
	float minX = m_vertices[0].x(), maxX = m_vertices[0].x();
	float minY = m_vertices[0].y(), maxY = m_vertices[0].y();

	for (const auto& vertex : m_vertices) {
		minX = std::min(minX, vertex.x());
		maxX = std::max(maxX, vertex.x());
		minY = std::min(minY, vertex.y());
		maxY = std::max(maxY, vertex.y());
	}

	// 数字的尺寸和间距
	const int digitWidth = 8;
	const int digitHeight = 16;
	const int horizontalSpacing = 12;
	const int verticalSpacing = 20;

	// 学号后四位 "1032"
	int studentId[] = { 1, 0, 3, 2 };
	int idLength = 4;

	// 计算学号整体的宽度
	int totalIdWidth = idLength * digitWidth + (idLength - 1) * horizontalSpacing;

	// 在多边形内部按网格填充学号
	for (int y = static_cast<int>(minY); y < maxY; y += verticalSpacing) {
		for (int x = static_cast<int>(minX); x < maxX; x += totalIdWidth + horizontalSpacing) {
			// 检查网格中心是否在多边形内
			QVector2D gridCenter(x + totalIdWidth / 2, y + digitHeight / 2);

			if (isPointInPolygon(gridCenter)) {
				// 在这个网格位置绘制学号
				for (int i = 0; i < idLength; i++) {
					drawDigit(pixels, getDigitTemplate(studentId[i]),
						x + i * (digitWidth + horizontalSpacing), y);
				}
			}
		}
	}
}

// 绘制数字
void Polygon::drawDigit(QVector<QVector2D>& pixels, const int digit[16][8],
	int startX, int startY) const
{
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			if (digit[y][x] == 1) {
				int pixelX = startX + x;
				// 反转Y坐标：将数学坐标转换为屏幕坐标
				int pixelY = startY + (15 - y);  // 16行数字，从0到15，所以反转是15-y

				// 检查像素是否在多边形内部
				if (isPointInPolygon(QVector2D(pixelX, pixelY))) {
					pixels.append(QVector2D(pixelX, pixelY));
				}
			}
		}
	}
}

// 判断点是否在多边形内部（使用射线法）
bool Polygon::isPointInPolygon(const QVector2D& point) const {
	if (m_vertices.size() < 3) return false;

	bool inside = false;
	int n = m_vertices.size();

	for (int i = 0, j = n - 1; i < n; j = i++) {
		const QVector2D& vi = m_vertices[i];
		const QVector2D& vj = m_vertices[j];

		// 检查点是否在多边形边界上
		if ((vi.y() == point.y() && vj.y() == point.y() &&
			((vi.x() <= point.x() && point.x() <= vj.x()) ||
				(vj.x() <= point.x() && point.x() <= vi.x())))) {
			return true;
		}

		if ((vi.y() > point.y()) != (vj.y() > point.y())) {
			float intersectX = (vj.x() - vi.x()) * (point.y() - vi.y()) / (vj.y() - vi.y()) + vi.x();

			if (point.x() <= intersectX) {
				inside = !inside;
			}
		}
	}

	return inside;
}

// 获取数字模板
const int (*Polygon::getDigitTemplate(int digit) const)[8] {
	using namespace DigitTemplates;

	switch (digit) {
	case 0: return digit0;
	case 1: return digit1;
	case 2: return digit2;
	case 3: return digit3;
	default: return digit0;
	}
}

// 获取颜色（返回边界颜色）
QColor Polygon::color() const
{
	return m_borderColor;
}