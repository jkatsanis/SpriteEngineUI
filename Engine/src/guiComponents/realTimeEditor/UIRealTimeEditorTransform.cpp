#include "UIRealTimeEditorTransform.h"

// Constructor


s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform()
{
    this->m_currentTool = nullptr;
    this->m_positionChanger = s2d::UIRealTimeEditorTransformPosition();
}

s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate,
    bool* isAnyUIWindowHovered, s2d::Event* windowEvent, const s2d::EditorTools* tool)
{
    this->m_currentTool = tool;
    this->m_positionChanger = s2d::UIRealTimeEditorTransformPosition(window, ptr_Inspectorstate, isAnyUIWindowHovered, windowEvent);
}

// Public functions

void s2d::UIRealTimeEditorTransform::update()
{
    std::cout << ((int)*this->m_currentTool) << std::endl;;
    this->m_positionChanger.update();
}