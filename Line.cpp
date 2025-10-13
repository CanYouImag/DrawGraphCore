// Line.cpp
#include "Line.h"
#include "LineDrawer.h"

// ���캯��
Line::Line(const QVector2D& p1, const QVector2D& p2, const QColor& color)
	: m_p1(p1), m_p2(p2), m_color(color) {}

// ��ȡ�߶����������ص�
QVector<QVector2D> Line::getPixels(int windowWidth, int windowHeight) const
{
	// ����������תΪ��������
	int x0 = static_cast<int>(m_p1.x());
	int y0 = static_cast<int>(m_p1.y());
	int x1 = static_cast<int>(m_p2.x());
	int y1 = static_cast<int>(m_p2.y());

	// �߽��飨��ѡ�Ż���
	if (x0 < 0 || x0 >= windowWidth || y0 < 0 || y0 >= windowHeight || x1 < 0 || x1 >= windowWidth || y1 < 0 || y1 >= windowHeight)
	{
		return LineDrawer::drawLine(x0, y0, x1, y1);
	}

	return LineDrawer::drawLine(x0, y0, x1, y1);
}