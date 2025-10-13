// Cube.cpp
#include "Cube.h"
#include "LineDrawer.h"
#include <cmath>

// Ĭ�Ϲ��캯��
Cube::Cube()
	: m_center(0, 0, -5), m_size(1.0f), m_color(Qt::white),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(m_size);
}

// ���캯��
Cube::Cube(const QVector3D& center, float size, const QColor& color)
	: m_center(center), m_size(size), m_color(color),
	m_rotateAngles(0, 0, 0), m_translate(0, 0, 0)
{
	initVertices(size);
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

// 3D����ͶӰ��2D����
QVector2D Cube::projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const
{
	// Ӧ��ģ�ͱ任
	QVector3D transformed = applyModelTransform(vertex);

	// ��͸��ͶӰ
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

// Ӧ�þ���任
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

// ������Ƿ�ɼ�
bool Cube::isFaceVisible(const int face[4]) const
{
	// �򻯵Ŀɼ��Լ�飺�����淨���������߷���
	// ����ʹ�ü򵥵ı����޳�
	QVector3D v0 = m_vertices[face[0]];
	QVector3D v1 = m_vertices[face[1]];
	QVector3D v2 = m_vertices[face[2]];

	// �����淨����
	QVector3D edge1 = v1 - v0;
	QVector3D edge2 = v2 - v0;
	QVector3D normal = QVector3D::crossProduct(edge1, edge2);
	normal.normalize();

	// ���߷��򣨴�����ָ�������
	QVector3D viewDir(0, 0, 1); // �򻯼��������Z��������

	// ��������������߷���нǴ���90�ȣ�����ɼ�
	return QVector3D::dotProduct(normal, viewDir) < 0;
}

// ��ȡ������ص�
QVector<QVector2D> Cube::getFacePixels(const int face[4], int windowWidth, int windowHeight) const
{
	// �������ʵ������䣬��������ҵҪ�����������������ֻ���߽���
	// ��ʱ���ؿգ�ֻ���Ʊ���
	return QVector<QVector2D>();
}

// ��ȡ������ͶӰ������ص�
QVector<QVector2D> Cube::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	// ������12���ߵĶ�������
	const int edges[12][2] = {
		{0,1}, {1,2}, {2,3}, {3,0},  // ǰ��
		{4,5}, {5,6}, {6,7}, {7,4},  // ����
		{0,4}, {1,5}, {2,6}, {3,7}   // ���ӱ�
	};

	// ����ÿ���ߵ�����
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