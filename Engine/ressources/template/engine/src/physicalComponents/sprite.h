#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <physicalComponents/gameObject.h>
#include <spriteComponents/boxCollider.h>
#include <SFML/Graphics.hpp>
#include <spriteComponents/physicsBody.h>
#include <stdExtension.h>
#include <spriteComponents/animator.h>
#include <manager/spriteRepository.h>
#include <spriteComponents/spriteRenderer.h>
#include <spriteComponents/prefab.h>
#include <spriteComponents/light.h>

#define INVALID_SPRITE_SYMBOLS 1

namespace s2d
{
	class Sprite
	{
	private:
		int m_parent_id;
		sf::Sprite m_sprite;
		sf::Texture* m_texture;
		int m_id;

		void initVariables(std::string name, s2d::Vector2 spawnPos, std::string path);
	public:
		// General info
		std::string name;
		std::string tag;

		// Components
		s2d::SpriteRenderer sprite_renderer;
		s2d::BoxCollider collider;
		s2d::PhsysicsBody physicsBody;
		s2d::Transform transform;
		s2d::Animator animator;
		s2d::Prefab prefab;
		s2d::Light light;

		//Parent / child infos
		std::vector<s2d::Sprite*> ptr_childs;
		s2d::Sprite* parent;

		Sprite();
		Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path);
		Sprite(s2d::Sprite& rhs);
		~Sprite();

		//////////////////////////////////////
		//// ENGINE FUNCTIONS 
		/////////////////////////////////////

		void validateProperties(int id, s2d::SpriteRepository& repo);
		void postInit();
		void setParentId(const int id) { this->m_parent_id = id; }
		void setId(const int id) { this->m_id = id; }
		sf::Sprite& getSprite() { return this->m_sprite; }
		sf::Texture& getTexture() { return *this->m_texture; }

		void clearParentData();
		void clearAllChilds() { this->ptr_childs.clear(); }

		//////////////////////////////////////
		//// USER FUNCTIONS 
		/////////////////////////////////////

		void setParent(s2d::Sprite* parent);

		/// <summary>
		/// Removes the child from the childs list
		/// </summary>
		/// <param name="child">Child</param>
		void removeChild(const s2d::Sprite* child);

		/// <summary>
		/// Gets the id of the parent
		/// </summary>
		int getParentId() const { return this->m_parent_id; }

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

		/// <summary>
		/// Gets the original position as a Cartesian coordinate system point (vector).
		/// </summary>
		/// <returns>The position as a vector</returns>
		s2d::Vector2 getOrigininalPosition();

		/// <summary>
		/// Gets the absulute parent
		/// </summary>
		s2d::Sprite* getNode(); 

		/// <summary>
		/// Get's the id of the current sprite
		/// </summary>
		/// <returns></returns>
		int getId() const { return this->m_id; }

		/// <summary>
		/// Gets the path to the texture file
		/// </summary>
		const std::string& getPathOfTextureFile() const { return this->sprite_renderer.path; }
	};
}

