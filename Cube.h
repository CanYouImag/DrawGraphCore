#pragma once
#include "Shape.h"
#include <QVector3D>
#include <QColor>

class Cube : public Shape
{
public:
	Cube();
	Cube(const QVector3D& center, float size, const QColor& color);

	QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;
	QColor color() const override { return m_color; }

	void translate(const QVector3D& delta);
	void rotate(const QVector3D& angles);
	void reset();

	QVector3D getRotateAngles() const { return m_rotateAngles; }
	QVector3D getTranslate() const { return m_translate; }

private:
	void initVertices();
	QVector2D projectTo2D(const QVector3D& vertex, int windowWidth, int windowHeight) const;
	QVector3D applyTransform(const QVector3D& vertex) const;

	QVector3D m_center;
	QVector3D m_rotateAngles;
	QVector3D m_translate;
	QVector<QVector3D> m_vertices;
	QColor m_color;
	float m_size;
};