#include "SpriteRepository.h"
#include "Sprite.h"

// Constructor / Destructor

spe::SpriteRepository::SpriteRepository()
{
    this->m_HighestId = 0;
    this->m_HighestLayer = 0;
    this->main_content_iniitialied = false;
    this->isFullScreened = nullptr;
}

spe::SpriteRepository::~SpriteRepository()
{
    this->cleanUp();
}

// Public functions

spe::Sprite* spe::SpriteRepository::GetById(uint32_t idx)
{
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it)
    {
        spe::Sprite* element = *it;
        if (element->getId() == idx)
        {
            return element;
        }
    }
    throw std::out_of_range("Couldn't find sprite with the index");
}


void spe::SpriteRepository::DelteWithId(uint32_t idx)
{
    std::vector<spe::Sprite*> childs_to_delete = std::vector<spe::Sprite*>(0);
    spe::Sprite* ptr_delete = this->GetById((int32_t)idx);
    this->addChildsToDelete(childs_to_delete, ptr_delete);

    this->eraseWithIdx(this->GetListIndex(ptr_delete));

    for (int i = 0; i < childs_to_delete.size(); i++)
    {
        this->eraseWithIdx(this->GetListIndex(childs_to_delete[i]));
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

void spe::SpriteRepository::DeleteWithName(const std::string& name)
{
    this->DelteWithId(getIdWithName(name));
}

void spe::SpriteRepository::Add(spe::Sprite* ptr)
{
    // Validate properties
    // Set id
    this->m_HighestId++;

    if (ptr->parent != nullptr)
    {
        ptr->setParentId(ptr->parent->getId());
    }

    if (!this->main_content_iniitialied)
    {
        // We do not sort the pointers yet should be done after
        this->m_sprites.push_back(ptr);
        return;
    }
    else
    {
        this->ValidateAdd(ptr);
    }

    this->sortSpritesByLayer(ptr);

    if (this->main_content_iniitialied)
    {
        for (size_t i = 0; i < ptr->ptr_childs.size(); i++)
        {
            this->Add(ptr->ptr_childs[i]);
        }    
    }
}

spe::Sprite* spe::SpriteRepository::GetByName(const std::string& name)
{
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it)
    {
        spe::Sprite* sprite = *it;
        if (sprite->name == name)
        {
            return sprite;
        }
    }
    throw std::out_of_range("Sprite with name was not found");
}

uint32_t spe::SpriteRepository::GetListIndex(spe::Sprite* sprite)
{
    uint32_t idx = 0;
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it)
    {
        spe::Sprite* srch = *it;
        if (srch->getId() == sprite->getId())
        {
            return idx;
        }
        idx++;
    }
    throw std::out_of_range("Sprite was not found in the linked list");
}

void spe::SpriteRepository::UpdateLayerIndex()
{
    this->m_HighestLayer = this->m_sprites.front()->sprite_renderer.sorting_layer_index;
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it)
    {
        spe::Sprite* sprite = *it;
        if (sprite->sprite_renderer.sorting_layer_index > this->m_HighestLayer)
        {
            this->m_HighestLayer = sprite->sprite_renderer.sorting_layer_index;
        }
    }
}

void spe::SpriteRepository::ReloadTextures()
{
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it) 
    {
        spe::Sprite* element = *it;
        element->setSpriteTexture(element->sprite_renderer.path);
    }
}

void spe::SpriteRepository::cleanUp()
{
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it)
    {
        spe::Sprite* element = *it;
        delete element;
    }
    this->m_sprites.clear();
}

// Private functions

uint32_t spe::SpriteRepository::getIdWithName(const std::string& name) const
{
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it)
    {
        spe::Sprite* element = *it;
        if (element->name == name)
        {
            return element->getId();
        }
    }
    throw std::out_of_range("Name was not found");
}

void spe::SpriteRepository::addChildsToDelete(std::vector<spe::Sprite*>& ids, spe::Sprite* parent)
{
    for (int i = 0; i < parent->ptr_childs.size(); i++)
    {
        ids.push_back(parent->ptr_childs[i]);
        addChildsToDelete(ids, parent->ptr_childs[i]);
    }
}

void spe::SpriteRepository::eraseWithIdx(uint32_t idx)
{
    auto it = this->m_sprites.begin();
    std::advance(it, idx);
    if (it != this->m_sprites.end()) {
        this->m_sprites.erase(it);
        return;
    }
    throw std::out_of_range("Cant delete");
}

void spe::SpriteRepository::sortSpritesByLayer(spe::Sprite* spr)
{
    if (spr->sprite_renderer.sorting_layer_index >= this->m_HighestLayer)
    {
        this->m_HighestLayer = spr->sprite_renderer.sorting_layer_index;
        this->m_sprites.push_back(spr);
        return;
    }
    size_t i = 0;
    for (auto it = this->m_sprites.begin(); it != this->m_sprites.end(); ++it)
    {
        const spe::Sprite* read = *it;

        if (read->sprite_renderer.sorting_layer_index > spr->sprite_renderer.sorting_layer_index)
        {
            this->m_sprites.insert(it, spr);
            return;
        }
        i++;
    }
    throw std::logic_error("this should not happen jesus");
}

void spe::SpriteRepository::ValidateAdd(spe::Sprite* spr)
{
    spr->setId(this->m_HighestId);
}

// Static functions

void spe::SpriteRepository::SortSpritesByLayer()
{
    std::vector<spe::Sprite*> sprites(this->m_sprites.begin(), this->m_sprites.end());
    this->m_sprites.clear();

    for (spe::Sprite* spr : sprites)
    {
        this->sortSpritesByLayer(spr);
    }

    this->m_HighestLayer = this->m_sprites.back()->sprite_renderer.sorting_layer_index;
}

void spe::SpriteRepository::SetSpriteSortingLayer(uint32_t layer, spe::Sprite* spr)
{
    uint32_t old = spr->sprite_renderer.sorting_layer_index;
    spr->sprite_renderer.sorting_layer_index = layer;

    // Rare Case of chaning the highest layer#
    if (old == this->m_HighestLayer
        && layer < this->m_HighestLayer)
    {      
        this->UpdateLayerIndex();
    }

    uint32_t idx = this->GetListIndex(spr);
    this->eraseWithIdx(idx);
    this->sortSpritesByLayer(spr);
}

void spe::SpriteRepository::getAllChilds(std::vector<const spe::Sprite*>& childs, const spe::Sprite* parent)
{
    for (size_t i = 0; i < parent->ptr_childs.size(); i++)
    {
        childs.push_back(parent->ptr_childs[i]);
        getAllChilds(childs, parent->ptr_childs[i]);
    }
}


spe::Sprite* spe::SpriteRepository::getWithId(std::vector<spe::Sprite*>& collection, uint32_t id)
{
    for (uint32_t i = 0; i < collection.size(); i++)
    {
        spe::Sprite* const sprite = collection[i];
        if (sprite->getId() == id)
        {
            return sprite;
        }
    }
    throw std::out_of_range("ID was not found in the collection");
}