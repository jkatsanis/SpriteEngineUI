#pragma once

#include <spriteComponents/component.h>


namespace s2d
{
	class Sprite;
	class Light : public s2d::Component
	{
	private:

		void init() override;
	public:
		float radius;
		Sprite* ptr_attached_sprite;

		Light();
		Light(Sprite* ptr_sprite);
	

		void reset() override;
	};
}

