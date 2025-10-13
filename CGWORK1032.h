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

// �������࣬�����û�����Ͳ˵�����
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
	// ͼ�λ��Ʋ˵�
	void on_actionDrawRectangle_triggered();
	void on_actionDrawCircle_triggered();
	void on_actionSetColor_triggered();

	// �������˵�  
	void on_actionDrawPolygon_triggered();
	void on_actionSetColorPolygon_triggered();

	// ��ά�任�˵�
	void on_actionDrawCube_triggered();
	void on_actionTranslateX_triggered();
	void on_actionTranslateY_triggered();
	void on_actionTranslateZ_triggered();
	void on_actionRotateX_triggered();
	void on_actionRotateY_triggered();
	void on_actionRotateZ_triggered();
	void on_actionSetData_triggered();

	// �������߲˵�
	void on_actionDrawBezier_triggered();

	// ���߲˵�
	void on_actionClear_triggered();
	void on_actionSave_triggered();
	void on_actionLoad_triggered();

	// �����˵�
	void on_actionAbout_triggered();
	void on_actionHelp_triggered();

	// ����״̬��
	void updateStatusBar();

private:
	void setupMenu();           // ���ò˵�
	void setupToolBar();        // ���ù�����
	void setupStatusBar();      // ����״̬��
	void setupActionGroups();   // ���ö�������

	Ui::CGWORK1032Class ui;
	QColor m_currentShapeColor;
	QColor m_currentPolygonColor;
	QColor m_currentPolygonFillColor;
	int m_translateStep;
	int m_rotateAngle;
	GLWidget* m_glWidget;

	// UI �ؼ�
	QLabel* m_statusLabel;
	QToolBar* m_drawToolBar;
	QActionGroup* m_drawActionGroup;
	QActionGroup* m_transformActionGroup;
};