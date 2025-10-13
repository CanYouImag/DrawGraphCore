// Polygon.h
#pragma once
#include "Shape.h"

// 填充模式枚举
enum class FillMode {
	None,        // 不填充
	EdgeFlag,    // 边标志算法填充
	FloodFill,   // 泛洪填充
	ScanLine     // 扫描线填充
};

// 多边形类：继承自Shape基类，支持填充功能
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

	// 获取多边形的像素点（边界或填充）
	// 参数: width, height - 窗口尺寸
	// 返回: 多边形像素点集合
	QVector<QVector2D> getPixels(int width, int height) const override;

	// 获取颜色（返回边界颜色）
	QColor color() const override;

private:
	// 添加学号填充
	// 参数: filledPixels - 填充像素集合, windowWidth, windowHeight - 窗口尺寸
	void addStudentId(QVector<QVector2D>& filledPixels, int windowWidth, int windowHeight) const;

	// 绘制数字
	// 参数: filledPixels - 填充像素集合, digit - 数字模板, startX, startY - 起始位置
	void drawDigit(QVector<QVector2D>& filledPixels, const int digit[16][8],
		int startX, int startY) const;

	// 获取边界像素点
	QVector<QVector2D> getBorderPixels() const;

	// 获取填充像素点
	QVector<QVector2D> getFilledPixels(int windowWidth, int windowHeight) const;

	// 标记边
	void markEdge(QVector<QVector<bool>>& edgeFlags, const QVector2D& p1,
		const QVector2D& p2, int width, int height) const;

	QVector<QVector2D> m_vertices;    // 顶点集合
	QColor m_borderColor;             // 边界颜色
	QColor m_idColor;                 // 学号颜色
	FillMode m_fillMode = FillMode::None;  // 填充模式
};