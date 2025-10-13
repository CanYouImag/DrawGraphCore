// Cube.cpp
#include "Cube.h"
#include "LineDrawer.h"
#include <cmath>

// 默认构造函数
Cube::Cube()
	: m_center(0, 0, -5), m_size(1.0f), m_color(Qt::white),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(m_size);
}

// 构造函数
Cube::Cube(const QVector3D& center, float size, const QColor& color)
	: m_center(center), m_size(size), m_color(color),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(size);
}

// 初始化立方体顶点
void Cube::initVertices(float size)
{
	// 本地坐标系顶点（中心在原点）
	float half = size / 2;
	m_vertices = {
		QVector3D(-half, -half, -half), // 0: 左前下
		QVector3D(half, -half, -half),  // 1: 右前下
		QVector3D(half, half, -half),   // 2: 右前上
		QVector3D(-half, half, -half),  // 3: 左前上
		QVector3D(-half, -half, half),  // 4: 左后下
		QVector3D(half, -half, half),   // 5: 右后下
		QVector3D(half, half, half),    // 6: 右后上
		QVector3D(-half, half, half)    // 7: 左后上
	};
}

// 3D顶点投影到2D窗口
QVector2D Cube::projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const
{
	// 应用模型变换
	QVector3D transformed = applyModelTransform(vertex);

	// 简化透视投影
	float distance = 5.0f; // 观察距离
	float perspective = distance / (distance + transformed.z());

	// 应用透视
	float x = transformed.x() * perspective;
	float y = transformed.y() * perspective;

	// 映射到窗口坐标
	float winX = (x + 1.0f) * 0.5f * windowWidth;
	float winY = (1.0f - y) * 0.5f * windowHeight;

	return QVector2D(winX, winY);
}

// 应用模型变换
QVector3D Cube::applyModelTransform(const QVector3D& vertex) const
{
	QVector3D result = vertex;

	// 应用旋转（绕X、Y、Z轴）
	result = rotateX(result, m_rotateAngles.x());
	result = rotateY(result, m_rotateAngles.y());
	result = rotateZ(result, m_rotateAngles.z());

	// 应用平移
	result += m_translate + m_center;

	return result;
}

// 绕X轴旋转
QVector3D Cube::rotateX(const QVector3D& vertex, float angle) const
{
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);

	float y = vertex.y() * cosA - vertex.z() * sinA;
	float z = vertex.y() * sinA + vertex.z() * cosA;

	return QVector3D(vertex.x(), y, z);
}

// 绕Y轴旋转
QVector3D Cube::rotateY(const QVector3D& vertex, float angle) const
{
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);

	float x = vertex.x() * cosA + vertex.z() * sinA;
	float z = -vertex.x() * sinA + vertex.z() * cosA;

	return QVector3D(x, vertex.y(), z);
}

// 绕Z轴旋转
QVector3D Cube::rotateZ(const QVector3D& vertex, float angle) const
{
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);

	float x = vertex.x() * cosA - vertex.y() * sinA;
	float y = vertex.x() * sinA + vertex.y() * cosA;

	return QVector3D(x, y, vertex.z());
}

// 应用矩阵变换
QVector3D Cube::applyMatrix(const QVector3D& vertex, const float matrix[4][4]) const
{
	float x = vertex.x() * matrix[0][0] + vertex.y() * matrix[1][0] + vertex.z() * matrix[2][0] + matrix[3][0];
	float y = vertex.x() * matrix[0][1] + vertex.y() * matrix[1][1] + vertex.z() * matrix[2][1] + matrix[3][1];
	float z = vertex.x() * matrix[0][2] + vertex.y() * matrix[1][2] + vertex.z() * matrix[2][2] + matrix[3][2];
	float w = vertex.x() * matrix[0][3] + vertex.y() * matrix[1][3] + vertex.z() * matrix[2][3] + matrix[3][3];

	if (w != 0.0f) {
		x /= w;
		y /= w;
		z /= w;
	}

	return QVector3D(x, y, z);
}

// 检查面是否可见
bool Cube::isFaceVisible(const int face[4]) const
{
	// 简化的可见性检查：基于面法向量和视线方向
	// 这里使用简单的背面剔除
	QVector3D v0 = m_vertices[face[0]];
	QVector3D v1 = m_vertices[face[1]];
	QVector3D v2 = m_vertices[face[2]];

	// 计算面法向量
	QVector3D edge1 = v1 - v0;
	QVector3D edge2 = v2 - v0;
	QVector3D normal = QVector3D::crossProduct(edge1, edge2);
	normal.normalize();

	// 视线方向（从物体指向相机）
	QVector3D viewDir(0, 0, 1); // 简化假设相机在Z轴正方向

	// 如果法向量与视线方向夹角大于90度，则面可见
	return QVector3D::dotProduct(normal, viewDir) < 0;
}

// 获取面的像素点
QVector<QVector2D> Cube::getFacePixels(const int face[4], int windowWidth, int windowHeight) const
{
	// 这里可以实现面填充，但根据作业要求，如果不会消隐可以只画边界线
	// 暂时返回空，只绘制边线
	return QVector<QVector2D>();
}

// 获取立方体投影后的像素点
QVector<QVector2D> Cube::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	// 立方体12条边的顶点索引
	const int edges[12][2] = {
		{0,1}, {1,2}, {2,3}, {3,0},  // 前面
		{4,5}, {5,6}, {6,7}, {7,4},  // 后面
		{0,4}, {1,5}, {2,6}, {3,7}   // 连接边
	};

	// 生成每条边的像素
	for (auto& edge : edges) {
		QVector2D p1 = projectTo2D(m_vertices[edge[0]], windowWidth, windowHeight);
		QVector2D p2 = projectTo2D(m_vertices[edge[1]], windowWidth, windowHeight);
		pixels += LineDrawer::drawLine(
			qRound(p1.x()), qRound(p1.y()),
			qRound(p2.x()), qRound(p2.y())
		);
	}

	return pixels;
}

// 平移变换
void Cube::translate(const QVector3D& delta)
{
	m_translate += delta;
}

// 旋转变换
void Cube::rotate(const QVector3D& angles)
{
	m_rotateAngles += angles;
}

// 重置变换
void Cube::reset()
{
	m_rotateAngles = QVector3D(0, 0, 0);
	m_translate = QVector3D(0, 0, 0);
}