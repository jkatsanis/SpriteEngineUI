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
	class Camera;
	class Sprite
	{
	private:
		int32_t m_ParentID;
		sf::Sprite m_Sprite;
		sf::Texture* m_Texture;
		int32_t m_ID;

		bool m_SetId;

		void InitVariables(spe::Vector2 spawnPos, std::string path, spe::LightRepository& lightrep);
	public:
		// General info
		std::string Name;
		std::string Tag;

		/// <summary>
		/// very wierd; use with caution
		/// </summary>
		bool DontDeleteOnSceneSwap;

		// Components
		spe::SpriteRenderer SpriteRenderer;
		spe::BoxCollider Collider;
		spe::PhsysicsBody Physicsbody;
		spe::Transform Transform;
		spe::Animator Animator;
		spe::Prefab Prefab;
		spe::Light Light;

		//Parent / child infos
		std::vector<spe::Sprite*> ptr_Childs;
		spe::Sprite* ptr_Parent;
		
		Sprite();
		Sprite(std::string name, spe::Vector2 spawnPosition, std::string path, spe::LightRepository& lightrepo);
		Sprite(spe::Sprite& rhs);
		~Sprite();

		//////////////////////////////////////
		//// ENGINE FUNCTIONS 
		/////////////////////////////////////

		void SetParentId(const int32_t id) noexcept { this->m_ParentID = id; }
		void SetId(const int32_t id) noexcept;
		sf::Sprite& GetSprite() { return this->m_Sprite; }
		sf::Texture& GetTexture() { return *this->m_Texture; }

		void ClearParentData();
		void ClearAllChilds() { this->ptr_Childs.clear(); }

		//////////////////////////////////////
		//// USER FUNCTIONS 
		/////////////////////////////////////

		void SetParent(spe::Sprite* parent);

		/// <summary>
		/// Removes the child from the childs list
		/// </summary>
		/// <param name="child">Child</param>
		void RemoveChild(const spe::Sprite* child);

		/// <summary>
		/// Gets the id of the parent
		/// </summary>
	
		/// <summary>
		/// Sets the new texture of the sprite
		/// </summary>
		/// <param name="path">The path of the new texture</param>
		void SetSpriteTexture(const std::string& path);

		/// <summary>
		/// Sets the new sprite texture
		/// IT DOES NOT!!! LOAD THE TEXTURE FROM THE FILE
		/// </summary>
		/// <param name="texture">The already loaded texture</param>
		/// <param name="path">The new path which needs to be set</param>
		void SetSpriteTexture(const sf::Texture& texture, const std::string& path);

		/// <summary>
		/// LOADS the texture from the file and sets it scale
		/// </summary>
		/// <param name="path">Path to the .png file</param>
		/// <param name="sclae">Scale to set</param>
		void SetSpriteTexture(const std::string& path, const spe::Vector2& sclae);

		/// <summary>
		/// Gets the absulute parent
		/// </summary>
		spe::Sprite* GetNode();

		/// <summary>
		/// Get's the id of the current sprite
		/// </summary>
		/// <returns></returns>
		int32_t GetId() const { return this->m_ID; }

		bool IsParent() const { return this->ptr_Childs.size() > 0; }

		bool ContainsChild(const spe::Sprite* child) const;

		bool ContainsChild(const ImGuiTextFilter& name) const;
		int GetParentId() const noexcept { return this->m_ParentID; }

		/// <summary>
		/// Gets the path to the texture file
		/// </summary>
		const std::string& getPathOfTextureFile() const { return this->SpriteRenderer.Path; }
	};
}
