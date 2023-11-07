#pragma once

#include <spriteComponents/component.h>
#include <manager/lightRepository.h>

#define DEFAULT_LIGHT_RADIUS 100

namespace s2d
{
	class Sprite;
	class Light : public s2d::Component
	{
	private:
		void init() override;
		uint32_t m_light_index;
		float m_radius;
		bool m_radius_changed;

		float m_intensity;
		bool m_intensity_changed;

	public:
		s2d::Vector3 color;

		Sprite* ptr_attached_sprite;

		Light();
		Light(Sprite* ptr_sprite);

		uint32_t getLightIndex() const { return this->m_light_index; }
		void deleteLight();
		void enable();
		void reset() override;


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

