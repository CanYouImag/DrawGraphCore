// Shape.h
#pragma once
#include <QVector2D>
#include <QColor>
#include <QVector>

// ���пɻ���ͼԪ�ĳ������
class Shape
{
public:
	virtual ~Shape() = default;

	// ��ȡ��ͼ�ε��������ص㣨����OpenGL���ƣ�
	// ����: windowWidth, windowHeight - ���ڳߴ�
	// ����: ͼ�����ص㼯��
	virtual QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const = 0;

	// ��ȡͼ����ɫ
	// ����: ͼ����ɫ
	virtual QColor color() const = 0;
};