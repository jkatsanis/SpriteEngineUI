#pragma once

#include <calculationComponents/vector2.h>
#include <spriteComponents/childSystem.h>
#include <spriteComponents/boxCollider.h>

namespace s2d
{
	class Sprite;
	class Transform
	{
	private:
		s2d::Vector2 m_scale;

		/// <summary>
		/// Sets the texture size
		/// </summary>
		/// <param name="scale">The scale for the new textureSize</param>
		void setTextureSize(const s2d::Vector2& scale);

		void pushSetup();
		void pushSpriteFromCollider(s2d::BoxColliderPositionData::Position p, bool smaller, float& tXY, float& lXY, float& nXY);
	public:
		s2d::Vector2 position;
		s2d::Vector2 textureSize;
		s2d::Vector2 nextPos;
		s2d::Vector2 lastPos;
		s2d::Vector2 positionToParent;
		s2d::Sprite* m_attachedSprite;

		bool posiitonChanged;
		bool keepOpenInHirachy;

		Transform();
		Transform(s2d::Sprite* attachedSprite);


		/// <summary>
		/// This method sets the last position of the transform. Needs to be user called.
		/// Sets "positionChanged" to true if it did that succesfully. Needs to be called after the position changed (!)
		/// </summary>
		void setLastPosition();

		/// <summary>
		/// DO NOT USE
		/// </summary>
		void updateTransformPosition();

		/// <summary>
		/// Sets the new scale
		/// </summary>
		/// <param name="setScaleForce">Sets the scale even if its the same, not remommended</param>
		void setScale(const s2d::Vector2& scale, bool setScaleForce = false);
		s2d::Vector2 const getScale() { return this->m_scale; }

	public:
		//Used in poll events
		static void onPositionChange(s2d::Sprite* sprite);
	};
}

