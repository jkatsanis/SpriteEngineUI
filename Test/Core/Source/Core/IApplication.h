#pragma once

namespace spe
{
	class IAppliaction
	{
	protected:
		virtual void UpdateComponents() = 0;
		virtual void Init() = 0;
		IAppliaction() { };

	public:

		virtual void Update() = 0;
	};

}

