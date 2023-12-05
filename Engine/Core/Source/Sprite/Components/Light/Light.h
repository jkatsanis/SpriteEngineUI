#pragma once

#include "Sprite/Components/Component.h"
#include "Sprite/Components/Light/LightRepository.h"

#define DEFAULT_LIGHT_RADIUS 100

#define THROW_IF_NO_LIGHT_REPO if (this->m_ptr_LighRepository == nullptr) \
							   {\
									throw std::runtime_error("You are a dumbas you dont have a light repo ptr");\
							   }
namespace spe
{
	class Sprite;
	class Light : public spe::Component
	{
	private:
		uint32_t m_LightIndex;
		float m_Radius;
		bool m_RadiusChanged;
		sf::Vector3f m_Color;
		bool m_ColorChanged;

		float m_Intensity;
		bool m_IntensityChanged;


		void init() override;
	public:
		Sprite* ptr_attached_sprite;
		spe::LightRepository* m_ptr_LighRepository;

		Light();
		Light(Sprite* ptr_sprite, spe::LightRepository* ptr);
		Light(Sprite* ptr_sprite, const spe::Light& rhs);
	
		uint32_t GetLightIndex() const noexcept { return this->m_LightIndex; }
		void DeleteLight();
		void Enable();
		void reset() override;

		const sf::Vector3f& GetColor() const noexcept { return this->m_Color; }
		void SetColor(const sf::Vector3f& color) noexcept;
		bool HasColorChanged() const noexcept { return this->m_ColorChanged; }
		void SetColorChangeFlag() noexcept { this->m_ColorChanged = false; }


		void SetIntensity(float intense) noexcept;
		float GetIntensity() const noexcept { return this->m_Intensity; }
		bool HasIntensityChanged() const noexcept { return this->m_IntensityChanged; }
		void SetIntensityChangeFlagFalse() noexcept { this->m_IntensityChanged = false; }

		void SetRadius(float radius) noexcept;
		float GetRadius() const noexcept { return this->m_Radius; }
		bool HasRadiusChanged() const noexcept  { return this->m_RadiusChanged; }
		void SetRadiosChangeFlagFalse() noexcept { this->m_RadiusChanged = false; };
	};
}

