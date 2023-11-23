#pragma once

#include "Sprite/Components/Component.h"
#include "Sprite/Components/Light/LightRepository.h"

#define DEFAULT_LIGHT_RADIUS 100

namespace spe
{
	class Sprite;
	class Light : public spe::Component
	{
	private:
		void init() override;
		uint32_t m_light_index;
		float m_radius;
		bool m_radius_changed;
		sf::Vector3f m_color;
		bool m_color_changed;

		float m_intensity;
		bool m_intensity_changed;

	public:
		Sprite* ptr_attached_sprite;

		Light();
		Light(Sprite* ptr_sprite);
		Light(Sprite* ptr_sprite, const spe::Light& rhs);

	
		uint32_t getLightIndex() const { return this->m_light_index; }
		void deleteLight();
		void enable();
		void reset() override;

		const sf::Vector3f& getColor() const { return this->m_color; }
		void setColor(const sf::Vector3f& color);
		bool hasColorChanged() const { return this->m_color_changed; }
		void setColorChangeFlag() { this->m_color_changed = false; }


		void setIntensity(float intense);
		float getIntensity() const { return this->m_intensity; }
		bool hasIntensityChanged() const { return this->m_intensity_changed; }
		void setIntensityChangeFlagFalse() { this->m_intensity_changed = false; }

		void setRadius(float radius);
		float getRadius() const { return this->m_radius; }
		bool hasRadiusChanged() const { return this->m_radius_changed; }
		void setRadiosChangeFlagFalse() { this->m_radius_changed = false; };
	};
}

