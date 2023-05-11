#pragma once

namespace s2d
{
	class Component
	{
	public:
		bool exist;
		bool base_component;

		virtual void reset() = 0;
	protected:
		virtual void init() = 0;
	};
}
