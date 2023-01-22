#include "UIRealTimeEditorTransform.h"

// Constructor


s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform()
{
    this->m_positionChanger = s2d::UIRealTimeEditorTransformPosition();
}

s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform(sf::RenderWindow* window, s2d::InspectorState* ptr_Inspectorstate, bool* isAnyUIWindowHovered, s2d::Event* windowEvent)
{
    this->m_positionChanger = s2d::UIRealTimeEditorTransformPosition(window, ptr_Inspectorstate, isAnyUIWindowHovered, windowEvent);
}

// Public functions

void s2d::UIRealTimeEditorTransform::update()
{
    this->m_positionChanger.update();
}