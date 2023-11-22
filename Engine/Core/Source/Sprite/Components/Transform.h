#pragma once

#include "Math/Vector2.h"
#include "Component.h"

namespace spe
{
	class Sprite;
	class Transform : public spe::Component
	{
	private:
		spe::Vector2 m_scale;
		uint32_t m_rotation;
		spe::Vector2 m_Position;

		void init() override;
		void UpdateSpritePositionToParent(const spe::Vector2& position);

	public:
		bool position_changed; // TODO
		spe::Vector2 texture_size;
		spe::Vector2 position_to_parent;
		spe::Sprite* m_attached_sprite;

		Transform();
		Transform(spe::Sprite* attachedSprite);
		Transform(spe::Sprite* attachedSprite, spe::Transform& transform);

		void setTextureSize(const spe::Vector2& scale);

		/// <summary>
		/// This method takes in a world posX and calculates 
		/// the scale needed to reach this world pos
		/// </summary>
		void calculateScaleXByWorldPosition(const float posX);
		spe::Vector2 getDefaultTextureSize() const;

		/// <summary>
		/// Updates the origin using the ´default size
		/// </summary>
		void setOrigin();

		const spe::Vector2& getPosition() { return this->m_Position; }

		/// <summary>
		/// Sets the position
		/// </summary>
		/// <param name="pos">The Positon</param>
		void SetPosition(const spe::Vector2& pos);

		/// <summary>
		/// Sets the rotation
		/// </summary>
		void setRotation(uint32_t angle);

		/// <summary>
		/// Set's the scale, sets it even if the parameter passed is the same as the getScale();
		/// </summary>
		/// <param name="setScaleForce">If you need to change the texture size even tho you have
		/// then same scale u need to call this method.</param>
		void setScale(const spe::Vector2& scale, bool set_scale_force = false);
		spe::Vector2 const getScale() const { return this->m_scale; }

		/// <summary>
		/// Resets the transform
		/// </summary>
		void reset() override;

		uint32_t getRotation() const { return this->m_rotation; }
	};
}

