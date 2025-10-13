// ShapeManager.cpp
#include "ShapeManager.h"
#include "Cube.h"
#include <memory>

// �Ƴ�ָ����������һ��ͼ��
void ShapeManager::removeLastShape(ShapeGroup group) {
	auto& groupShapes = m_shapes[group];
	if (!groupShapes.empty()) {
		groupShapes.pop_back();
	}
}

// ���ָ�����������ͼ��
void ShapeManager::clearGroup(ShapeGroup group) {
	m_shapes[group].clear();
}

// �������ͼ��
void ShapeManager::clearAll() {
	m_shapes.clear();
}

// ��ȡ���������ָ��
Cube* ShapeManager::getCube() {
	auto& transformShapes = m_shapes[ShapeGroup::ThreeDTransform];
	for (auto& shape : transformShapes) {
		Cube* cube = dynamic_cast<Cube*>(shape.get());
		if (cube) return cube;
	}
	return nullptr;
}

// ��ȡָ�����������ͼ�ε�ֻ������
const std::vector<std::unique_ptr<Shape>>& ShapeManager::getShapes(ShapeGroup group) const {
	static const std::vector<std::unique_ptr<Shape>> empty;
	auto it = m_shapes.find(group);
	return it != m_shapes.end() ? it->second : empty;
}