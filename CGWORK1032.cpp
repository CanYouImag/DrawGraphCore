// CGWORK1032.cpp
#include "CGWORK1032.h"
#include "glwidget.h"
#include <QColorDialog>
#include <QInputDialog>
#include <QMessageBox>
#include <QFileDialog>
#include <QToolBar>
#include <QActionGroup>
#include <QLabel>

// 构造函数：初始化界面和默认参数
CGWORK1032::CGWORK1032(QWidget* parent)
	: QMainWindow(parent)
	, ui()
	, m_currentShapeColor(Qt::black)
	, m_currentPolygonColor(Qt::blue)
	, m_currentPolygonFillColor(Qt::green)
	, m_translateStep(10)
	, m_rotateAngle(15)
	, m_glWidget(nullptr)
	, m_statusLabel(nullptr)
	, m_drawToolBar(nullptr)
	, m_drawActionGroup(nullptr)
	, m_transformActionGroup(nullptr)
{
	ui.setupUi(this);

	// 设置窗口属性
	setWindowTitle("CGWORK1032 - 计算机图形学应用程序");
	setMinimumSize(800, 600);

	// 初始化UI组件
	setupMenu();
	setupToolBar();
	setupStatusBar();
	setupActionGroups();

	// 查找GLWidget
	QWidget* middleContainer = findChild<QWidget*>(); // 找到中间容器
	if (middleContainer) {
		m_glWidget = middleContainer->findChild<GLWidget*>("glWidget");
	}
	if (!m_glWidget)
	{
		QMessageBox::warning(this, "错误", "找不到名为\"glWidget\"的控件！");
		return;
	}

	m_glWidget->setDrawMode(DrawMode::None);
	m_glWidget->setTranslateStep(m_translateStep);
	m_glWidget->setRotateAngle(m_rotateAngle);

	// 连接信号更新状态栏
	connect(m_glWidget, &GLWidget::drawModeChanged, this, &CGWORK1032::updateStatusBar);
	connect(m_glWidget, &GLWidget::transformModeChanged, this, &CGWORK1032::updateStatusBar);

	// 初始状态栏更新
	updateStatusBar();
}

// 析构函数
CGWORK1032::~CGWORK1032() = default;

// 设置菜单
void CGWORK1032::setupMenu()
{
	// 菜单已经在.ui文件中设置
}

// 设置工具栏
void CGWORK1032::setupToolBar()
{
	m_drawToolBar = addToolBar("绘图工具");
	m_drawToolBar->setIconSize(QSize(24, 24));

	// 安全地添加绘图工具到工具栏（检查动作是否存在）
	if (ui.actionDrawRectangle) {
		m_drawToolBar->addAction(ui.actionDrawRectangle);
	}
	if (ui.actionDrawCircle) {
		m_drawToolBar->addAction(ui.actionDrawCircle);
	}
	if (ui.actionDrawPolygon) {
		m_drawToolBar->addAction(ui.actionDrawPolygon);
	}
	if (ui.actionDrawCube) {
		m_drawToolBar->addAction(ui.actionDrawCube);
	}
	if (ui.actionDrawBezier) {
		m_drawToolBar->addAction(ui.actionDrawBezier);
	}

	m_drawToolBar->addSeparator();

	// 安全地添加变换工具
	if (ui.actionTranslateX) {
		m_drawToolBar->addAction(ui.actionTranslateX);
	}
	if (ui.actionTranslateY) {
		m_drawToolBar->addAction(ui.actionTranslateY);
	}
	if (ui.actionTranslateZ) {
		m_drawToolBar->addAction(ui.actionTranslateZ);
	}
	if (ui.actionRotateX) {
		m_drawToolBar->addAction(ui.actionRotateX);
	}
	if (ui.actionRotateY) {
		m_drawToolBar->addAction(ui.actionRotateY);
	}
	if (ui.actionRotateZ) {
		m_drawToolBar->addAction(ui.actionRotateZ);
	}

	m_drawToolBar->addSeparator();

	// 安全地添加工具按钮
	if (ui.actionSetColor) {
		m_drawToolBar->addAction(ui.actionSetColor);
	}
	if (ui.actionSetData) {
		m_drawToolBar->addAction(ui.actionSetData);
	}
	if (ui.actionClear) {
		m_drawToolBar->addAction(ui.actionClear);
	}
}

// 设置状态栏
void CGWORK1032::setupStatusBar()
{
	m_statusLabel = new QLabel("就绪");
	statusBar()->addWidget(m_statusLabel);

	// 添加永久状态信息
	QLabel* infoLabel = new QLabel("使用 A/L 键进行变换 | R键重置");
	statusBar()->addPermanentWidget(infoLabel);
}

// 设置动作分组
void CGWORK1032::setupActionGroups()
{
	// 绘图动作分组（互斥）
	m_drawActionGroup = new QActionGroup(this);
	if (ui.actionDrawRectangle) {
		m_drawActionGroup->addAction(ui.actionDrawRectangle);
	}
	if (ui.actionDrawCircle) {
		m_drawActionGroup->addAction(ui.actionDrawCircle);
	}
	if (ui.actionDrawPolygon) {
		m_drawActionGroup->addAction(ui.actionDrawPolygon);
	}
	if (ui.actionDrawCube) {
		m_drawActionGroup->addAction(ui.actionDrawCube);
	}
	if (ui.actionDrawBezier) {
		m_drawActionGroup->addAction(ui.actionDrawBezier);
	}
	m_drawActionGroup->setExclusive(true);

	// 变换动作分组（互斥）
	m_transformActionGroup = new QActionGroup(this);
	if (ui.actionTranslateX) {
		m_transformActionGroup->addAction(ui.actionTranslateX);
	}
	if (ui.actionTranslateY) {
		m_transformActionGroup->addAction(ui.actionTranslateY);
	}
	if (ui.actionTranslateZ) {
		m_transformActionGroup->addAction(ui.actionTranslateZ);
	}
	if (ui.actionRotateX) {
		m_transformActionGroup->addAction(ui.actionRotateX);
	}
	if (ui.actionRotateY) {
		m_transformActionGroup->addAction(ui.actionRotateY);
	}
	if (ui.actionRotateZ) {
		m_transformActionGroup->addAction(ui.actionRotateZ);
	}
	m_transformActionGroup->setExclusive(true);
}

// 更新状态栏
void CGWORK1032::updateStatusBar()
{
	if (!m_glWidget) return;

	QString status;
	switch (m_glWidget->getCurrentDrawMode()) {
	case DrawMode::None:
		status = "就绪 - 选择绘图模式开始";
		break;
	case DrawMode::Rectangle:
		status = "矩形绘制模式 - 点击并拖动绘制矩形";
		break;
	case DrawMode::Circle:
		status = "圆形绘制模式 - 点击并拖动绘制圆形";
		break;
	case DrawMode::Polygon:
		status = "多边形绘制模式 - 左键添加顶点，右键完成绘制";
		break;
	case DrawMode::Cube:
		status = "立方体模式 - 使用变换工具操作立方体";
		break;
	case DrawMode::BezierCurve:
		status = "Bezier曲线模式 - 点击添加控制点";
		break;
	default:
		status = "就绪";
		break;
	}

	// 添加变换模式信息
	switch (m_glWidget->getCurrentTransformMode()) {
	case TransformMode::TranslateX:
		status += " | X轴平移模式 - 使用A/L键平移";
		break;
	case TransformMode::TranslateY:
		status += " | Y轴平移模式 - 使用A/L键平移";
		break;
	case TransformMode::TranslateZ:
		status += " | Z轴平移模式 - 使用A/L键平移";
		break;
	case TransformMode::RotateX:
		status += " | X轴旋转模式 - 使用A/L键旋转";
		break;
	case TransformMode::RotateY:
		status += " | Y轴旋转模式 - 使用A/L键旋转";
		break;
	case TransformMode::RotateZ:
		status += " | Z轴旋转模式 - 使用A/L键旋转";
		break;
	default:
		break;
	}

	m_statusLabel->setText(status);
}

// 设置平移步长
void CGWORK1032::setTranslateStep(int step)
{
	m_translateStep = step;
	if (m_glWidget) {
		m_glWidget->setTranslateStep(step);
	}
	updateStatusBar();
}

// 设置旋转角度
void CGWORK1032::setRotateAngle(int angle)
{
	m_rotateAngle = angle;
	if (m_glWidget) {
		m_glWidget->setRotateAngle(angle);
	}
	updateStatusBar();
}

// 绘制矩形菜单项触发
void CGWORK1032::on_actionDrawRectangle_triggered()
{
	if (m_glWidget) {
		m_glWidget->setDrawMode(DrawMode::Rectangle);
		updateStatusBar();
	}
}

// 绘制圆形菜单项触发
void CGWORK1032::on_actionDrawCircle_triggered()
{
	if (m_glWidget) {
		m_glWidget->setDrawMode(DrawMode::Circle);
		updateStatusBar();
	}
}

// 设置图形颜色菜单项触发
void CGWORK1032::on_actionSetColor_triggered()
{
	if (m_glWidget)
	{
		m_glWidget->setShapeColor();
		updateStatusBar();
	}
}

// 绘制多边形菜单项触发
void CGWORK1032::on_actionDrawPolygon_triggered()
{
	if (m_glWidget) {
		m_glWidget->setDrawMode(DrawMode::Polygon);
		updateStatusBar();
	}
}

// 设置多边形颜色菜单项触发
void CGWORK1032::on_actionSetColorPolygon_triggered()
{
	if (m_glWidget) {
		m_glWidget->setPolygonColor();
		updateStatusBar();
	}
}

// 绘制立方体菜单项触发
void CGWORK1032::on_actionDrawCube_triggered()
{
	if (m_glWidget)
	{
		m_glWidget->setDrawMode(DrawMode::Cube);
		m_glWidget->resetCube();
		updateStatusBar();
	}
}

// X轴平移菜单项触发
void CGWORK1032::on_actionTranslateX_triggered()
{
	if (m_glWidget) {
		m_glWidget->setTransformMode(TransformMode::TranslateX);
		updateStatusBar();
	}
}

// Y轴平移菜单项触发
void CGWORK1032::on_actionTranslateY_triggered()
{
	if (m_glWidget) {
		m_glWidget->setTransformMode(TransformMode::TranslateY);
		updateStatusBar();
	}
}

// Z轴平移菜单项触发
void CGWORK1032::on_actionTranslateZ_triggered()
{
	if (m_glWidget) {
		m_glWidget->setTransformMode(TransformMode::TranslateZ);
		updateStatusBar();
	}
}

// 绕X轴旋转菜单项触发
void CGWORK1032::on_actionRotateX_triggered()
{
	if (m_glWidget) {
		m_glWidget->setTransformMode(TransformMode::RotateX);
		updateStatusBar();
	}
}

// 绕Y轴旋转菜单项触发
void CGWORK1032::on_actionRotateY_triggered()
{
	if (m_glWidget) {
		m_glWidget->setTransformMode(TransformMode::RotateY);
		updateStatusBar();
	}
}

// 绕Z轴旋转菜单项触发
void CGWORK1032::on_actionRotateZ_triggered()
{
	if (m_glWidget) {
		m_glWidget->setTransformMode(TransformMode::RotateZ);
		updateStatusBar();
	}
}

// 设置变换参数菜单项触发
void CGWORK1032::on_actionSetData_triggered()
{
	bool ok;

	// 平移步长设置
	int step = QInputDialog::getInt(this, "设置平移步长",
		"平移步长:", m_translateStep, 1, 100, 1, &ok);
	if (ok) {
		setTranslateStep(step);
	}

	// 旋转角度设置
	int angle = QInputDialog::getInt(this, "设置旋转角度",
		"旋转角度:", m_rotateAngle, 1, 360, 1, &ok);
	if (ok) {
		setRotateAngle(angle);
	}

	updateStatusBar();
}

// 绘制Bezier曲线菜单项触发
void CGWORK1032::on_actionDrawBezier_triggered()
{
	if (m_glWidget) {
		m_glWidget->setDrawMode(DrawMode::BezierCurve);
		updateStatusBar();
	}
}

// 清空画布菜单项触发
void CGWORK1032::on_actionClear_triggered()
{
	if (m_glWidget) {
		m_glWidget->clear();
		updateStatusBar();
	}
}

// 保存菜单项触发
void CGWORK1032::on_actionSave_triggered()
{
	QString fileName = QFileDialog::getSaveFileName(this,
		"保存图形", "", "图形文件 (*.cg)");
	if (!fileName.isEmpty()) {
		QMessageBox::information(this, "保存", "保存功能待实现");
	}
}

// 加载菜单项触发
void CGWORK1032::on_actionLoad_triggered()
{
	QString fileName = QFileDialog::getOpenFileName(this,
		"加载图形", "", "图形文件 (*.cg)");
	if (!fileName.isEmpty()) {
		QMessageBox::information(this, "加载", "加载功能待实现");
	}
}

// 关于菜单项触发
void CGWORK1032::on_actionAbout_triggered()
{
	QMessageBox::about(this, "关于 CGWORK1032",
		"<h3>计算机图形学应用程序</h3>"
		"<p>版本 1.0</p>"
		"<p>学号: 1032</p>"
		"<p>功能：</p>"
		"<ul>"
		"<li>基本图形绘制（矩形、圆形）</li>"
		"<li>多边形区域填充</li>"
		"<li>三维立方体变换</li>"
		"<li>Bezier曲线绘制</li>"
		"</ul>");
}

// 帮助菜单项触发
void CGWORK1032::on_actionHelp_triggered()
{
	QMessageBox::information(this, "帮助",
		"<h3>使用说明</h3>"
		"<p><b>基本绘制：</b></p>"
		"<ul>"
		"<li>选择绘图模式后，在画布上点击拖动绘制</li>"
		"<li>多边形：左键添加顶点，右键完成绘制</li>"
		"</ul>"
		"<p><b>三维变换：</b></p>"
		"<ul>"
		"<li>选择变换模式（平移/旋转）</li>"
		"<li>使用 A 键正向变换，L 键反向变换</li>"
		"<li>Shift + A/L 加速变换</li>"
		"<li>R 键重置立方体</li>"
		"</ul>"
		"<p><b>其他：</b></p>"
		"<ul>"
		"<li>清空：清除所有图形</li>"
		"<li>设置数据：调整平移步长和旋转角度</li>"
		"</ul>");
}

// 键盘事件处理：处理变换操作
void CGWORK1032::keyPressEvent(QKeyEvent* event)
{
	if (!m_glWidget) {
		QMainWindow::keyPressEvent(event);
		return;
	}

	// 处理修饰键状态
	Qt::KeyboardModifiers modifiers = QApplication::keyboardModifiers();
	bool shiftPressed = modifiers & Qt::ShiftModifier;

	// 组合键：Shift+按键加速
	int multiplier = shiftPressed ? 5 : 1;

	switch (event->key()) {
		// 根据题目要求使用 A 和 L 键
	case Qt::Key_A:  // 正向
		m_glWidget->handleKeyTransform(true, multiplier);
		updateStatusBar();
		break;
	case Qt::Key_L:  // 负向
		m_glWidget->handleKeyTransform(false, multiplier);
		updateStatusBar();
		break;

		// 重置功能
	case Qt::Key_R:
		m_glWidget->resetCube();
		updateStatusBar();
		break;

	default:
		QMainWindow::keyPressEvent(event);
		return;
	}

	event->accept();
}