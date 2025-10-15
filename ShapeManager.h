// ShapeManager.h
#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "Shape.h"
#include "BezierCurve.h"

// ͼ�η���ö��
enum class ShapeGroup {
	GraphicDraw,    // ͼ�λ���
	AreaFill,       // �������  
	ThreeDTransform,// ��ά�任
	CurveDraw       // ��������
};

class Cube;

// ͼ�ι������ࣺ��������ά������ͼ�ζ���
class ShapeManager
{
public:
	// ������� Shape �������ͼ��
	// ����: group - ͼ�η���, args - ͼ�ι������
	template<typename T, typename... Args>
	void addShape(ShapeGroup group, Args&&... args) {
		m_shapes[group].emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	}

	// �Ƴ�ָ����������һ��ͼ��
	void removeLastShape(ShapeGroup group);

	// ���ָ�����������ͼ��
	void clearGroup(ShapeGroup group);

	// �������ͼ��
	void clearAll();

	// ��ȡ���������ָ��
	Cube* getCube();

	// ��ȡָ�����������ͼ�ε�ֻ������
	const std::vector<std::unique_ptr<Shape>>& getShapes(ShapeGroup group) const;

private:
	std::unordered_map<ShapeGroup, std::vector<std::unique_ptr<Shape>>> m_shapes;
};