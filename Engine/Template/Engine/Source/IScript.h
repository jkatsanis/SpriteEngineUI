#pragma once

namespace spe
{
	class IScript
	{
	public:

		// Do not declare them as pure virtual because maybe user wants to pass a reference
		virtual void Update() { };
		virtual void Start() { };
	};
}