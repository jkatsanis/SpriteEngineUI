#include "spriteRepository.h"
#include <physicalComponents/sprite.h>

// Constructor / Destructor

s2d::SpriteRepository::SpriteRepository()
{
    this->asset_folder_data.dragAndDropName = " ";
    this->asset_folder_data.drag_and_drop_path = " ";
    this->current_tool = s2d::EditorTools::PositionTool;
    this->sprited_hovered_in_hierarchy = nullptr;
    this->sprite_in_inspector = nullptr;
    this->child_to_parent = nullptr;
    this->right_clicked_sprite = nullptr;
    this->dupeNameCounter = 0;
    this->highestSpriteId = 0;
    this->m_highestLayerIndex = 0;
}

s2d::SpriteRepository::~SpriteRepository()
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        delete this->m_sprites[i];
        this->m_sprites[i] = nullptr;
    }
}

// Public functions

s2d::Sprite* const s2d::SpriteRepository::readAt(size_t idx)
{
    if (idx >= this->m_sprites.size())
    {
        std::cout << "LOG [ERROR] Cant read at a invalid position";
        return nullptr;
    }
    s2d::Sprite* const ptr = this->m_sprites[idx];
    return ptr;
}

const s2d::Sprite* const s2d::SpriteRepository::readAt(size_t idx, bool constConst) const
{
    const s2d::Sprite* ptr = this->m_sprites[idx];
    return ptr;
}

void s2d::SpriteRepository::deleteAt(size_t idx)
{
    std::vector<s2d::Sprite*> childs_to_delete = std::vector<s2d::Sprite*>(0);
    s2d::Sprite* ptr_delete = this->m_sprites[idx];
    this->addChildsToDelete(childs_to_delete, ptr_delete);
    this->m_sprites.erase(this->m_sprites.begin() + idx);

    for (int i = 0; i < childs_to_delete.size(); i++)
    {
        this->eraseWithId(childs_to_delete[i]->getId());
    }
    for (int i = 0; i < childs_to_delete.size(); i++)
    {
        delete childs_to_delete[i];
        childs_to_delete[i] = nullptr;
    }

    // Destructor call
    delete ptr_delete;
    ptr_delete = nullptr;
}

void s2d::SpriteRepository::deleteWithName(const std::string& name)
{
    this->deleteAt(getIndexAtName(name));
}

void s2d::SpriteRepository::add(s2d::Sprite* ptr)
{
    // Validate properties
    // Set id
    this->highestSpriteId++;
    ptr->validateProperties(this->highestSpriteId, *this);
    this->m_sprites.push_back(ptr);
}

s2d::Sprite* s2d::SpriteRepository::getSpriteWithName(const std::string& name)
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        s2d::Sprite* sprite = this->m_sprites[i];
        if (sprite->name == name)
        {
            return sprite;
        }
    }

    return nullptr;
}

s2d::Sprite* s2d::SpriteRepository::getSpriteWithId(int id)
{
    for (int i = 0; i < this->m_sprites.size(); i++)
    {
        s2d::Sprite* const sprite = this->m_sprites[i];
        if (sprite->getId() == id)
        {
            return sprite;
        }
    }
    return nullptr;
}

void s2d::SpriteRepository::updateHighestLayerIndex()
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        s2d::Sprite* const sprite = this->m_sprites[i];
        if (sprite->sprite_renderer.sorting_layer_index > m_highestLayerIndex)
            m_highestLayerIndex = sprite->sprite_renderer.sorting_layer_index;
    }
}

// Private functions

size_t s2d::SpriteRepository::getIndexAtName(const std::string& name) const
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        const s2d::Sprite* sprite = this->m_sprites[i];
        if (sprite->name == name)
        {
            return i;
        }
    }

    return -1;
}

void s2d::SpriteRepository::addChildsToDelete(std::vector<s2d::Sprite*>& ids, s2d::Sprite* parent)
{
    for (int i = 0; i < parent->ptr_childs.size(); i++)
    {
        ids.push_back(parent->ptr_childs[i]);
        addChildsToDelete(ids, parent->ptr_childs[i]);
    }
}

void s2d::SpriteRepository::eraseWithId(uint8_t id)
{
    for (int i = 0; i < this->amount(); i++)
    {
        if (this->m_sprites[i]->getId() == id)
        {
            this->m_sprites.erase(this->m_sprites.begin() + i);
            return;
        }
    }
}
