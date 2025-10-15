// Cube.cpp
#include "Cube.h"
#include "LineDrawer.h"
#include <cmath>
#include <algorithm>
#include <QVector>

// 默认构造函数
Cube::Cube()
	: m_center(0, 0, -5), m_size(1.0f), m_color(Qt::white),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(m_size);
	initFaces();
	initFaceColors();
}

// 构造函数
Cube::Cube(const QVector3D& center, float size, const QColor& color)
	: m_center(center), m_size(size), m_color(color),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(size);
	initFaces();
	initFaceColors();
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

// 初始化面数据
void Cube::initFaces()
{
	// 定义6个面，每个面4个顶点（顺时针方向，从外部看）
	m_faces = {
		{0, 1, 2, 3},  // 前面
		{5, 4, 7, 6},  // 后面
		{4, 0, 3, 7},  // 左面
		{1, 5, 6, 2},  // 右面
		{3, 2, 6, 7},  // 上面
		{4, 5, 1, 0}   // 下面
	};
}

// 初始化面颜色
void Cube::initFaceColors()
{
	m_faceColors = {
		QColor(255, 0, 0),      // 前面 - 红色
		QColor(0, 255, 0),      // 后面 - 绿色
		QColor(0, 0, 255),      // 左面 - 蓝色
		QColor(255, 255, 0),    // 右面 - 黄色
		QColor(255, 0, 255),    // 上面 - 洋红
		QColor(0, 255, 255)     // 下面 - 青色
	};
}

// 3D顶点投影到2D窗口
QVector2D Cube::projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const
{
	// 应用模型变换
	QVector3D transformed = applyModelTransform(vertex);

	// 透视投影
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

// 检查面是否可见
bool Cube::isFaceVisible(int faceIndex) const
{
	if (faceIndex < 0 || faceIndex >= m_faces.size()) return false;

	const auto& face = m_faces[faceIndex];

	// 获取面的三个顶点（用于计算法向量）
	QVector3D v0 = applyModelTransform(m_vertices[face[0]]);
	QVector3D v1 = applyModelTransform(m_vertices[face[1]]);
	QVector3D v2 = applyModelTransform(m_vertices[face[2]]);

	// 计算面法向量
	QVector3D edge1 = v1 - v0;
	QVector3D edge2 = v2 - v0;
	QVector3D normal = QVector3D::crossProduct(edge1, edge2);

	// 如果法向量长度为0，说明面退化
	if (normal.length() < 0.0001f) return false;

	normal.normalize();

	// 视线方向（从物体指向相机）
	QVector3D viewDir(0, 0, 1);

	// 如果法向量与视线方向夹角小于90度，则面可见
	return QVector3D::dotProduct(normal, viewDir) < 0;
}

// 获取立方体投影后的像素点
QVector<QVector2D> Cube::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	// 1. 先绘制可见面的填充
	for (int i = 0; i < m_faces.size(); ++i) {
		if (isFaceVisible(i)) {
			// 获取面的2D投影顶点
			QVector<QVector2D> vertices2D;
			const auto& face = m_faces[i];
			for (int vertexIndex : face) {
				vertices2D.append(projectTo2D(m_vertices[vertexIndex], windowWidth, windowHeight));
			}

			// 简单填充：绘制面的边界线（临时方案，避免性能问题）
			for (int j = 0; j < 4; j++) {
				int startIdx = j;
				int endIdx = (j + 1) % 4;
				QVector2D p1 = vertices2D[startIdx];
				QVector2D p2 = vertices2D[endIdx];

				// 使用面的颜色绘制边界
				auto linePixels = LineDrawer::drawLine(
					qRound(p1.x()), qRound(p1.y()),
					qRound(p2.x()), qRound(p2.y())
				);

				// 添加边界像素
				for (const auto& pixel : linePixels) {
					pixels.append(pixel);
				}
			}

			// 在面中心绘制一个点表示填充颜色
			QVector2D center(0, 0);
			for (const auto& vertex : vertices2D) {
				center += vertex;
			}
			center /= vertices2D.size();

			// 在面中心绘制3x3的点阵表示填充
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					pixels.append(QVector2D(center.x() + dx, center.y() + dy));
				}
			}
		}
	}

	// 2. 绘制所有边界线（黑色）
	const int edges[12][2] = {
		{0,1}, {1,2}, {2,3}, {3,0},  // 前面
		{4,5}, {5,6}, {6,7}, {7,4},  // 后面
		{0,4}, {1,5}, {2,6}, {3,7}   // 连接边
	};

	// 生成每条边的像素
	for (auto& edge : edges) {
		QVector2D p1 = projectTo2D(m_vertices[edge[0]], windowWidth, windowHeight);
		QVector2D p2 = projectTo2D(m_vertices[edge[1]], windowWidth, windowHeight);

		auto edgePixels = LineDrawer::drawLine(
			qRound(p1.x()), qRound(p1.y()),
			qRound(p2.x()), qRound(p2.y())
		);

		// 将边界线像素添加到结果中
		for (const auto& pixel : edgePixels) {
			pixels.append(pixel);
		}
	}

	return pixels;
}

// 获取面的像素点（填充）- 简化版本避免性能问题
QVector<QVector2D> Cube::getFacePixels(int faceIndex, int windowWidth, int windowHeight) const
{
	QVector<QVector2D> facePixels;

	if (faceIndex < 0 || faceIndex >= m_faces.size()) return facePixels;

	const auto& face = m_faces[faceIndex];

	// 获取面的2D投影顶点
	QVector<QVector2D> vertices2D;
	for (int vertexIndex : face) {
		vertices2D.append(projectTo2D(m_vertices[vertexIndex], windowWidth, windowHeight));
	}

	// 简单实现：绘制面的边界和内部一些点来表示填充
	// 避免使用复杂的扫描线算法导致性能问题

	// 绘制边界
	for (int i = 0; i < 4; i++) {
		int j = (i + 1) % 4;
		QVector2D p1 = vertices2D[i];
		QVector2D p2 = vertices2D[j];

		auto linePixels = LineDrawer::drawLine(
			qRound(p1.x()), qRound(p1.y()),
			qRound(p2.x()), qRound(p2.y())
		);

		facePixels += linePixels;
	}

	// 在面内部绘制一些点表示填充
	if (vertices2D.size() == 4) {
		// 计算面的中心
		QVector2D center(0, 0);
		for (const auto& vertex : vertices2D) {
			center += vertex;
		}
		center /= 4;

		// 在中心周围绘制一些点
		for (int i = -2; i <= 2; i++) {
			for (int j = -2; j <= 2; j++) {
				facePixels.append(QVector2D(center.x() + i, center.y() + j));
			}
		}
	}

	return facePixels;
}

// 绘制三角形边框（用于调试）
void Cube::drawTriangleBorder(QVector<QVector2D>& pixels, const QVector2D& v1, const QVector2D& v2, const QVector2D& v3) const
{
	auto line1 = LineDrawer::drawLine(qRound(v1.x()), qRound(v1.y()), qRound(v2.x()), qRound(v2.y()));
	auto line2 = LineDrawer::drawLine(qRound(v2.x()), qRound(v2.y()), qRound(v3.x()), qRound(v3.y()));
	auto line3 = LineDrawer::drawLine(qRound(v3.x()), qRound(v3.y()), qRound(v1.x()), qRound(v1.y()));

	pixels += line1;
	pixels += line2;
	pixels += line3;
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