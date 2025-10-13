// Cube.h
#pragma once
#include "Shape.h"
#include <QVector3D>

// 立方体类：继承自Shape基类，实现3D立方体功能
class Cube : public Shape
{
public:
	Cube();

	// 构造函数
	// 参数: center - 中心坐标, size - 边长, color - 颜色
	Cube(const QVector3D& center, float size, const QColor& color);

	// 获取立方体投影后的像素点
	// 参数: windowWidth, windowHeight - 窗口尺寸
	// 返回: 立方体边界的像素点集合
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

	// 获取颜色
	QColor color() const override { return m_color; }

	// 平移变换
	// 参数: delta - 平移向量
	void translate(const QVector3D& delta);

	// 旋转变换
	// 参数: angles - 旋转角度向量
	void rotate(const QVector3D& angles);

	// 重置变换
	void reset();

	// 获取旋转角度
	QVector3D getRotateAngles() const { return m_rotateAngles; }

	// 获取平移向量
	QVector3D getTranslate() const { return m_translate; }

private:
	// 初始化立方体顶点
	// 参数: size - 立方体边长
	void initVertices(float size);

	// 3D顶点投影到2D窗口
	// 参数: vertex - 3D顶点坐标, windowWidth, windowHeight - 窗口尺寸
	// 返回: 投影后的2D坐标
	QVector2D projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const;

	// 应用模型变换
	QVector3D applyModelTransform(const QVector3D& vertex) const;

	// 绕X轴旋转
	QVector3D rotateX(const QVector3D& vertex, float angle) const;

	// 绕Y轴旋转  
	QVector3D rotateY(const QVector3D& vertex, float angle) const;

	// 绕Z轴旋转
	QVector3D rotateZ(const QVector3D& vertex, float angle) const;

	// 应用矩阵变换
	QVector3D applyMatrix(const QVector3D& vertex, const float matrix[4][4]) const;

	// 检查面是否可见
	bool isFaceVisible(const int face[4]) const;

	// 获取面的像素点
	QVector<QVector2D> getFacePixels(const int face[4], int windowWidth, int windowHeight) const;

	QVector3D m_center;           // 立方体中心坐标
	QVector3D m_rotateAngles;     // 旋转角度（X/Y/Z轴，度）
	QVector3D m_translate;        // 平移向量
	QVector<QVector3D> m_vertices; // 立方体8个顶点
	QColor m_color;               // 绘制颜色
	float m_size;                 // 立方体边长
};