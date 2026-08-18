#pragma once

#include <cstdint>
#include <SFML/Graphics.hpp>
#include "Math/Vector2.h"

namespace spe
{
	enum class EmissionShape
	{
		Point,
		Circle,
		Rectangle,
		Line,
		Edge // edge of rectangle (meaning its only on the borders of it)
	};

	struct ParticleEmitterConfig
	{
		bool PlayOnStart;			// by default true; plays the particles on scene load

		float EmissionRate;         // particles per second
		uint32_t MaxParticles;      // self explanatory (max particles that are allowed to spawn)
		EmissionShape Shape;
		float ShapeRadius;          // for circle shape
		spe::Vector2 ShapeSize;     // for rect/line/edge shapes
		float LineAngle;            // rotation angle for line shape in deg

		float LifetimeMin;
		float LifetimeMax;
		float SpeedMin;
		float SpeedMax;
		spe::Vector2 Direction;
		float DirectionSpread;      // spread angle in degrees (0-360)
		spe::Vector2 Offset;        // offset of the particles from the sprite center (default position)

		sf::Color StartColorMin;
		sf::Color StartColorMax;
		sf::Color EndColorMin;
		sf::Color EndColorMax;

		float StartSizeMin;
		float StartSizeMax;
		float EndSizeMin;
		float EndSizeMax;

		float RotationMin;
		float RotationMax;
		float RotationSpeedMin;
		float RotationSpeedMax;

		spe::Vector2 Gravity;       // e.g. (0, 10) for falling down; (10, -10) for diagonal up right
		float Drag;                 // 0 means speed stays consistent, higher values means speed decreases over time

		sf::BlendMode BlendMode;

		ParticleEmitterConfig()
			: PlayOnStart(true)
			, EmissionRate(50.0f)
			, MaxParticles(500)
			, Shape(EmissionShape::Point)
			, ShapeRadius(10.0f)
			, ShapeSize(10.0f, 10.0f)
			, LineAngle(0.0f)
			, LifetimeMin(1.0f)
			, LifetimeMax(2.0f)
			, SpeedMin(50.0f)
			, SpeedMax(100.0f)
			, Direction(0.0f, -1.0f)
			, DirectionSpread(15.0f)
			, Offset(0.0f, 0.0f)
			, StartColorMin(sf::Color::White)
			, StartColorMax(sf::Color::White)
			, EndColorMin(sf::Color(255, 255, 255, 0))
			, EndColorMax(sf::Color(255, 255, 255, 0))
			, StartSizeMin(4.0f)
			, StartSizeMax(8.0f)
			, EndSizeMin(1.0f)
			, EndSizeMax(2.0f)
			, RotationMin(0.0f)
			, RotationMax(360.0f)
			, RotationSpeedMin(0.0f)
			, RotationSpeedMax(0.0f)
			, Gravity(0.0f, 0.0f)
			, Drag(0.0f)
			, BlendMode(sf::BlendAlpha)
		{
		}
	};
}

