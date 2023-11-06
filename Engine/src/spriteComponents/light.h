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

	public:
		float radius;
		float intensity;
		s2d::Vector3 color;

		Sprite* ptr_attached_sprite;

		Light();
		Light(Sprite* ptr_sprite);
	
		uint32_t getLightIndex() const { return this->m_light_index; }
		void deleteLight();
		void enable();
		void reset() override;
	};
}

