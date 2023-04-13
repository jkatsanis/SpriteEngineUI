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
		sf::Sprite m_sprite;
	    sf::Texture* m_texture;
		uint32_t m_id;
		int32_t m_parentId;

		void initVariables(std::string& name, s2d::Vector2& spawnPos, std::string& path);
	public:	
		// General info
		std::string name;
		std::string path;
		int32_t sortingLayerIndex;

		// Components
		s2d::BoxCollider collider;
		s2d::PhsysicsBody physicsBody;
		s2d::Transform transform;
		s2d::Animator animator; 
		s2d::Prefab prefab;

		//Parent / child infos
		// Pointer to all the childs.
		// The childs are located in the sprite repository
		// So no deletions are happening here
		std::vector<s2d::Sprite*> ptr_childs;
		s2d::Sprite* parent;

		Sprite();
		Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path);
		~Sprite();

		void validateProperties(uint32_t id, s2d::SpriteRepository& repo);

		/// <summary>
		/// Removes the child from the childs list
		/// </summary>
		/// <param name="child">Child</param>
		void removeChild(const s2d::Sprite* child);

		/// <summary>
		/// Clears all the childs of the sprite
		/// DOES NOT DELETE THEM
		/// </summary>
		void clearAllChilds();

		/// <summary>
		/// Erases the child from the parents list if contains
		/// </summary>
		void clearParentData();

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

		bool isParent() const { return this->ptr_childs.size() != 0; }

		bool containsChild(const s2d::Sprite* child) const;
	public:
		void setParent(s2d::Sprite* sprite);

		uint32_t getId() const { return this->m_id; }
		int32_t getParentId() const { return this->m_parentId; }

		void setId(uint32_t id) { this->m_id = id; }
		void setParentId(int32_t id) { this->m_parentId = id; }

		sf::Sprite& getSprite() { return this->m_sprite; }	
		sf::Texture& getTexture() { return *this->m_texture; }
		s2d::Vector2 getOrigininalPosition() const;

	};
}

