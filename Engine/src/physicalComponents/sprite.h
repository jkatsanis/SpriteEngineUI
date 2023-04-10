#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <data/spriteData.h>
#include <stdExtension.h>
#include <physicalComponents/gameObject.h>
#include <spriteComponents/boxCollider.h>
#include <SFML/Graphics.hpp>
#include <spriteComponents/physicsBody.h>
#include <spriteComponents/animator.h>
#include <data/engineData.h>
#include <spriteComponents/prefab.h>
#include <manager/spriteRepository.h>

#define INVALID_SPRITE_SYMBOLS 1

namespace s2d
{
	class Sprite
	{
	private:
		static s2d::SpriteRepository* s_spriteRepository;

		sf::Sprite m_sprite;
		sf::Texture m_texture;
		int m_id;
		int m_parentId;
		int m_childCount;

		/// <summary>
		/// This is only used if it is a child. It is the position in the vector
		/// from the parent sprite
		/// </summary>
		int m_childListPos;

		void initVariables(std::string& name, s2d::Vector2& spawnPos, std::string& path);
		void validateProperties(std::string& name, s2d::Vector2& spawnPos, std::string& path);
	public:	
		// General info
		std::string name;
		std::string path;
		int sortingLayerIndex;

		// Components
		s2d::BoxCollider collider;
		s2d::PhsysicsBody physicsBody;
		s2d::Transform transform;
		s2d::Animator animator; 
		s2d::Prefab prefab;

		//Parent / child infos
		std::vector<std::unique_ptr<s2d::Sprite*>> childs;
		s2d::Sprite* parent;

		Sprite();
		Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path);
		~Sprite();


		/// <summary>
		/// Deletes all the childs of the sprite
		/// </summary>
		void deleteAllChilds();

		/// <summary>
		/// Deletes a sprite at the specified position
		/// </summary>
		/// <param name="idx">The idx of the child vector</param>
		void deleteChildAt(uint8_t idx);

		/// <summary>
		/// Rests the parent data
		/// </summary>
		void resetChildData();

		/// <summary>
		/// Sets the new texture of the sprite
		/// </summary>
		/// <param name="path">The path of the new texture</param>
		void setSpriteTexture(const std::string& path);

		/// <summary>
		/// Sets the new sprite texture
		/// IT DOES NOT!!! LOAD THE TEXTURE FROM THE FILE
		/// </summary>
		/// <param name="texture">The already loaded texture</param>
		/// <param name="path">The new path which needs to be set</param>
		void setSpriteTexture(const sf::Texture& texture, const std::string& path);

		/// <summary>
		/// LOADS the texture from the file and sets it scale
		/// </summary>
		/// <param name="path">Path to the .png file</param>
		/// <param name="sclae">Scale to set</param>
		void setSpriteTexture(const std::string& path, const s2d::Vector2& sclae);

		bool isParent() const { return this->childs.size() != 0; }

		bool containsChild(const s2d::Sprite* child) const;
	public:
		void setParent(s2d::Sprite* sprite);
		void setChildCount(const int cnt) { this->m_childCount = cnt; }
		void setParentId(const int id) { this->m_parentId = id; }
		void setId(const int id) { this->m_id = id; }
		void setChildListPos(const int pos) { this->m_childListPos = pos; }

		int getId() const { return this->m_id; }
		int getParentId() const { return this->m_parentId; }
		int getChildCount() const { return this->m_childCount; }
		int getChildListPosition() const { return this->m_childListPos; }

		sf::Sprite& getSprite() { return this->m_sprite; }	
		sf::Texture& getTexture() { return this->m_texture; }
		s2d::Vector2 getOrigininalPosition() const;

	public:
		static int getMaxNumber(std::vector<s2d::Sprite*>& vec);
		static void updateHightestLayerIndex();
		static s2d::Sprite* getSpriteById(int id);
		static void setSpriteRepository(s2d::SpriteRepository& repo) { s2d::Sprite::s_spriteRepository = &repo; }
		static int s_highestLayerIndex;
	};
}

