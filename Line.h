// Line.h
#pragma once
#include "Shape.h"

// ֱ���ࣺ�̳���Shape����
class Line : public Shape
{
public:
	Line() = default;

	// ���캯��
	// ����: p1, p2 - �˵�����, color - ��ɫ
	Line(const QVector2D& p1, const QVector2D& p2, const QColor& color);

	// ��ȡ�������
	QVector2D p1() const { return m_p1; }

	// ��ȡ�յ�����
	QVector2D p2() const { return m_p2; }

	// ��ȡ��ɫ
	QColor color() const override { return m_color; }

	// ��ȡ�߶����������ص�
	// ����: windowWidth, windowHeight - ���ڳߴ�
	// ����: ֱ���ϵ����ص㼯��
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
	QVector2D m_p1;     // �������
	QVector2D m_p2;     // �յ�����
	QColor m_color;     // ��ɫ
};