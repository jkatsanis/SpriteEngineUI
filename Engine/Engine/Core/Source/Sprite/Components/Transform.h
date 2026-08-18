#pragma once

#include "Math/Vector2.h"
#include "Sprite/Components/Component.h"

namespace spe
{
	class Sprite;
	class Transform : public spe::Component
	{
	private:
		spe::Vector2 m_Scale;
		uint32_t m_Rotation;
		spe::Vector2 m_Position;

		void Init() override;
		void UpdateSpritePositionToParent(const spe::Vector2& position);
		spe::Vector2 HandleCollisions(const spe::Vector2& pos);

	public:
		bool PositionChanged; // TODO
		spe::Vector2 TextureSize;
		spe::Vector2 PositionToParent;
		spe::Sprite* ptr_Sprite;

		Transform();
		Transform(spe::Sprite* attachedSprite);
		Transform(spe::Sprite* attachedSprite, spe::Transform& transform);

		void SetTextureSize(const spe::Vector2& scale);

		/// <summary>
		/// This method takes in a world posX and calculates 
		/// the scale needed to reach this world pos
		/// </summary>
		void CalculateScaleXByWorldPosition(const float posX);
		spe::Vector2 GetDefaultTextureSize() const noexcept;

		/// <summary>
		/// Updates the origin using the default size
		/// </summary>
		void SetOrigin();
		void SetPosition(const spe::Vector2& pos);
		void AddPositionY(float y);

		void SetPositionX(float x);
		void SetRotation(uint32_t angle);

		void Teleport(const spe::Vector2& pos);
		/// <summary>
		/// Set's the scale, sets it even if the parameter passed is the same as the getScale();
		/// </summary>
		/// <param name="setScaleForce">If you need to change the texture size even tho you have
		/// then same scale u need to call this method.</param>
		void SetScale(const spe::Vector2& scale, bool set_scale_force = false);

		const spe::Vector2& GetPosition() const noexcept { return this->m_Position; }
		spe::Vector2 const GetScale() const noexcept { return this->m_Scale; }

		/// <summary>
		/// Gets the position converted to SFML screen coordinates (Top-Left origin, Y-down).
		/// Used primarily for rendering.
		/// </summary>
		spe::Vector2 GetScreenPosition() const;

		// Forces an update of the internal SFML sprite position based on current World Position and Window Size
		void RefreshScreenPosition();

		uint32_t GetRotation() const noexcept { return this->m_Rotation; }

		/// <summary>
		/// Resets the transform
		/// </summary>
		void Reset() override;

	};
}
