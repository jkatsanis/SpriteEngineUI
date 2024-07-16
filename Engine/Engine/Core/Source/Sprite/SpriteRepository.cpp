#include "SpriteRepository.h"
#include "Sprite.h"

// Constructor / Destructor

spe::SpriteRepository::SpriteRepository()
{
    this->m_HighestId = 0;
    this->m_HighestLayer = 0;
    this->Initialized = false;
    this->IsFullScreened = nullptr;
}

spe::SpriteRepository::~SpriteRepository()
{
    this->CleanUp();
}

// Public functions

spe::Sprite* spe::SpriteRepository::GetById(uint32_t idx)
{
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        spe::Sprite* element = *it;
        if ((uint32_t)element->GetId() == idx)
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
    this->AddChildsToDelete(childs_to_delete, ptr_delete);

    uint32_t highest_layer = ptr_delete->SpriteRenderer.SortinLayerIdx;

    this->EraseWithIdx(this->GetListIndex(ptr_delete));

    for (int i = 0; i < childs_to_delete.size(); i++)
    {
        if (childs_to_delete[i]->SpriteRenderer.SortinLayerIdx > highest_layer)
        {
            highest_layer = childs_to_delete[i]->SpriteRenderer.SortinLayerIdx;
        }
        this->EraseWithIdx(this->GetListIndex(childs_to_delete[i]));
    }
    for (int i = 0; i < childs_to_delete.size(); i++)
    {
        delete childs_to_delete[i];
        childs_to_delete[i] = nullptr;
    }

    // Rare Case of chaning the highest layer#
    if (highest_layer == this->m_HighestLayer)
    {
        this->UpdateLayerIndex();
    }


    // Destructor call
    delete ptr_delete;
    ptr_delete = nullptr;
}

void spe::SpriteRepository::DeleteWithName(const std::string& name)
{
    this->DelteWithId(GetIdWithName(name));
}

void spe::SpriteRepository::Add(spe::Sprite* ptr)
{
    // Validate properties
    // Set id
    this->m_HighestId++;

    if (ptr->ptr_Parent != nullptr)
    {
        ptr->SetParentId(ptr->ptr_Parent->GetId());
    }

    if (!this->Initialized)
    {
        // We do not sort the pointers yet should be done after
        this->m_Sprites.push_back(ptr);
        return;
    }
    else
    {
        this->ValidateAdd(ptr);
    }

    this->SortSpritesByLayer(ptr);

    if (this->Initialized)
    {
        for (size_t i = 0; i < ptr->ptr_Childs.size(); i++)
        {
            this->Add(ptr->ptr_Childs[i]);
        }    
    }
}

spe::Sprite* spe::SpriteRepository::GetByName(const std::string& name)
{
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        spe::Sprite* sprite = *it;
        if (sprite->Name == name)
        {
            return sprite;
        }
    }
    throw std::out_of_range("Sprite with name was not found");
}

bool spe::SpriteRepository::ExistWithId(uint32_t id)
{
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        spe::Sprite* sprite = *it;
        if ((uint32_t)sprite->GetId() == id)
        {
            return true;
        }
    }
    return false;
}

uint32_t spe::SpriteRepository::GetListIndex(spe::Sprite* sprite)
{
    uint32_t idx = 0;
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        spe::Sprite* srch = *it;
        if (srch->GetId() == sprite->GetId())
        {
            return idx;
        }
        idx++;
    }
    throw std::out_of_range("Sprite was not found in the linked list");
}

void spe::SpriteRepository::UpdateLayerIndex()
{
    if (this->m_Sprites.size() == 0)
    {
        return;
    }
    this->m_HighestLayer = this->m_Sprites.front()->SpriteRenderer.SortinLayerIdx;
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        spe::Sprite* sprite = *it;
        if (sprite->SpriteRenderer.SortinLayerIdx > this->m_HighestLayer)
        {
            this->m_HighestLayer = sprite->SpriteRenderer.SortinLayerIdx;
        }
    }
}

void spe::SpriteRepository::ReloadTextures()
{
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it) 
    {
        spe::Sprite* element = *it;
        element->SetSpriteTexture(element->SpriteRenderer.Path);
    }
}

void spe::SpriteRepository::CleanUp()
{
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end();)
    {
        spe::Sprite* element = *it;
        if (element->DontDeleteOnSceneSwap)
        {
            ++it; 
            continue;
        }
        delete element;
        it = this->m_Sprites.erase(it); 
    }

    this->Initialized = false;
    this->m_HighestLayer = 0;
}

void spe::SpriteRepository::DeleteAll()
{
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end();)
    {
        spe::Sprite* element = *it;
   
        delete element;
       
        it = this->m_Sprites.erase(it);
    }

    this->Initialized = false;
    this->m_HighestLayer = 0;
}

void spe::SpriteRepository::SetHighestId(uint32_t id)
{
    this->m_HighestId = id;
    spe::Log::LogString("Settind highest id...");
}

// Private functions

uint32_t spe::SpriteRepository::GetIdWithName(const std::string& name) const
{
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        spe::Sprite* element = *it;
        if (element->Name == name)
        {
            return element->GetId();
        }
    }
    throw std::out_of_range("Name was not found");
}

void spe::SpriteRepository::AddChildsToDelete(std::vector<spe::Sprite*>& ids, spe::Sprite* parent)
{
    for (int i = 0; i < parent->ptr_Childs.size(); i++)
    {
        ids.push_back(parent->ptr_Childs[i]);
        AddChildsToDelete(ids, parent->ptr_Childs[i]);
    }
}

void spe::SpriteRepository::EraseWithIdx(uint32_t idx)
{
    auto it = this->m_Sprites.begin();
    std::advance(it, idx);
    if (it != this->m_Sprites.end()) {
        this->m_Sprites.erase(it);
        return;
    }
    throw std::out_of_range("Cant delete");
}

void spe::SpriteRepository::SortSpritesByLayer(spe::Sprite* spr)
{
    if (spr->SpriteRenderer.SortinLayerIdx >= this->m_HighestLayer)
    {
        this->m_HighestLayer = spr->SpriteRenderer.SortinLayerIdx;
        this->m_Sprites.push_back(spr);
        return;
    }
    size_t i = 0;
    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        const spe::Sprite* read = *it;

        if (read->SpriteRenderer.SortinLayerIdx > spr->SpriteRenderer.SortinLayerIdx)
        {
            this->m_Sprites.insert(it, spr);
            return;
        }
        i++;
    }
    if (i == 0)
    {
        this->m_Sprites.push_back(spr);
        return;
    }
    throw std::logic_error("this should not happen jesus");
}

void spe::SpriteRepository::ValidateAdd(spe::Sprite* spr)
{
    spr->SetId(this->m_HighestId);

    for (auto it = this->m_Sprites.begin(); it != this->m_Sprites.end(); ++it)
    {
        spe::Sprite* element = *it;
        if (element->Name == spr->Name)
        {
            spr->Name += "(D)";
            it = this->m_Sprites.begin();
        }
        if (element->GetId() == spr->GetId())
        {
            throw new std::exception("This should not happen, probably added a sprite containing childs that already have been added to the list!");
        }
    }         
}

// Static functions

void spe::SpriteRepository::SortSpritesByLayer()
{
    if (this->m_Sprites.size() == 0)
    {
        return;
    }
    std::vector<spe::Sprite*> sprites(this->m_Sprites.begin(), this->m_Sprites.end());
    this->m_Sprites.clear();

    for (spe::Sprite* spr : sprites)
    {
        this->SortSpritesByLayer(spr);
    }

    this->m_HighestLayer = this->m_Sprites.back()->SpriteRenderer.SortinLayerIdx;
}

void spe::SpriteRepository::SetSpriteSortingLayer(uint32_t layer, spe::Sprite* spr)
{
    uint32_t old = spr->SpriteRenderer.SortinLayerIdx;
    spr->SpriteRenderer.SortinLayerIdx = layer;

    // Rare Case of chaning the highest layer#
    if (old == this->m_HighestLayer
        && layer < this->m_HighestLayer)
    {      
        this->UpdateLayerIndex();
    }

    uint32_t idx = this->GetListIndex(spr);
    this->EraseWithIdx(idx);
    this->SortSpritesByLayer(spr);
}

void spe::SpriteRepository::GetAllChilds(std::vector<const spe::Sprite*>& childs, const spe::Sprite* parent)
{
    for (size_t i = 0; i < parent->ptr_Childs.size(); i++)
    {
        childs.push_back(parent->ptr_Childs[i]);
        GetAllChilds(childs, parent->ptr_Childs[i]);
    }
}


spe::Sprite* spe::SpriteRepository::GetWithId(std::vector<spe::Sprite*>& collection, uint32_t id)
{
    for (uint32_t i = 0; i < collection.size(); i++)
    {
        spe::Sprite* const sprite = collection[i];
        if ((uint32_t)sprite->GetId() == id)
        {
            return sprite;
        }
    }
    return nullptr;
}