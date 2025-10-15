// ShapeManager.h
#pragma once
#include <vector>
#include <memory>
#include <unordered_map>
#include "Shape.h"
#include "BezierCurve.h"

// 图形分组枚举
enum class ShapeGroup {
	GraphicDraw,    // 图形绘制
	AreaFill,       // 区域填充  
	ThreeDTransform,// 三维变换
	CurveDraw       // 绘制曲线
};

class Cube;

// 图形管理器类：负责管理和维护所有图形对象
class ShapeManager
{
public:
	// 添加任意 Shape 派生类的图形
	// 参数: group - 图形分组, args - 图形构造参数
	template<typename T, typename... Args>
	void addShape(ShapeGroup group, Args&&... args) {
		m_shapes[group].emplace_back(std::make_unique<T>(std::forward<Args>(args)...));
	}

	// 移除指定分组的最后一个图形
	void removeLastShape(ShapeGroup group);

	// 清空指定分组的所有图形
	void clearGroup(ShapeGroup group);

	// 清空所有图形
	void clearAll();

	// 获取立方体对象指针
	Cube* getCube();

	// 获取指定分组的所有图形的只读引用
	const std::vector<std::unique_ptr<Shape>>& getShapes(ShapeGroup group) const;

private:
	std::unordered_map<ShapeGroup, std::vector<std::unique_ptr<Shape>>> m_shapes;
};