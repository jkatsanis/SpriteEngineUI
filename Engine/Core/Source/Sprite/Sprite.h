#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <Include/SFML/Graphics.hpp>
#include <Include/UtilityFunctions.h>

#include "Math/Vector2.h"
#include "Sprite/Components/BoxCollider.h"
#include "Sprite/Components/PhysicsBody.h"
#include "Sprite/Components/SpriteRenderer.h"
#include "Sprite/Components/Transform.h"
#include "Sprite/Components/Light/Light.h"
#include "Sprite/Components/Prefab/Prefab.h"
#include "Sprite/Components/Animator/Animator.h"

#define INVALID_SPRITE_SYMBOLS 1

namespace spe
{
	class Sprite
	{
	private:
		int32_t m_parent_id;
		sf::Sprite m_sprite;
		sf::Texture* m_texture;
		int32_t m_id;

		bool m_SetId;

		void initVariables(std::string name, spe::Vector2 spawnPos, std::string path, spe::LightRepository& lightrep);
	public:
		// General info
		std::string name;
		std::string tag;

		// Components
		spe::SpriteRenderer sprite_renderer;
		spe::BoxCollider collider;
		spe::PhsysicsBody physicsBody;
		spe::Transform transform;
		spe::Animator animator;
		spe::Prefab prefab;
		spe::Light light;

		//Parent / child infos
		std::vector<spe::Sprite*> ptr_childs;
		spe::Sprite* parent;

		Sprite();
		Sprite(std::string name, spe::Vector2 spawnPosition, std::string path, spe::LightRepository& lightrepo);
		Sprite(spe::Sprite& rhs);
		~Sprite();

		//////////////////////////////////////
		//// ENGINE FUNCTIONS 
		/////////////////////////////////////

		void setParentId(const int32_t id) { this->m_parent_id = id; }
		void setId(const int32_t id);
		sf::Sprite& getSprite() { return this->m_sprite; }
		sf::Texture& getTexture() { return *this->m_texture; }

		void clearParentData();
		void clearAllChilds() { this->ptr_childs.clear(); }

		//////////////////////////////////////
		//// USER FUNCTIONS 
		/////////////////////////////////////

		void setParent(spe::Sprite* parent);

		/// <summary>
		/// Removes the child from the childs list
		/// </summary>
		/// <param name="child">Child</param>
		void removeChild(const spe::Sprite* child);

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
		void setSpriteTexture(const std::string& path, const spe::Vector2& sclae);

		/// <summary>
		/// Gets the absulute parent
		/// </summary>
		spe::Sprite* getNode();

		/// <summary>
		/// Get's the id of the current sprite
		/// </summary>
		/// <returns></returns>
		int getId() const { return this->m_id; }

		bool IsParent() const { return this->ptr_childs.size() > 0; }

		bool ContainsChild(const spe::Sprite* child) const;

		bool ContainsChild(const ImGuiTextFilter& name) const;


		/// <summary>
		/// Gets the path to the texture file
		/// </summary>
		const std::string& getPathOfTextureFile() const { return this->sprite_renderer.path; }
	};
}
