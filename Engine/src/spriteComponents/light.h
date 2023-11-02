#pragma once

#include <spriteComponents/component.h>
#include <manager/lightRepository.h>

#define DEFAULT_LIGHT_RADIUS 20

namespace s2d
{
	class Sprite;
	class Light : public s2d::Component
	{
	private:

		void init() override;
	public:
		float radius;
		float intensity;
		s2d::Vector3 color;

		Sprite* ptr_attached_sprite;
		uint32_t light_index;

		Light();
		Light(Sprite* ptr_sprite);
	
		void enable();
		void reset() override;
	};
}

