#pragma once

namespace spe
{
	class Component
	{
	public:
		bool Exist;
		bool BaseComponent;

		virtual void Reset() = 0;
	protected:
		virtual void Init() = 0;
	};
}
