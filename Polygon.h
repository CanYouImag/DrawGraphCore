#pragma once
#include "Shape.h"
#include <QVector2D>
#include <QColor>

// 填充模式枚举
enum class FillMode {
	None,        // 不填充
	WithId       // 用学号数字填充
};

// 多边形类：继承自Shape基类，支持学号数字填充功能
class Polygon : public Shape
{
public:
	Polygon() = default;

	// 构造函数
	// 参数: vertices - 顶点集合, borderColor - 边界颜色, idColor - 学号颜色
	Polygon(const QVector<QVector2D>& vertices, const QColor& borderColor, const QColor& idColor);

	// 获取顶点集合
	QVector<QVector2D> vertices() const { return m_vertices; }

	// 获取边界颜色
	QColor borderColor() const { return m_borderColor; }

	// 获取学号颜色
	QColor idColor() const { return m_idColor; }

	// 获取填充模式
	FillMode fillMode() const { return m_fillMode; }

	// 设置填充模式
	void setFillMode(FillMode mode) { m_fillMode = mode; }

	// 获取多边形的像素点（边界和学号数字）
	// 参数: width, height - 窗口尺寸
	// 返回: 多边形像素点集合
	QVector<QVector2D> getPixels(int width, int height) const override;

	// 获取颜色（返回边界颜色）
	QColor color() const override;

private:
	// 用学号数字填充多边形区域
	void fillWithStudentId(QVector<QVector2D>& pixels, int windowWidth, int windowHeight) const;

	// 判断点是否在多边形内部
	bool isPointInPolygon(const QVector2D& point) const;

	// 获取数字模板
	const int (*getDigitTemplate(int digit) const)[8];

	// 绘制数字
	void drawDigit(QVector<QVector2D>& pixels, const int digit[16][8],
		int startX, int startY) const;

	// 获取边界像素点
	QVector<QVector2D> getBorderPixels() const;

	QVector<QVector2D> m_vertices;    // 顶点集合
	QColor m_borderColor;             // 边界颜色
	QColor m_idColor;                 // 学号颜色
	FillMode m_fillMode = FillMode::None;  // 填充模式
};