#include "UIRealTimeEditorTransform.h"

// Constructor

s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform()
{
    this->m_sprite_repository = nullptr;
    this->m_positionChanger = s2d::UIRealTimeEditorTransformPosition();
}

s2d::UIRealTimeEditorTransform::UIRealTimeEditorTransform(s2d::InspectorState* ptr_Inspectorstate,
    bool* isAnyUIWindowHovered, s2d::Event* windowEvent, s2d::SpriteRepository& repo, s2d::GUIRepository& gui_repo)
{
    this->m_scaleChanger = s2d::UIRealTimeEditorTransfsormScale(windowEvent, repo, gui_repo);
    this->m_positionChanger = s2d::UIRealTimeEditorTransformPosition(ptr_Inspectorstate, isAnyUIWindowHovered, windowEvent, repo);
    this->m_sprite_repository = &repo;
}


// Public functions

void s2d::UIRealTimeEditorTransform::update()
{
    
    if(this->m_sprite_repository->current_tool == s2d::EditorTools::PositionTool)
        this->m_positionChanger.update(); 

    if (this->m_sprite_repository->current_tool == s2d::EditorTools::ScaleTool
        && this->m_sprite_repository->sprite_in_inspector != nullptr)
    {
        this->m_scaleChanger.renderDolls();
        this->m_scaleChanger.update();
    }
    else
    {
        this->m_scaleChanger.unrenderDolls();
    }
}