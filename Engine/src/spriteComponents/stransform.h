#pragma once

#include <calculationComponents/vector2.h>
#include <spriteComponents/childSystem.h>
#include <spriteComponents/component.h>

namespace s2d
{
	class Sprite;
	class Transform : public s2d::Component
	{
	private:
		s2d::Vector2 m_scale;
		uint32_t m_rotation;

		void init() override;

	public:
		s2d::Vector2 position;
		s2d::Vector2 textureSize;
		s2d::Vector2 nextPos;
		s2d::Vector2 lastPos;
		s2d::Vector2 position_to_parent;
		s2d::Sprite* m_attached_sprite;

		bool position_changed;

		Transform();
		Transform(s2d::Sprite* attachedSprite);

		/// <summary>
		/// This method sets the last position of the transform. Needs to be user called.
		/// Sets "positionChanged" to true if it did that succesfully. 
		/// Needs to be called after the position changed (!)(!)
		/// </summary>
		void setLastPosition();
		void updateTransformPosition();
		void setTextureSize(const s2d::Vector2& scale);

		/// <summary>
		/// This method takes in a world posX and calculates 
		/// the scale needed to reach this world pos
		/// </summary>
		void calculateScaleXByWorldPosition(const float posX);
		s2d::Vector2 getDefaultTextureSize() const;

	public:
		
		/// <summary>
		/// Updates the origin using the texturesize
		/// </summary>
		void updateOrigin();

		void setRotation(uint32_t angle);

		/// <summary>
		/// Set's the scale, sets it even if the parameter passed is the same as the getScale();
		/// </summary>
		/// <param name="setScaleForce">If you need to change the texture size even tho you have
		/// then same scale u need to call this method.</param>
		void setScale(const s2d::Vector2& scale, bool set_scale_force = false);
		s2d::Vector2 const getScale() const { return this->m_scale; }

		/// <summary>
		/// Resets the transform
		/// </summary>
		void reset() override;

		uint32_t getRotation() const { return this->m_rotation; }

	public:
		//Used in poll events
		static void onPositionChange(s2d::Sprite* sprite);
	};
}

