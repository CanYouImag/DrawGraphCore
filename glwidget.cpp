// glwidget.cpp
#include "glwidget.h"
#include "Line.h"
#include "Circle.h"
#include "Rectangle.h"
#include "Polygon.h"
#include <QMouseEvent>
#include <QColorDialog>
#include <QMessageBox>
#include <QApplication>

// 构造函数
GLWidget::GLWidget(QWidget* parent)
	: QOpenGLWidget(parent)
	, m_shapeManager(std::make_unique<ShapeManager>())
{
	connect(&m_timer, &QTimer::timeout, this, qOverload<>(&QWidget::update));
	m_timer.start(16);

	// 设置焦点策略，接收键盘事件
	setFocusPolicy(Qt::StrongFocus);
}

// 析构函数
GLWidget::~GLWidget()
{
	makeCurrent();
	if (m_program) {
		delete m_program;
		m_program = nullptr;
	}
	doneCurrent();
}

// OpenGL初始化
void GLWidget::initializeGL()
{
	initializeOpenGLFunctions();
	// 设置浅灰色背景
	glClearColor(0.9f, 0.9f, 0.9f, 1.0f);

	m_program = new QOpenGLShaderProgram(this);
	m_program->addShaderFromSourceCode(QOpenGLShader::Vertex,
		"attribute vec3 vertex;\n"
		"attribute vec3 color;\n"
		"varying vec3 fragColor;\n"
		"void main() {\n"
		"   fragColor = color;\n"
		"   gl_Position = vec4(vertex, 1.0);\n"
		"}\n"
	);

	m_program->addShaderFromSourceCode(QOpenGLShader::Fragment,
		"varying vec3 fragColor;\n"
		"void main() {\n"
		"   gl_FragColor = vec4(fragColor, 1.0);\n"
		"}\n"
	);

	m_program->link();
	m_vertexAttr = m_program->attributeLocation("vertex");
	m_colorAttr = m_program->attributeLocation("color");
}

// 窗口大小改变
void GLWidget::resizeGL(int w, int h)
{
	glViewport(0, 0, w, h);
}

// 绘制函数
// 在paintGL函数中修改立方体绘制逻辑
void GLWidget::paintGL()
{
	glClear(GL_COLOR_BUFFER_BIT);
	m_program->bind();

	glEnableVertexAttribArray(m_vertexAttr);
	glEnableVertexAttribArray(m_colorAttr);

	// 绘制所有已保存的图形（按分组）
	for (int group = 0; group < 4; ++group) {
		ShapeGroup shapeGroup = static_cast<ShapeGroup>(group);
		for (const auto& shape : m_shapeManager->getShapes(shapeGroup)) {
			auto pixels = shape->getPixels(width(), height());

			// 特殊处理立方体：需要按面绘制不同颜色
			Cube* cube = dynamic_cast<Cube*>(shape.get());
			if (cube) {
				// 立方体已经在getPixels中返回了带颜色的像素
				// 这里使用立方体的默认颜色绘制所有像素
				drawPoints(pixels, shape->color());
			}
			else {
				// 其他图形正常绘制
				Polygon* polygon = dynamic_cast<Polygon*>(shape.get());
				if (polygon && polygon->fillMode() != FillMode::None) {
					drawPoints(pixels, m_polygonIdColor);
				}
				else {
					drawPoints(pixels, shape->color());
				}
			}
		}
	}

	// 实时预览
	if (m_isDrawingPreview && m_currentMode != DrawMode::None) {
		drawPreview();
	}

	glDisableVertexAttribArray(m_vertexAttr);
	glDisableVertexAttribArray(m_colorAttr);
	m_program->release();
}

// 屏幕坐标转像素坐标
QVector2D GLWidget::mapToPixel(const QPoint& pos)
{
	return QVector2D(pos.x(), height() - pos.y());
}

// 绘制点集
void GLWidget::drawPoints(const QVector<QVector2D>& points, const QColor& color)
{
	GLfloat r = color.redF();
	GLfloat g = color.greenF();
	GLfloat b = color.blueF();

	for (const auto& p : points) {
		GLfloat vertexData[] = {
			p.x() / width() * 2.0f - 1.0f,
			p.y() / height() * 2.0f - 1.0f,
			0.0f,
			r, g, b
		};

		glVertexAttribPointer(m_vertexAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vertexData);
		glVertexAttribPointer(m_colorAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vertexData + 3);
		glDrawArrays(GL_POINTS, 0, 1);
	}
}

// 绘制单个点
void GLWidget::drawSinglePoint(const QVector2D& point, const QColor& color)
{
	GLfloat r = color.redF();
	GLfloat g = color.greenF();
	GLfloat b = color.blueF();

	GLfloat vertexData[] = {
		point.x() / width() * 2.0f - 1.0f,
		point.y() / height() * 2.0f - 1.0f,
		0.0f,
		r, g, b
	};

	glVertexAttribPointer(m_vertexAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vertexData);
	glVertexAttribPointer(m_colorAttr, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat), vertexData + 3);
	glDrawArrays(GL_POINTS, 0, 1);
}

// 绘制虚线点集
void GLWidget::drawDashedPoints(const QVector<QVector2D>& points, const QColor& color)
{
	// 每隔3个点绘制1个点，实现虚线效果
	for (int i = 0; i < points.size(); i += 3) {
		if (i < points.size()) {
			drawSinglePoint(points[i], color);
		}
	}
}

// 绘制预览
void GLWidget::drawPreview()
{
	QVector2D current = mapToPixel(QCursor::pos());

	switch (m_currentMode) {
	case DrawMode::Rectangle:
		drawRectanglePreview(m_startPoint, current);
		break;
	case DrawMode::Circle:
		drawCirclePreview(m_startPoint, current);
		break;
	case DrawMode::Polygon:
		drawPolygonPreview(current);
		break;
	case DrawMode::BezierCurve:
		drawBezierPreview(current);
		break;
	default:
		break;
	}
}

// 绘制矩形预览
void GLWidget::drawRectanglePreview(const QVector2D& start, const QVector2D& end)
{
	Rectangle temp(start, end, m_currentShapeColor);
	auto pixels = temp.getPixels(width(), height());
	drawDashedPoints(pixels, m_currentShapeColor);
}

// 绘制圆形预览
void GLWidget::drawCirclePreview(const QVector2D& center, const QVector2D& edge)
{
	float radius = (edge - center).length();
	Circle temp(center, static_cast<int>(radius), m_currentShapeColor);
	auto pixels = temp.getPixels(width(), height());
	drawDashedPoints(pixels, m_currentShapeColor);
}

// 绘制多边形预览
void GLWidget::drawPolygonPreview(const QVector2D& current)
{
	if (!m_tempPolygonVertices.isEmpty()) {
		// 绘制已添加的顶点连线
		for (int i = 0; i < m_tempPolygonVertices.size() - 1; ++i) {
			Line temp(m_tempPolygonVertices[i], m_tempPolygonVertices[i + 1], m_polygonBorderColor);
			auto pixels = temp.getPixels(width(), height());
			drawDashedPoints(pixels, m_polygonBorderColor);
		}
		// 绘制最后一个顶点到当前鼠标位置的连线
		Line temp(m_tempPolygonVertices.last(), current, m_polygonBorderColor);
		auto pixels = temp.getPixels(width(), height());
		drawDashedPoints(pixels, m_polygonBorderColor);
	}
}

// 绘制Bezier预览
void GLWidget::drawBezierPreview(const QVector2D& current)
{
	// 绘制控制点
	drawControlPoints();

	if (!m_bezierControlPoints.isEmpty()) {
		// 绘制控制多边形
		for (int i = 0; i < m_bezierControlPoints.size() - 1; ++i) {
			Line temp(m_bezierControlPoints[i], m_bezierControlPoints[i + 1], Qt::gray);
			auto pixels = temp.getPixels(width(), height());
			drawDashedPoints(pixels, Qt::gray);
		}

		// 绘制当前点到鼠标位置的控制线
		if (m_bezierControlPoints.size() < 4) {
			Line temp(m_bezierControlPoints.last(), current, Qt::gray);
			auto pixels = temp.getPixels(width(), height());
			drawDashedPoints(pixels, Qt::gray);
		}

		// 如果控制点达到4个，绘制Bezier曲线
		if (m_bezierControlPoints.size() == 4) {
			QVector<QVector2D> curvePoints = generateBezierCurve(m_bezierControlPoints);
			for (int i = 0; i < curvePoints.size() - 1; ++i) {
				Line segment(curvePoints[i], curvePoints[i + 1], m_currentShapeColor);
				auto pixels = segment.getPixels(width(), height());
				drawPoints(pixels, m_currentShapeColor);
			}
		}
	}
}

// 绘制控制点
void GLWidget::drawControlPoints()
{
	// 绘制Bezier控制点（小方块）
	for (const auto& point : m_bezierControlPoints) {
		// 绘制5x5的小方块表示控制点
		int x = static_cast<int>(point.x());
		int y = static_cast<int>(point.y());

		for (int dx = -2; dx <= 2; ++dx) {
			for (int dy = -2; dy <= 2; ++dy) {
				if (x + dx >= 0 && x + dx < width() && y + dy >= 0 && y + dy < height()) {
					drawSinglePoint(QVector2D(x + dx, y + dy), Qt::black);
				}
			}
		}
	}
}

// 鼠标按下事件处理
void GLWidget::mousePressEvent(QMouseEvent* event)
{
	if (event->button() != Qt::LeftButton && event->button() != Qt::RightButton) {
		return;
	}

	QVector2D point = mapToPixel(event->pos());

	switch (m_currentMode) {
	case DrawMode::Rectangle:
	case DrawMode::Circle:
		if (!m_isDrawingPreview) {
			m_startPoint = point;
			m_isDrawingPreview = true;
		}
		else {
			completeShapeDrawing(point);
			m_isDrawingPreview = false;
		}
		break;

	case DrawMode::Polygon:
		if (event->button() == Qt::LeftButton) {
			m_tempPolygonVertices.append(point);
			m_isDrawingPreview = true;
		}
		else if (event->button() == Qt::RightButton && m_tempPolygonVertices.size() >= 3) {
			completePolygonDrawing();
		}
		break;

	case DrawMode::BezierCurve:
		if (event->button() == Qt::LeftButton) {
			m_bezierControlPoints.append(point);
			m_isDrawingPreview = true;

			// 如果控制点达到4个，自动完成曲线
			if (m_bezierControlPoints.size() == 4) {
				completeBezierDrawing();
			}
		}
		else if (event->button() == Qt::RightButton) {
			// 右键取消当前Bezier曲线绘制
			m_bezierControlPoints.clear();
			m_isDrawingPreview = false;
		}
		break;

	case DrawMode::Cube:
		// 立方体不需要鼠标交互
		break;

	default:
		break;
	}

	update();
}

// 鼠标移动事件处理
void GLWidget::mouseMoveEvent(QMouseEvent* event)
{
	if (m_isDrawingPreview) {
		update();
	}
}

// 设置绘制模式
void GLWidget::setDrawMode(DrawMode mode)
{
	m_currentMode = mode;
	m_isDrawingPreview = false;
	m_startPoint = QVector2D();
	m_tempPolygonVertices.clear();
	m_bezierControlPoints.clear();

	if (mode == DrawMode::Cube && !m_cube) {
		m_cube = new Cube(QVector3D(0, 0, -5), 1.0f, Qt::white);
		m_shapeManager->addShape<Cube>(ShapeGroup::ThreeDTransform, *m_cube);
	}

	setCursor(mode == DrawMode::None ? Qt::ArrowCursor : Qt::CrossCursor);

	// 发出信号
	emit drawModeChanged(mode);
}

// 设置变换模式
void GLWidget::setTransformMode(TransformMode mode)
{
	m_transformMode = mode;

	// 发出信号
	emit transformModeChanged(mode);
}

// 设置图形颜色
void GLWidget::setShapeColor()
{
	QColor newColor = QColorDialog::getColor(m_currentShapeColor, this, "选择图形颜色");
	if (newColor.isValid()) {
		m_currentShapeColor = newColor;
	}
}

// 设置多边形颜色
void GLWidget::setPolygonColor()
{
	QColor newBorderColor = QColorDialog::getColor(m_polygonBorderColor, this, "选择边界颜色");
	if (newBorderColor.isValid()) {
		m_polygonBorderColor = newBorderColor;
	}

	QColor newIdColor = QColorDialog::getColor(m_polygonIdColor, this, "选择学号颜色");
	if (newIdColor.isValid()) {
		m_polygonIdColor = newIdColor;
	}
}

// 清空画布
void GLWidget::clear()
{
	m_shapeManager->clearAll();
	m_isDrawingPreview = false;
	m_startPoint = QVector2D();
	m_tempPolygonVertices.clear();
	m_bezierControlPoints.clear();
	m_cube = nullptr;
	update();
}

// 重置立方体
void GLWidget::resetCube()
{
	if (m_cube) {
		saveCurrentStateToUndoStack();
		m_cube->reset();
		update();
	}
}

// 完成图形绘制
void GLWidget::completeShapeDrawing(const QVector2D& endPoint)
{
	ShapeGroup group = getCurrentShapeGroup();

	switch (m_currentMode) {
	case DrawMode::Rectangle:
		m_shapeManager->addShape<Rectangle>(group, m_startPoint, endPoint, m_currentShapeColor);
		break;
	case DrawMode::Circle:
	{
		float radius = (endPoint - m_startPoint).length();
		m_shapeManager->addShape<Circle>(group, m_startPoint, static_cast<int>(radius), m_currentShapeColor);
	}
	break;
	}
}

// 完成多边形绘制
void GLWidget::completePolygonDrawing()
{
	if (m_tempPolygonVertices.size() >= 3) {
		m_shapeManager->addShape<Polygon>(ShapeGroup::AreaFill, m_tempPolygonVertices, m_polygonBorderColor, m_polygonIdColor);
		m_tempPolygonVertices.clear();
		m_isDrawingPreview = false;
	}
}

// 完成Bezier绘制
void GLWidget::completeBezierDrawing()
{
	if (m_bezierControlPoints.size() == 4) {
		// 生成Bezier曲线
		m_shapeManager->addShape<BezierCurve>(ShapeGroup::CurveDraw, m_bezierControlPoints, m_currentShapeColor);

		m_bezierControlPoints.clear();
		m_isDrawingPreview = false;
	}
}

// 获取当前图形分组
ShapeGroup GLWidget::getCurrentShapeGroup() const
{
	switch (m_currentMode) {
	case DrawMode::Rectangle:
	case DrawMode::Circle:
		return ShapeGroup::GraphicDraw;
	case DrawMode::Polygon:
		return ShapeGroup::AreaFill;
	case DrawMode::Cube:
		return ShapeGroup::ThreeDTransform;
	case DrawMode::BezierCurve:
		return ShapeGroup::CurveDraw;
	default:
		return ShapeGroup::GraphicDraw;
	}
}

// 生成Bezier曲线
QVector<QVector2D> GLWidget::generateBezierCurve(const QVector<QVector2D>& controlPoints)
{
	QVector<QVector2D> curvePoints;

	if (controlPoints.size() == 4) {
		// 3次Bezier曲线公式
		for (float t = 0; t <= 1.0f; t += 0.01f) {
			float u = 1.0f - t;
			float b0 = u * u * u;
			float b1 = 3 * u * u * t;
			float b2 = 3 * u * t * t;
			float b3 = t * t * t;

			float x = controlPoints[0].x() * b0 +
				controlPoints[1].x() * b1 +
				controlPoints[2].x() * b2 +
				controlPoints[3].x() * b3;

			float y = controlPoints[0].y() * b0 +
				controlPoints[1].y() * b1 +
				controlPoints[2].y() * b2 +
				controlPoints[3].y() * b3;

			curvePoints.append(QVector2D(x, y));
		}
	}

	return curvePoints;
}

// 处理键盘变换操作
void GLWidget::handleKeyTransform(bool positive, int multiplier)
{
	if (!m_cube) return;

	saveCurrentStateToUndoStack();

	float step = m_translateStep * 0.1f * multiplier;
	float angle = m_rotateAngle * multiplier;

	if (!positive) {
		step = -step;
		angle = -angle;
	}

	switch (m_transformMode) {
	case TransformMode::TranslateX:
		m_cube->translate(QVector3D(step, 0, 0));
		break;
	case TransformMode::TranslateY:
		m_cube->translate(QVector3D(0, step, 0));
		break;
	case TransformMode::TranslateZ:
		m_cube->translate(QVector3D(0, 0, step));
		break;
	case TransformMode::RotateX:
		m_cube->rotate(QVector3D(angle, 0, 0));
		break;
	case TransformMode::RotateY:
		m_cube->rotate(QVector3D(0, angle, 0));
		break;
	case TransformMode::RotateZ:
		m_cube->rotate(QVector3D(0, 0, angle));
		break;
	default:
		break;
	}

	update();
}

// 保存当前状态到撤销栈
void GLWidget::saveCurrentStateToUndoStack()
{
	if (m_cube) {
		TransformState state;
		state.rotateAngles = m_cube->getRotateAngles();
		state.translate = m_cube->getTranslate();
		m_undoStack.push_back(state);

		// 限制撤销栈大小
		if (m_undoStack.size() > 50) {
			m_undoStack.removeFirst();
		}
	}
}