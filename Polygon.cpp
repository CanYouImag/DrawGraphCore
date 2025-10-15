#include "Polygon.h"
#include "LineDrawer.h"
#include <algorithm>
#include <QDebug>

// ����ģ�嶨��
namespace DigitTemplates {
	// ���� 0
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

	// ���� 1
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

	// ���� 2
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

	// ���� 3
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

// ���캯��
Polygon::Polygon(const QVector<QVector2D>& vertices, const QColor& borderColor, const QColor& idColor)
	: m_vertices(vertices.size() >= 3 ? vertices : QVector<QVector2D>()),
	m_borderColor(borderColor), m_idColor(idColor),
	m_fillMode(FillMode::WithId)  // Ĭ��ʹ��ѧ�����
{
	// ����֤��������
	for (int i = 0; i < m_vertices.size(); ++i) {
		if (m_vertices[i].x() < 0 || m_vertices[i].y() < 0) {
			qDebug() << "Warning: Polygon vertex has negative coordinates";
		}
	}
}

// ��ȡ����ε����ص㣨�߽��ѧ�����֣�
QVector<QVector2D> Polygon::getPixels(int windowWidth, int windowHeight) const
{
	QVector<QVector2D> pixels;

	if (m_vertices.isEmpty()) return pixels;

	// 1. ���ƶ���α߽�
	pixels += getBorderPixels();

	// 2. ��ѧ��������������ڲ�
	if (m_fillMode != FillMode::None)
	{
		fillWithStudentId(pixels, windowWidth, windowHeight);
	}

	return pixels;
}

// ��ȡ�߽����ص�
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

// ��ѧ�����������������
void Polygon::fillWithStudentId(QVector<QVector2D>& pixels, int windowWidth, int windowHeight) const
{
	if (m_vertices.size() < 3) return;

	// �������εİ�Χ��
	float minX = m_vertices[0].x(), maxX = m_vertices[0].x();
	float minY = m_vertices[0].y(), maxY = m_vertices[0].y();

	for (const auto& vertex : m_vertices) {
		minX = std::min(minX, vertex.x());
		maxX = std::max(maxX, vertex.x());
		minY = std::min(minY, vertex.y());
		maxY = std::max(maxY, vertex.y());
	}

	// ���ֵĳߴ�ͼ��
	const int digitWidth = 8;
	const int digitHeight = 16;
	const int horizontalSpacing = 12;
	const int verticalSpacing = 20;

	// ѧ�ź���λ "1032"
	int studentId[] = { 1, 0, 3, 2 };
	int idLength = 4;

	// ����ѧ������Ŀ��
	int totalIdWidth = idLength * digitWidth + (idLength - 1) * horizontalSpacing;

	// �ڶ�����ڲ����������ѧ��
	for (int y = static_cast<int>(minY); y < maxY; y += verticalSpacing) {
		for (int x = static_cast<int>(minX); x < maxX; x += totalIdWidth + horizontalSpacing) {
			// ������������Ƿ��ڶ������
			QVector2D gridCenter(x + totalIdWidth / 2, y + digitHeight / 2);

			if (isPointInPolygon(gridCenter)) {
				// ���������λ�û���ѧ��
				for (int i = 0; i < idLength; i++) {
					drawDigit(pixels, getDigitTemplate(studentId[i]),
						x + i * (digitWidth + horizontalSpacing), y);
				}
			}
		}
	}
}

// ��������
void Polygon::drawDigit(QVector<QVector2D>& pixels, const int digit[16][8],
	int startX, int startY) const
{
	for (int y = 0; y < 16; y++) {
		for (int x = 0; x < 8; x++) {
			if (digit[y][x] == 1) {
				int pixelX = startX + x;
				// ��תY���꣺����ѧ����ת��Ϊ��Ļ����
				int pixelY = startY + (15 - y);  // 16�����֣���0��15�����Է�ת��15-y

				// ��������Ƿ��ڶ�����ڲ�
				if (isPointInPolygon(QVector2D(pixelX, pixelY))) {
					pixels.append(QVector2D(pixelX, pixelY));
				}
			}
		}
	}
}

// �жϵ��Ƿ��ڶ�����ڲ���ʹ�����߷���
bool Polygon::isPointInPolygon(const QVector2D& point) const {
	if (m_vertices.size() < 3) return false;

	bool inside = false;
	int n = m_vertices.size();

	for (int i = 0, j = n - 1; i < n; j = i++) {
		const QVector2D& vi = m_vertices[i];
		const QVector2D& vj = m_vertices[j];

		// �����Ƿ��ڶ���α߽���
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

// ��ȡ����ģ��
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

// ��ȡ��ɫ�����ر߽���ɫ��
QColor Polygon::color() const
{
	return m_borderColor;
}