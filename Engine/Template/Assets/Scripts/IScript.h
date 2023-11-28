#pragma once

namespace spe
{
	class IScript
	{
	public:
		IScript() { }

		virtual void Update() = 0;
		virtual void Start() = 0;
	};
}