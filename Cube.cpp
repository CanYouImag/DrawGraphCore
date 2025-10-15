#include "Cube.h"
#include "LineDrawer.h"
#include <cmath>

Cube::Cube()
	: m_center(0, 0, 0), m_size(2.0f), m_color(Qt::black),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices();
}

Cube::Cube(const QVector3D& center, float size, const QColor& color)
	: m_center(center), m_size(size), m_color(color),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices();
}

void Cube::initVertices()
{
	float s = m_size / 2.0f;

	// 定义立方体的8个顶点
	m_vertices = {
		QVector3D(-s, -s, -s), // 0: 左前下
		QVector3D(s, -s, -s), // 1: 右前下  
		QVector3D(s,  s, -s), // 2: 右前上
		QVector3D(-s,  s, -s), // 3: 左前上
		QVector3D(-s, -s,  s), // 4: 左后下
		QVector3D(s, -s,  s), // 5: 右后下
		QVector3D(s,  s,  s), // 6: 右后上
		QVector3D(-s,  s,  s)  // 7: 左后上
	};
}

QVector2D Cube::projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const
{
	// 应用变换
	QVector3D transformed = applyTransform(vertex);

	// 简单的透视投影
	float distance = 3.0f;
	float scale = distance / (distance + transformed.z());

	// 应用透视
	float x = transformed.x() * scale;
	float y = transformed.y() * scale;

	// 映射到屏幕坐标
	float screenX = (x + 1.0f) * 0.5f * windowWidth;
	float screenY = (1.0f - (y + 1.0f) * 0.5f) * windowHeight;

	return QVector2D(screenX, screenY);
}

QVector3D Cube::applyTransform(const QVector3D& vertex) const
{
	QVector3D result = vertex;

	// 应用旋转
	float rx = m_rotateAngles.x() * M_PI / 180.0f;
	float ry = m_rotateAngles.y() * M_PI / 180.0f;
	float rz = m_rotateAngles.z() * M_PI / 180.0f;

	// 绕X轴旋转
	if (rx != 0) {
		float cosX = cos(rx);
		float sinX = sin(rx);
		float y = result.y() * cosX - result.z() * sinX;
		float z = result.y() * sinX + result.z() * cosX;
		result.setY(y);
		result.setZ(z);
	}

	// 绕Y轴旋转
	if (ry != 0) {
		float cosY = cos(ry);
		float sinY = sin(ry);
		float x = result.x() * cosY + result.z() * sinY;
		float z = -result.x() * sinY + result.z() * cosY;
		result.setX(x);
		result.setZ(z);
	}

	// 绕Z轴旋转
	if (rz != 0) {
		float cosZ = cos(rz);
		float sinZ = sin(rz);
		float x = result.x() * cosZ - result.y() * sinZ;
		float y = result.x() * sinZ + result.y() * cosZ;
		result.setX(x);
		result.setY(y);
	}

	// 应用平移
	result += m_translate + m_center;

	return result;
}

QVector<QVector2D> Cube::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	// 只绘制边线，不填充面
	const int edges[12][2] = {
		{0,1}, {1,2}, {2,3}, {3,0},  // 前面
		{4,5}, {5,6}, {6,7}, {7,4},  // 后面
		{0,4}, {1,5}, {2,6}, {3,7}   // 连接边
	};

	// 生成每条边的像素
	for (int i = 0; i < 12; i++) {
		int v1 = edges[i][0];
		int v2 = edges[i][1];

		QVector2D p1 = projectTo2D(m_vertices[v1], windowWidth, windowHeight);
		QVector2D p2 = projectTo2D(m_vertices[v2], windowWidth, windowHeight);

		// 绘制直线
		int x1 = static_cast<int>(p1.x());
		int y1 = static_cast<int>(p1.y());
		int x2 = static_cast<int>(p2.x());
		int y2 = static_cast<int>(p2.y());

		auto linePixels = LineDrawer::drawLine(x1, y1, x2, y2);
		pixels += linePixels;
	}

	return pixels;
}

void Cube::translate(const QVector3D& delta)
{
	m_translate += delta;
}

void Cube::rotate(const QVector3D& angles)
{
	m_rotateAngles += angles;
}

void Cube::reset()
{
	m_rotateAngles = QVector3D(0, 0, 0);
	m_translate = QVector3D(0, 0, 0);
}