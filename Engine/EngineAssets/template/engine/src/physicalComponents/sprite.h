#pragma once

#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <spriteComponents/spriteData.h>
#include <physicalComponents/gameObject.h>
#include <spriteComponents/boxCollider.h>
#include <SFML/Graphics.hpp>
#include <spriteComponents/physicsBody.h>
#include <stdExtension.h>

namespace s2d
{
	class Sprite
	{
	private:
		int m_childCount;
		int m_parentId;


		sf::Sprite m_sprite;
		sf::Texture m_texture;
		std::string m_path;
		int m_vectorPosition;
		int m_id;

		/// <summary>
		/// This is only used if it is a child. It is the position in the vector
		/// from the parent sprite
		/// </summary>
		int m_childListPos;

		void initVariables(std::string name, s2d::Vector2 spawnPos, std::string path);

	public:
		// General info
		std::string name;
		int sortingLayerIndex;

		// Components
		s2d::BoxCollider collider;
		s2d::PhsysicsBody physicsBody;
		s2d::Transform transform;

		//Parent / child infos
		std::vector<s2d::Sprite*> childs;
		s2d::Sprite* parent;

		Sprite();
		Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path);
		Sprite(std::string name, s2d::Vector2 spawnPosition, std::string path, bool addToWindowByConstruction);

		//////////////////////////////////////
		//// ENGINE FUNCTIONS 
		/////////////////////////////////////

		/// <summary>
		/// DO NOT USE
		/// </summary>
		void setId(const int id) { this->m_id = id; }

		/// <summary>
		/// DO NOT USE
		/// </summary>
		void setVectorPosition(const int vec) { this->m_vectorPosition = vec; }

		/// <summary>
		/// DO NOT USE
		/// </summary>
		sf::Sprite& getSprite() { return this->m_sprite; }

		/// <summary>
		/// DO NOT USE
		/// </summary>
		sf::Texture& getTexture() { return this->m_texture; }

		//////////////////////////////////////
		//// USER FUNCTIONS 
		/////////////////////////////////////

		/// <summary>
		/// Sets the new texture of the sprite
		/// </summary>
		/// <param name="path">The path of the new texture</param>
		void setSpriteTexture(const std::string path);

		/// <summary>
		/// Pushed the sprite to the sprites vector, gets rendered
		/// automaticly
		/// </summary>
		void addSpriteToScene();

		/// <summary>
		/// Gets the original position as a Cartesian coordinate system point (vector).
		/// </summary>
		/// <returns>The position as a vector</returns>
		s2d::Vector2 getOrigininalPosition();

		/// <summary>
		/// Renders the sprite instant to the screen when you create a 
		/// 'new' sprite, do not use this functions if you don't have a reason to 
		/// </summary>
		void renderInstant();

		/// <summary>
		/// Gets the absulute parent
		/// </summary>
		s2d::Sprite* getNode(); 

		/// <summary>
		/// Updates the sprite texture of only 1 sprite (reads the path from the file)
		/// </summary>
		void updateSpriteTexture();

		/// <summary>
		/// Get's the id of the current sprite
		/// </summary>
		/// <returns></returns>
		int getId() const { return this->m_id; }

		/// <summary>
		/// Gets the path to the texture file
		/// </summary>
		const std::string& getPathOfTextureFile() const { return this->m_path; }

		/// <summary>
		/// Gets the position in the s2d::Spirte::activeSprite array
		/// </summary>
		int getVectorPosition() { return this->m_vectorPosition; }

	public:
		static void initActiveSprites();
		static int getMaxNumber(std::vector<s2d::Sprite*>& vec);
		static void updateHightestLayerIndex();
		static s2d::Sprite* getSpriteById(int id);
		static Sprite* getSpriteByName(std::string name);
		static void deleteSpriteByName(const std::string& name);
		static int highestLayerIndex;

		//Pointers getting deletet in gameWindow.cpp ~
		static std::vector<s2d::Sprite*> activeSprites;
	};
}

