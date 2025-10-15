// Cube.cpp
#include "Cube.h"
#include "LineDrawer.h"
#include <cmath>
#include <algorithm>
#include <QVector>

// Ĭ�Ϲ��캯��
Cube::Cube()
	: m_center(0, 0, -5), m_size(1.0f), m_color(Qt::white),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(m_size);
	initFaces();
	initFaceColors();
}

// ���캯��
Cube::Cube(const QVector3D& center, float size, const QColor& color)
	: m_center(center), m_size(size), m_color(color),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(size);
	initFaces();
	initFaceColors();
}

// ��ʼ�������嶥��
void Cube::initVertices(float size)
{
	// ��������ϵ���㣨������ԭ�㣩
	float half = size / 2;
	m_vertices = {
		QVector3D(-half, -half, -half), // 0: ��ǰ��
		QVector3D(half, -half, -half),  // 1: ��ǰ��
		QVector3D(half, half, -half),   // 2: ��ǰ��
		QVector3D(-half, half, -half),  // 3: ��ǰ��
		QVector3D(-half, -half, half),  // 4: �����
		QVector3D(half, -half, half),   // 5: �Һ���
		QVector3D(half, half, half),    // 6: �Һ���
		QVector3D(-half, half, half)    // 7: �����
	};
}

// ��ʼ��������
void Cube::initFaces()
{
	// ����6���棬ÿ����4�����㣨˳ʱ�뷽�򣬴��ⲿ����
	m_faces = {
		{0, 1, 2, 3},  // ǰ��
		{5, 4, 7, 6},  // ����
		{4, 0, 3, 7},  // ����
		{1, 5, 6, 2},  // ����
		{3, 2, 6, 7},  // ����
		{4, 5, 1, 0}   // ����
	};
}

// ��ʼ������ɫ
void Cube::initFaceColors()
{
	m_faceColors = {
		QColor(255, 0, 0),      // ǰ�� - ��ɫ
		QColor(0, 255, 0),      // ���� - ��ɫ
		QColor(0, 0, 255),      // ���� - ��ɫ
		QColor(255, 255, 0),    // ���� - ��ɫ
		QColor(255, 0, 255),    // ���� - ���
		QColor(0, 255, 255)     // ���� - ��ɫ
	};
}

// 3D����ͶӰ��2D����
QVector2D Cube::projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const
{
	// Ӧ��ģ�ͱ任
	QVector3D transformed = applyModelTransform(vertex);

	// ͸��ͶӰ
	float distance = 5.0f; // �۲����
	float perspective = distance / (distance + transformed.z());

	// Ӧ��͸��
	float x = transformed.x() * perspective;
	float y = transformed.y() * perspective;

	// ӳ�䵽��������
	float winX = (x + 1.0f) * 0.5f * windowWidth;
	float winY = (1.0f - y) * 0.5f * windowHeight;

	return QVector2D(winX, winY);
}

// Ӧ��ģ�ͱ任
QVector3D Cube::applyModelTransform(const QVector3D& vertex) const
{
	QVector3D result = vertex;

	// Ӧ����ת����X��Y��Z�ᣩ
	result = rotateX(result, m_rotateAngles.x());
	result = rotateY(result, m_rotateAngles.y());
	result = rotateZ(result, m_rotateAngles.z());

	// Ӧ��ƽ��
	result += m_translate + m_center;

	return result;
}

// ��X����ת
QVector3D Cube::rotateX(const QVector3D& vertex, float angle) const
{
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);

	float y = vertex.y() * cosA - vertex.z() * sinA;
	float z = vertex.y() * sinA + vertex.z() * cosA;

	return QVector3D(vertex.x(), y, z);
}

// ��Y����ת
QVector3D Cube::rotateY(const QVector3D& vertex, float angle) const
{
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);

	float x = vertex.x() * cosA + vertex.z() * sinA;
	float z = -vertex.x() * sinA + vertex.z() * cosA;

	return QVector3D(x, vertex.y(), z);
}

// ��Z����ת
QVector3D Cube::rotateZ(const QVector3D& vertex, float angle) const
{
	float rad = angle * M_PI / 180.0f;
	float cosA = cos(rad);
	float sinA = sin(rad);

	float x = vertex.x() * cosA - vertex.y() * sinA;
	float y = vertex.x() * sinA + vertex.y() * cosA;

	return QVector3D(x, y, vertex.z());
}

// ������Ƿ�ɼ�
bool Cube::isFaceVisible(int faceIndex) const
{
	if (faceIndex < 0 || faceIndex >= m_faces.size()) return false;

	const auto& face = m_faces[faceIndex];

	// ��ȡ����������㣨���ڼ��㷨������
	QVector3D v0 = applyModelTransform(m_vertices[face[0]]);
	QVector3D v1 = applyModelTransform(m_vertices[face[1]]);
	QVector3D v2 = applyModelTransform(m_vertices[face[2]]);

	// �����淨����
	QVector3D edge1 = v1 - v0;
	QVector3D edge2 = v2 - v0;
	QVector3D normal = QVector3D::crossProduct(edge1, edge2);

	// �������������Ϊ0��˵�����˻�
	if (normal.length() < 0.0001f) return false;

	normal.normalize();

	// ���߷��򣨴�����ָ�������
	QVector3D viewDir(0, 0, 1);

	// ��������������߷���н�С��90�ȣ�����ɼ�
	return QVector3D::dotProduct(normal, viewDir) < 0;
}

// ��ȡ������ͶӰ������ص�
QVector<QVector2D> Cube::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	// 1. �Ȼ��ƿɼ�������
	for (int i = 0; i < m_faces.size(); ++i) {
		if (isFaceVisible(i)) {
			// ��ȡ���2DͶӰ����
			QVector<QVector2D> vertices2D;
			const auto& face = m_faces[i];
			for (int vertexIndex : face) {
				vertices2D.append(projectTo2D(m_vertices[vertexIndex], windowWidth, windowHeight));
			}

			// ����䣺������ı߽��ߣ���ʱ�����������������⣩
			for (int j = 0; j < 4; j++) {
				int startIdx = j;
				int endIdx = (j + 1) % 4;
				QVector2D p1 = vertices2D[startIdx];
				QVector2D p2 = vertices2D[endIdx];

				// ʹ�������ɫ���Ʊ߽�
				auto linePixels = LineDrawer::drawLine(
					qRound(p1.x()), qRound(p1.y()),
					qRound(p2.x()), qRound(p2.y())
				);

				// ��ӱ߽�����
				for (const auto& pixel : linePixels) {
					pixels.append(pixel);
				}
			}

			// �������Ļ���һ�����ʾ�����ɫ
			QVector2D center(0, 0);
			for (const auto& vertex : vertices2D) {
				center += vertex;
			}
			center /= vertices2D.size();

			// �������Ļ���3x3�ĵ����ʾ���
			for (int dx = -1; dx <= 1; dx++) {
				for (int dy = -1; dy <= 1; dy++) {
					pixels.append(QVector2D(center.x() + dx, center.y() + dy));
				}
			}
		}
	}

	// 2. �������б߽��ߣ���ɫ��
	const int edges[12][2] = {
		{0,1}, {1,2}, {2,3}, {3,0},  // ǰ��
		{4,5}, {5,6}, {6,7}, {7,4},  // ����
		{0,4}, {1,5}, {2,6}, {3,7}   // ���ӱ�
	};

	// ����ÿ���ߵ�����
	for (auto& edge : edges) {
		QVector2D p1 = projectTo2D(m_vertices[edge[0]], windowWidth, windowHeight);
		QVector2D p2 = projectTo2D(m_vertices[edge[1]], windowWidth, windowHeight);

		auto edgePixels = LineDrawer::drawLine(
			qRound(p1.x()), qRound(p1.y()),
			qRound(p2.x()), qRound(p2.y())
		);

		// ���߽���������ӵ������
		for (const auto& pixel : edgePixels) {
			pixels.append(pixel);
		}
	}

	return pixels;
}

// ��ȡ������ص㣨��䣩- �򻯰汾������������
QVector<QVector2D> Cube::getFacePixels(int faceIndex, int windowWidth, int windowHeight) const
{
	QVector<QVector2D> facePixels;

	if (faceIndex < 0 || faceIndex >= m_faces.size()) return facePixels;

	const auto& face = m_faces[faceIndex];

	// ��ȡ���2DͶӰ����
	QVector<QVector2D> vertices2D;
	for (int vertexIndex : face) {
		vertices2D.append(projectTo2D(m_vertices[vertexIndex], windowWidth, windowHeight));
	}

	// ��ʵ�֣�������ı߽���ڲ�һЩ������ʾ���
	// ����ʹ�ø��ӵ�ɨ�����㷨������������

	// ���Ʊ߽�
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

	// �����ڲ�����һЩ���ʾ���
	if (vertices2D.size() == 4) {
		// �����������
		QVector2D center(0, 0);
		for (const auto& vertex : vertices2D) {
			center += vertex;
		}
		center /= 4;

		// ��������Χ����һЩ��
		for (int i = -2; i <= 2; i++) {
			for (int j = -2; j <= 2; j++) {
				facePixels.append(QVector2D(center.x() + i, center.y() + j));
			}
		}
	}

	return facePixels;
}

// ���������α߿����ڵ��ԣ�
void Cube::drawTriangleBorder(QVector<QVector2D>& pixels, const QVector2D& v1, const QVector2D& v2, const QVector2D& v3) const
{
	auto line1 = LineDrawer::drawLine(qRound(v1.x()), qRound(v1.y()), qRound(v2.x()), qRound(v2.y()));
	auto line2 = LineDrawer::drawLine(qRound(v2.x()), qRound(v2.y()), qRound(v3.x()), qRound(v3.y()));
	auto line3 = LineDrawer::drawLine(qRound(v3.x()), qRound(v3.y()), qRound(v1.x()), qRound(v1.y()));

	pixels += line1;
	pixels += line2;
	pixels += line3;
}

// ƽ�Ʊ任
void Cube::translate(const QVector3D& delta)
{
	m_translate += delta;
}

// ��ת�任
void Cube::rotate(const QVector3D& angles)
{
	m_rotateAngles += angles;
}

// ���ñ任
void Cube::reset()
{
	m_rotateAngles = QVector3D(0, 0, 0);
	m_translate = QVector3D(0, 0, 0);
}