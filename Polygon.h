// Polygon.h
#pragma once
#include "Shape.h"

// ���ģʽö��
enum class FillMode {
	None,        // �����
	EdgeFlag,    // �߱�־�㷨���
	FloodFill,   // �������
	ScanLine     // ɨ�������
};

// ������ࣺ�̳���Shape���֧࣬����书��
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

	// ��ȡ����ε����ص㣨�߽����䣩
	// ����: width, height - ���ڳߴ�
	// ����: ��������ص㼯��
	QVector<QVector2D> getPixels(int width, int height) const override;

	// ��ȡ��ɫ�����ر߽���ɫ��
	QColor color() const override;

private:
	// ���ѧ�����
	// ����: filledPixels - ������ؼ���, windowWidth, windowHeight - ���ڳߴ�
	void addStudentId(QVector<QVector2D>& filledPixels, int windowWidth, int windowHeight) const;

	// ��������
	// ����: filledPixels - ������ؼ���, digit - ����ģ��, startX, startY - ��ʼλ��
	void drawDigit(QVector<QVector2D>& filledPixels, const int digit[16][8],
		int startX, int startY) const;

	// ��ȡ�߽����ص�
	QVector<QVector2D> getBorderPixels() const;

	// ��ȡ������ص�
	QVector<QVector2D> getFilledPixels(int windowWidth, int windowHeight) const;

	// ��Ǳ�
	void markEdge(QVector<QVector<bool>>& edgeFlags, const QVector2D& p1,
		const QVector2D& p2, int width, int height) const;

	QVector<QVector2D> m_vertices;    // ���㼯��
	QColor m_borderColor;             // �߽���ɫ
	QColor m_idColor;                 // ѧ����ɫ
	FillMode m_fillMode = FillMode::None;  // ���ģʽ
};