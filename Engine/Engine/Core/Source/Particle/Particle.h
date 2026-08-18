#pragma once

#include <SFML/Graphics.hpp>
#include "Math/Vector2.h"

namespace spe
{
	struct Particle
	{
		spe::Vector2 Position;
		spe::Vector2 Velocity;
		float Rotation;
		float RotationSpeed;
		float Size;
		float StartSize;
		float EndSize;
		sf::Color Color;
		sf::Color StartColor;
		sf::Color EndColor;
		float Lifetime; // in seconds
		float RemainingLife;
		bool Active;

		Particle()
			: Position(0.0f, 0.0f)
			, Velocity(0.0f, 0.0f)
			, Rotation(0.0f)
			, RotationSpeed(0.0f)
			, Size(1.0f)
			, StartSize(1.0f)
			, EndSize(1.0f)
			, Color(sf::Color::White)
			, StartColor(sf::Color::White)
			, EndColor(sf::Color::White)
			, Lifetime(1.0f)
			, RemainingLife(1.0f)
			, Active(false)
		{
		}

		// returns lifetime left as percentage (0-1; 0 meaning just born, 1 is basically dead)
		float GetLifeRatio() const
		{
			if (Lifetime <= 0.0f) {
				return 1.0f;
			}
			return 1.0f - (RemainingLife / Lifetime);
		}

		// updates color and size based on time left (meanign if its about to die it will be closer to end color)
		void UpdateInterpolatedValues()
		{
			const float time = GetLifeRatio();

			Size = StartSize + (EndSize - StartSize) * time;

			Color.r = static_cast<sf::Uint8>(StartColor.r + (EndColor.r - StartColor.r) * time);
			Color.g = static_cast<sf::Uint8>(StartColor.g + (EndColor.g - StartColor.g) * time);
			Color.b = static_cast<sf::Uint8>(StartColor.b + (EndColor.b - StartColor.b) * time);
			Color.a = static_cast<sf::Uint8>(StartColor.a + (EndColor.a - StartColor.a) * time);
		}
	};
}

