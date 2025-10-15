// BezierCurve.cpp
#include "BezierCurve.h"
#include "LineDrawer.h"

BezierCurve::BezierCurve(const QVector<QVector2D>& controlPoints, const QColor& color)
    : m_controlPoints(controlPoints), m_color(color)
{
}

QVector<QVector2D> BezierCurve::getPixels(int windowWidth, int windowHeight) const
{
    QVector<QVector2D> curvePixels;

    if (m_controlPoints.size() != 4) {
        return curvePixels;
    }

    // 生成曲线上的点
    QVector<QVector2D> curvePoints = generateCurvePoints();

    // 将曲线点连接成线段
    for (int i = 0; i < curvePoints.size() - 1; ++i) {
        QVector2D p1 = curvePoints[i];
        QVector2D p2 = curvePoints[i + 1];

        int x1 = static_cast<int>(p1.x());
        int y1 = static_cast<int>(p1.y());
        int x2 = static_cast<int>(p2.x());
        int y2 = static_cast<int>(p2.y());

        curvePixels += LineDrawer::drawLine(x1, y1, x2, y2);
    }

    return curvePixels;
}

QVector<QVector2D> BezierCurve::generateCurvePoints() const
{
    QVector<QVector2D> curvePoints;

    if (m_controlPoints.size() == 4) {
        // 3次Bezier曲线公式
        for (float t = 0; t <= 1.0f; t += 0.01f) {
            float u = 1.0f - t;
            float b0 = u * u * u;
            float b1 = 3 * u * u * t;
            float b2 = 3 * u * t * t;
            float b3 = t * t * t;

            float x = m_controlPoints[0].x() * b0 +
                m_controlPoints[1].x() * b1 +
                m_controlPoints[2].x() * b2 +
                m_controlPoints[3].x() * b3;

            float y = m_controlPoints[0].y() * b0 +
                m_controlPoints[1].y() * b1 +
                m_controlPoints[2].y() * b2 +
                m_controlPoints[3].y() * b3;

            curvePoints.append(QVector2D(x, y));
        }
    }

    return curvePoints;
}