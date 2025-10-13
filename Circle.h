// Circle.h
#pragma once
#include "Shape.h"

// Բ���ࣺ�̳���Shape����
class Circle : public Shape
{
public:
	Circle() = default;

	// ���캯��
	// ����: center - Բ������, radius - �뾶, color - ��ɫ
	Circle(const QVector2D& center, int radius, const QColor& color);

	// ��ȡԲ������
	QVector2D center() const { return m_center; }

	// ��ȡ�뾶
	int radius() const { return m_radius; }

	// ��ȡ��ɫ
	QColor color() const override { return m_color; }

	// ��ȡԲ�α߽��ϵ��������ص�
	// ����: windowWidth, windowHeight - ���ڳߴ�
	// ����: Բ�α߽����ص㼯��
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
	QVector2D m_center;  // Բ������
	int m_radius;        // �뾶
	QColor m_color;      // ��ɫ
};