// Rectangle.h
#pragma once
#include "Shape.h"

// �����ࣺ�̳���Shape����
class Rectangle : public Shape
{
public:
	Rectangle() = default;

	// ���캯��
	// ����: p1, p2 - �Խǵ�����, color - ��ɫ
	Rectangle(const QVector2D& p1, const QVector2D& p2, const QColor& color);

	// ��ȡ��һ���ǵ�����
	QVector2D p1() const { return m_p1; }

	// ��ȡ�ڶ����ǵ�����
	QVector2D p2() const { return m_p2; }

	// ��ȡ��ɫ
	QColor color() const override { return m_color; }

	// ��ȡ���������ߵ����ص�
	// ����: windowWidth, windowHeight - ���ڳߴ�
	// ����: ���α߽�����ص㼯��
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
	QVector2D m_p1;     // ��һ���ǵ�
	QVector2D m_p2;     // �ڶ����ǵ�
	QColor m_color;     // ��ɫ
};