// BezierCurve.h
#pragma once
#include "Shape.h"

class BezierCurve : public Shape
{
public:
    BezierCurve() = default;

    // 构造函数
    BezierCurve(const QVector<QVector2D>& controlPoints, const QColor& color);

    // 获取控制点
    QVector<QVector2D> controlPoints() const { return m_controlPoints; }

    // 获取颜色
    QColor color() const override { return m_color; }

    // 获取曲线上的像素点
    QVector<QVector2D> getPixels(int windowWidth, int windowHeight) const override;

private:
    // 生成Bezier曲线点
    QVector<QVector2D> generateCurvePoints() const;

    QVector<QVector2D> m_controlPoints;  // 控制点
    QColor m_color;                      // 颜色
};