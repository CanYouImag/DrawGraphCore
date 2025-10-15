// glwidget.h
#pragma once
#include <QOpenGLWidget>
#include <QOpenGLFunctions>
#include <QOpenGLShaderProgram>
#include <QTimer>
#include <QVector2D>
#include <QVector3D>
#include <QVector>
#include <QColor>
#include "Shape.h"
#include "ShapeManager.h"
#include "Cube.h"

// 前向声明
class BezierCurve;
// 绘制模式枚举
enum class DrawMode
{
	None,           // 无操作
	Line,           // 直线
	Circle,         // 圆
	Rectangle,      // 矩形
	Polygon,        // 多边形
	Cube,           // 立方体
	BezierCurve     // Bezier曲线
};

// 变换模式枚举
enum class TransformMode
{
	None,           // 无变换
	TranslateX,     // X轴平移
	TranslateY,     // Y轴平移
	TranslateZ,     // Z轴平移
	RotateX,        // 绕X轴旋转
	RotateY,        // 绕Y轴旋转
	RotateZ         // 绕Z轴旋转
};

// OpenGL绘制组件类：负责图形绘制和交互
class GLWidget : public QOpenGLWidget, protected QOpenGLFunctions
{
	Q_OBJECT

public:
	explicit GLWidget(QWidget* parent = nullptr);
	~GLWidget();

	// 设置平移步长
	void setTranslateStep(int step) { m_translateStep = step; }

	// 设置旋转角度
	void setRotateAngle(int angle) { m_rotateAngle = angle; }

	// 处理键盘变换操作
	// 参数: positive - 是否正向变换, multiplier - 变换倍数
	void handleKeyTransform(bool positive, int multiplier);

	// 获取当前绘制模式
	DrawMode getCurrentDrawMode() const { return m_currentMode; }

	// 获取当前变换模式
	TransformMode getCurrentTransformMode() const { return m_transformMode; }

	// 获取多边形填充学号颜色
	QColor getPolygonIdColor() const { return m_polygonIdColor; }

	// 设置多边形学号颜色
	void setPolygonIdColor(const QColor& color) { m_polygonIdColor = color; }

public slots:
	// 设置绘制模式
	void setDrawMode(DrawMode mode);

	// 设置变换模式
	void setTransformMode(TransformMode mode);

	// 设置图形颜色
	void setShapeColor();

	// 设置多边形颜色
	void setPolygonColor();

	// 清空画布
	void clear();

	// 重置立方体
	void resetCube();

signals:
	// 绘制模式改变信号
	void drawModeChanged(DrawMode newMode);

	// 变换模式改变信号
	void transformModeChanged(TransformMode newMode);

protected:
	// OpenGL初始化
	void initializeGL() override;

	// 窗口大小改变
	void resizeGL(int w, int h) override;

	// 绘制函数
	void paintGL() override;

	// 鼠标按下事件
	void mousePressEvent(QMouseEvent* event) override;

	// 鼠标移动事件
	void mouseMoveEvent(QMouseEvent* event) override;

private:
	// 屏幕坐标转像素坐标
	QVector2D mapToPixel(const QPoint& pos);

	// 绘制点集
	void drawPoints(const QVector<QVector2D>& points, const QColor& color);

	// 绘制单个点
	void drawSinglePoint(const QVector2D& point, const QColor& color);

	// 绘制虚线点集
	void drawDashedPoints(const QVector<QVector2D>& points, const QColor& color);

	// 保存当前状态到撤销栈
	void saveCurrentStateToUndoStack();

	// 绘制控制点
	void drawControlPoints();

	// 绘制预览
	void drawPreview();

	// 绘制矩形预览
	void drawRectanglePreview(const QVector2D& start, const QVector2D& end);

	// 绘制圆形预览
	void drawCirclePreview(const QVector2D& center, const QVector2D& edge);

	// 绘制多边形预览
	void drawPolygonPreview(const QVector2D& current);

	// 绘制Bezier预览
	void drawBezierPreview(const QVector2D& current);

	// 完成图形绘制
	void completeShapeDrawing(const QVector2D& endPoint);

	// 完成多边形绘制
	void completePolygonDrawing();

	// 完成Bezier绘制
	void completeBezierDrawing();

	// 获取当前图形分组
	ShapeGroup getCurrentShapeGroup() const;

	// 生成Bezier曲线
	QVector<QVector2D> generateBezierCurve(const QVector<QVector2D>& controlPoints);

	QOpenGLShaderProgram* m_program = nullptr;  // 着色器程序
	int m_vertexAttr = 0;                       // 顶点属性位置
	int m_colorAttr = 0;                        // 颜色属性位置
	QTimer m_timer;                             // 动画定时器

	DrawMode m_currentMode = DrawMode::None;    // 当前绘制模式
	TransformMode m_transformMode = TransformMode::None;  // 当前变换模式

	QColor m_currentShapeColor = Qt::black;     // 当前图形颜色
	QColor m_polygonBorderColor = Qt::blue;     // 多边形边界颜色
	QColor m_polygonIdColor = Qt::green;        // 多边形学号颜色

	QVector2D m_startPoint;                     // 绘制起始点
	bool m_isDrawingPreview = false;            // 是否正在预览绘制

	std::unique_ptr<ShapeManager> m_shapeManager;  // 图形管理器
	QVector<QVector2D> m_tempPolygonVertices;   // 多边形临时顶点

	// Bezier曲线控制点
	QVector<QVector2D> m_bezierControlPoints;   // Bezier控制点

	// 立方体变换状态
	Cube* m_cube = nullptr;                     // 立方体指针
	int m_translateStep = 10;                   // 平移步长
	int m_rotateAngle = 15;                     // 旋转角度

	// 撤销栈
	struct TransformState {
		QVector3D rotateAngles;                 // 旋转角度
		QVector3D translate;                    // 平移向量
	};
	QVector<TransformState> m_undoStack;        // 撤销状态栈
};