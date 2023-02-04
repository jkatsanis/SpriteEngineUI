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
    if (this->m_currentTool == nullptr)
    {
        return;
    }
    
    if(*this->m_currentTool == s2d::EditorTools::PositionTool)
        this->m_positionChanger.update();

    if (*this->m_currentTool == s2d::EditorTools::ScaleTool)
        this->m_scaleChanger.update();
}