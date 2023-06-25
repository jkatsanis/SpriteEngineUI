#pragma once

#include <calculationComponents/vector2.h>
#include <UIToolButtonsModels.h>
#include <stdExtension.h>

#include <vector>
#include <memory>
#include <iostream>
#include <string>

namespace s2d
{
    struct AssetFolderData
    {
        std::string darg_and_drop_name;
        std::string drag_and_drop_path;
    };
    class Sprite;
    class SpriteRepository
    {
    private:
        std::vector<s2d::Sprite*> m_sprites;
        int32_t m_highestLayerIndex;
        std::vector<std::string> m_tags;

        size_t getIndexAtName(const std::string& name) const;
        void addChildsToDelete(std::vector<s2d::Sprite*>& childs, s2d::Sprite* parent);
        void eraseWithId(uint8_t id);

    public:
        s2d::Sprite* child_to_parent;
        s2d::Sprite* right_clicked_sprite;
        s2d::Sprite* sprite_in_inspector; 
        s2d::Sprite* sprited_hovered_in_hierarchy;

        s2d::EditorTools current_tool;

        uint32_t highest_sprite_id;
        uint32_t dupeNameCounter;

        AssetFolderData asset_folder_data;
        bool* isFullScreened;

        SpriteRepository();
        ~SpriteRepository();
    
        size_t amount() const { return this->m_sprites.size(); }
        s2d::Sprite* const readAt(size_t idx);
        const s2d::Sprite* const readAt(size_t idx, bool constConst) const;
        void deleteAt(size_t idx);
        void deleteWithName(const std::string& name);
        void add(s2d::Sprite* ref);
        void addChildsOfParent(s2d::Sprite* parent);
        s2d::Sprite* getSpriteWithName(const std::string& name);
        s2d::Sprite* getSpriteWithId(int id);
        void instanitatePrefab(const std::string& path_to);


        void addTag(const std::string& tag);
        const std::vector<std::string>& getTags() const { return this->m_tags; }

        void cleanUp();
        void updateHighestLayerIndex();
        void reloadTextures();

        uint32_t getHighestLayerIndex() const { return this->m_highestLayerIndex; }

    public:
        static void getAllChilds(std::vector<const s2d::Sprite*>& childs, const s2d::Sprite* parent);
        static s2d::Sprite* getWithId(std::vector<s2d::Sprite*>& collection, uint32_t id);
        static void setValidParentIds(s2d::Sprite* parent, uint32_t highest_id);
    };
}

