#pragma once

namespace s2d
{
	class Component
	{
	public:
		bool exist;

		virtual void reset() = 0;
	protected:
		virtual void init() = 0;
	};
}

