// BezierCurve.h
#pragma once
#include "Shape.h"

class BezierCurve : public Shape
{
public:
    BezierCurve() = default;

    // ���캯��
    BezierCurve(const QVector<QVector2D>& controlPoints, const QColor& color);

    // ��ȡ���Ƶ�
    QVector<QVector2D> controlPoints() const { return m_controlPoints; }

    // ��ȡ��ɫ
    QColor color() const override { return m_color; }

    // ��ȡ�����ϵ����ص�
    QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
    // ����Bezier���ߵ�
    QVector<QVector2D> generateCurvePoints() const;

    QVector<QVector2D> m_controlPoints;  // ���Ƶ�
    QColor m_color;                      // ��ɫ
};