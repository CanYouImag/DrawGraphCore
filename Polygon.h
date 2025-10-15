#pragma once
#include "Shape.h"
#include <QVector2D>
#include <QColor>

// ���ģʽö��
enum class FillMode {
	None,        // �����
	WithId       // ��ѧ���������
};

// ������ࣺ�̳���Shape���֧࣬��ѧ��������书��
class Polygon : public Shape
{
public:
	Polygon() = default;

	// ���캯��
	// ����: vertices - ���㼯��, borderColor - �߽���ɫ, idColor - ѧ����ɫ
	Polygon(const QVector<QVector2D>& vertices, const QColor& borderColor, const QColor& idColor);

	// ��ȡ���㼯��
	QVector<QVector2D> vertices() const { return m_vertices; }

	// ��ȡ�߽���ɫ
	QColor borderColor() const { return m_borderColor; }

	// ��ȡѧ����ɫ
	QColor idColor() const { return m_idColor; }

	// ��ȡ���ģʽ
	FillMode fillMode() const { return m_fillMode; }

	// �������ģʽ
	void setFillMode(FillMode mode) { m_fillMode = mode; }

	// ��ȡ����ε����ص㣨�߽��ѧ�����֣�
	// ����: width, height - ���ڳߴ�
	// ����: ��������ص㼯��
	QVector<QVector2D> getPixels(int width, int height) const override;

	// ��ȡ��ɫ�����ر߽���ɫ��
	QColor color() const override;

private:
	// ��ѧ�����������������
	void fillWithStudentId(QVector<QVector2D>& pixels, int windowWidth, int windowHeight) const;

	// �жϵ��Ƿ��ڶ�����ڲ�
	bool isPointInPolygon(const QVector2D& point) const;

	// ��ȡ����ģ��
	const int (*getDigitTemplate(int digit) const)[8];

	// ��������
	void drawDigit(QVector<QVector2D>& pixels, const int digit[16][8],
		int startX, int startY) const;

	// ��ȡ�߽����ص�
	QVector<QVector2D> getBorderPixels() const;

	QVector<QVector2D> m_vertices;    // ���㼯��
	QColor m_borderColor;             // �߽���ɫ
	QColor m_idColor;                 // ѧ����ɫ
	FillMode m_fillMode = FillMode::None;  // ���ģʽ
};