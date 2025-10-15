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

	// �����������8������
	m_vertices = {
		QVector3D(-s, -s, -s), // 0: ��ǰ��
		QVector3D(s, -s, -s), // 1: ��ǰ��  
		QVector3D(s,  s, -s), // 2: ��ǰ��
		QVector3D(-s,  s, -s), // 3: ��ǰ��
		QVector3D(-s, -s,  s), // 4: �����
		QVector3D(s, -s,  s), // 5: �Һ���
		QVector3D(s,  s,  s), // 6: �Һ���
		QVector3D(-s,  s,  s)  // 7: �����
	};
}

QVector2D Cube::projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const
{
	// Ӧ�ñ任
	QVector3D transformed = applyTransform(vertex);

	// �򵥵�͸��ͶӰ
	float distance = 3.0f;
	float scale = distance / (distance + transformed.z());

	// Ӧ��͸��
	float x = transformed.x() * scale;
	float y = transformed.y() * scale;

	// ӳ�䵽��Ļ����
	float screenX = (x + 1.0f) * 0.5f * windowWidth;
	float screenY = (1.0f - (y + 1.0f) * 0.5f) * windowHeight;

	return QVector2D(screenX, screenY);
}

QVector3D Cube::applyTransform(const QVector3D& vertex) const
{
	QVector3D result = vertex;

	// Ӧ����ת
	float rx = m_rotateAngles.x() * M_PI / 180.0f;
	float ry = m_rotateAngles.y() * M_PI / 180.0f;
	float rz = m_rotateAngles.z() * M_PI / 180.0f;

	// ��X����ת
	if (rx != 0) {
		float cosX = cos(rx);
		float sinX = sin(rx);
		float y = result.y() * cosX - result.z() * sinX;
		float z = result.y() * sinX + result.z() * cosX;
		result.setY(y);
		result.setZ(z);
	}

	// ��Y����ת
	if (ry != 0) {
		float cosY = cos(ry);
		float sinY = sin(ry);
		float x = result.x() * cosY + result.z() * sinY;
		float z = -result.x() * sinY + result.z() * cosY;
		result.setX(x);
		result.setZ(z);
	}

	// ��Z����ת
	if (rz != 0) {
		float cosZ = cos(rz);
		float sinZ = sin(rz);
		float x = result.x() * cosZ - result.y() * sinZ;
		float y = result.x() * sinZ + result.y() * cosZ;
		result.setX(x);
		result.setY(y);
	}

	// Ӧ��ƽ��
	result += m_translate + m_center;

	return result;
}

QVector<QVector2D> Cube::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	// ֻ���Ʊ��ߣ��������
	const int edges[12][2] = {
		{0,1}, {1,2}, {2,3}, {3,0},  // ǰ��
		{4,5}, {5,6}, {6,7}, {7,4},  // ����
		{0,4}, {1,5}, {2,6}, {3,7}   // ���ӱ�
	};

	// ����ÿ���ߵ�����
	for (int i = 0; i < 12; i++) {
		int v1 = edges[i][0];
		int v2 = edges[i][1];

		QVector2D p1 = projectTo2D(m_vertices[v1], windowWidth, windowHeight);
		QVector2D p2 = projectTo2D(m_vertices[v2], windowWidth, windowHeight);

		// ����ֱ��
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