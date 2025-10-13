// CGWORK1032.h
#pragma once
#include <QtWidgets/QMainWindow>
#include <QColor>
#include <QInputDialog>
#include <QKeyEvent>
#include <QLabel>
#include <QStatusBar>
#include <QToolBar>
#include <QActionGroup>
#include "ui_CGWORK1032.h"

class GLWidget;

// 主窗口类，负责用户界面和菜单交互
class CGWORK1032 : public QMainWindow
{
	Q_OBJECT

public:
	explicit CGWORK1032(QWidget* parent = nullptr);
	~CGWORK1032();

	void setTranslateStep(int step);
	void setRotateAngle(int angle);
	void keyPressEvent(QKeyEvent* event) override;

private slots:
	// 图形绘制菜单
	void on_actionDrawRectangle_triggered();
	void on_actionDrawCircle_triggered();
	void on_actionSetColor_triggered();

	// 区域填充菜单  
	void on_actionDrawPolygon_triggered();
	void on_actionSetColorPolygon_triggered();

	// 三维变换菜单
	void on_actionDrawCube_triggered();
	void on_actionTranslateX_triggered();
	void on_actionTranslateY_triggered();
	void on_actionTranslateZ_triggered();
	void on_actionRotateX_triggered();
	void on_actionRotateY_triggered();
	void on_actionRotateZ_triggered();
	void on_actionSetData_triggered();

	// 绘制曲线菜单
	void on_actionDrawBezier_triggered();

	// 工具菜单
	void on_actionClear_triggered();
	void on_actionSave_triggered();
	void on_actionLoad_triggered();

	// 帮助菜单
	void on_actionAbout_triggered();
	void on_actionHelp_triggered();

	// 更新状态栏
	void updateStatusBar();

private:
	void setupMenu();           // 设置菜单
	void setupToolBar();        // 设置工具栏
	void setupStatusBar();      // 设置状态栏
	void setupActionGroups();   // 设置动作分组

	Ui::CGWORK1032Class ui;
	QColor m_currentShapeColor;
	QColor m_currentPolygonColor;
	QColor m_currentPolygonFillColor;
	int m_translateStep;
	int m_rotateAngle;
	GLWidget* m_glWidget;

	// UI 控件
	QLabel* m_statusLabel;
	QToolBar* m_drawToolBar;
	QActionGroup* m_drawActionGroup;
	QActionGroup* m_transformActionGroup;
};