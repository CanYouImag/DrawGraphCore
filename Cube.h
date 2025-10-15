// Cube.h
#pragma once
#include "Shape.h"
#include <QVector3D>
#include <QColor>

// �������ࣺ�̳���Shape���࣬ʵ��3D�����幦��
class Cube : public Shape
{
public:
	Cube();

	// ���캯��
	// ����: center - ��������, size - �߳�, color - �߽���ɫ
	Cube(const QVector3D& center, float size, const QColor& color);

	// ��ȡ������ͶӰ������ص�
	// ����: windowWidth, windowHeight - ���ڳߴ�
	// ����: ��������ͱ߽�����ص㼯��
	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

	// ��ȡ��ɫ�����ر߽���ɫ��
	QColor color() const override { return m_color; }

	// ƽ�Ʊ任
	// ����: delta - ƽ������
	void translate(const QVector3D& delta);

	// ��ת�任
	// ����: angles - ��ת�Ƕ�����
	void rotate(const QVector3D& angles);

	// ���ñ任
	void reset();

	// ��ȡ��ת�Ƕ�
	QVector3D getRotateAngles() const { return m_rotateAngles; }

	// ��ȡƽ������
	QVector3D getTranslate() const { return m_translate; }

private:
	// ��ʼ�������嶥��
	// ����: size - ������߳�
	void initVertices(float size);

	// ��ʼ��������
	void initFaces();

	// ��ʼ������ɫ
	void initFaceColors();

	// 3D����ͶӰ��2D����
	// ����: vertex - 3D��������, windowWidth, windowHeight - ���ڳߴ�
	// ����: ͶӰ���2D����
	QVector2D projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const;

	// Ӧ��ģ�ͱ任
	QVector3D applyModelTransform(const QVector3D& vertex) const;

	// ��X����ת
	QVector3D rotateX(const QVector3D& vertex, float angle) const;

	// ��Y����ת  
	QVector3D rotateY(const QVector3D& vertex, float angle) const;

	// ��Z����ת
	QVector3D rotateZ(const QVector3D& vertex, float angle) const;

	// ������Ƿ�ɼ�
	bool isFaceVisible(int faceIndex) const;

	// ��ȡ������ص㣨��䣩
	QVector<QVector2D> getFacePixels(int faceIndex, int windowWidth, int windowHeight) const;

	// ���������α߿����ڵ��ԣ�
	void drawTriangleBorder(QVector<QVector2D>& pixels, const QVector2D& v1, const QVector2D& v2, const QVector2D& v3) const;

	QVector3D m_center;           // ��������������
	QVector3D m_rotateAngles;     // ��ת�Ƕȣ�X/Y/Z�ᣬ�ȣ�
	QVector3D m_translate;        // ƽ������
	QVector<QVector3D> m_vertices; // ������8������
	QVector<QVector<int>> m_faces; // ��Ķ�������
	QVector<QColor> m_faceColors;  // ÿ�������ɫ
	QColor m_color;               // �߽���ɫ
	float m_size;                 // ������߳�
};