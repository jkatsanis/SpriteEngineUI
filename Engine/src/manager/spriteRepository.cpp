#include "spriteRepository.h"
#include <physicalComponents/sprite.h>

// Constructor / Destructor

s2d::SpriteRepository::SpriteRepository()
{
    this->asset_folder_data.darg_and_drop_name = " ";
    this->asset_folder_data.drag_and_drop_path = " ";
    this->current_tool = s2d::EditorTools::PositionTool;
    this->sprited_hovered_in_hierarchy = nullptr;
    this->sprite_in_inspector = nullptr;
    this->child_to_parent = nullptr;
    this->right_clicked_sprite = nullptr;
    this->dupeNameCounter = 0;
    this->highest_sprite_id = 0;
    this->m_highestLayerIndex = 0;
    this->initialied = false;
}

s2d::SpriteRepository::~SpriteRepository()
{
    this->cleanUp();
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
    this->highest_sprite_id++;
    ptr->validateProperties(this->highest_sprite_id, *this);
    this->m_sprites.push_back(ptr);

    if (this->initialied)
    {
        for (size_t i = 0; i < ptr->ptr_childs.size(); i++)
        {
            this->add(ptr->ptr_childs[i]);
        }
    }
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
    return s2d::SpriteRepository::getWithId(this->m_sprites, id);
}

void s2d::SpriteRepository::instanitatePrefab(const std::string& path_to)
{
    s2d::Initializer::initPrefab(path_to, *this);
}

void s2d::SpriteRepository::addTag(const std::string& tag)
{
    if (!std::isEqualWithAny(tag, this->m_tags))
    {
        this->m_tags.push_back(tag);
    }
}

void s2d::SpriteRepository::cleanUp()
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        this->deleteAt(i);
    }
    this->child_to_parent = nullptr;
    this->sprite_in_inspector = nullptr;
    this->right_clicked_sprite = nullptr;

    if (this->m_sprites.size() > 0)
    {
        delete this->m_sprites[0];
    }
    this->m_sprites.clear();
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

void s2d::SpriteRepository::reloadTextures()
{
    for (size_t i = 0; i < this->m_sprites.size(); i++)
    {
        this->m_sprites[i]->setSpriteTexture(this->m_sprites[i]->sprite_renderer.path);
        for (auto& animation : this->m_sprites[i]->animator.animations)
        {
            animation.second.realoadTextures();
        }
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

// Static functions

void s2d::SpriteRepository::getAllChilds(std::vector<const s2d::Sprite*>& childs, const s2d::Sprite* parent)
{
    for (size_t i = 0; i < parent->ptr_childs.size(); i++)
    {
        childs.push_back(parent->ptr_childs[i]);
        getAllChilds(childs, parent->ptr_childs[i]);
    }
}

s2d::Sprite* s2d::SpriteRepository::getWithId(std::vector<s2d::Sprite*>& collection, uint32_t id)
{
    for (uint32_t i = 0; i < collection.size(); i++)
    {
        s2d::Sprite* const sprite = collection[i];
        if (sprite->getId() == id)
        {
            return sprite;
        }
    }
    return nullptr;
}
