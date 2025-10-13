// ShapeManager.cpp
#include "ShapeManager.h"
#include "Cube.h"
#include <memory>

// 移除指定分组的最后一个图形
void ShapeManager::removeLastShape(ShapeGroup group) {
	auto& groupShapes = m_shapes[group];
	if (!groupShapes.empty()) {
		groupShapes.pop_back();
	}
}

// 清空指定分组的所有图形
void ShapeManager::clearGroup(ShapeGroup group) {
	m_shapes[group].clear();
}

// 清空所有图形
void ShapeManager::clearAll() {
	m_shapes.clear();
}

// 获取立方体对象指针
Cube* ShapeManager::getCube() {
	auto& transformShapes = m_shapes[ShapeGroup::ThreeDTransform];
	for (auto& shape : transformShapes) {
		Cube* cube = dynamic_cast<Cube*>(shape.get());
		if (cube) return cube;
	}
	return nullptr;
}

// 获取指定分组的所有图形的只读引用
const std::vector<std::unique_ptr<Shape>>& ShapeManager::getShapes(ShapeGroup group) const {
	static const std::vector<std::unique_ptr<Shape>> empty;
	auto it = m_shapes.find(group);
	return it != m_shapes.end() ? it->second : empty;
}