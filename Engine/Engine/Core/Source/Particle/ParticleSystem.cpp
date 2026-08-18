#include "Particle/ParticleSystem.h"
#include "Core/Log.h"

#include <cmath>

#ifndef M_PI
#define M_PI 3.14159265358979323846f
#endif


namespace spe
{
	// static 1 pixel white texture for non-textured particles; needed for shaders
	static sf::Texture* s_WhiteTexture = nullptr;

	static sf::Texture* GetWhiteTexture()
	{
		if (s_WhiteTexture == nullptr)
		{
			s_WhiteTexture = new sf::Texture();
			sf::Image whiteImage;
			whiteImage.create(1, 1, sf::Color::White);
			s_WhiteTexture->loadFromImage(whiteImage);
		}
		return s_WhiteTexture;
	}

	ParticleSystem::ParticleSystem()
		: m_Texture(nullptr)
		, m_EmitterPosition(0.0f, 0.0f)
		, m_EmissionAccumulator(0.0f)
		, m_IsEmitting(false)
		, m_RandomEngine(std::random_device{}())
	{
		SetConfig(ParticleEmitterConfig{});
	}

	ParticleSystem::ParticleSystem(const ParticleEmitterConfig& config)
		: m_Texture(nullptr)
		, m_EmitterPosition(0.0f, 0.0f)
		, m_EmissionAccumulator(0.0f)
		, m_IsEmitting(false)
		, m_RandomEngine(std::random_device{}())
	{
		SetConfig(config);
	}

	void ParticleSystem::SetConfig(const ParticleEmitterConfig& config)
	{
		m_Config = config;

		// pre-allocate particles with max amoutn
		m_Particles.resize(m_Config.MaxParticles);
		for (auto& particle : m_Particles)
		{
			particle.Active = false;
		}

		// vertex array (4 vertices per particle for quads)
		m_Vertices.setPrimitiveType(sf::Quads);
		m_Vertices.resize(m_Config.MaxParticles * 4);
	}

	void ParticleSystem::Update(float deltaTime)
	{
		if (m_IsEmitting && m_Config.EmissionRate > 0.0f)
		{
			m_EmissionAccumulator += deltaTime;
			float timePerParticle = 1.0f / m_Config.EmissionRate;

			// int emittedThisFrame = 0;
			while (m_EmissionAccumulator >= timePerParticle)
			{
				EmitParticle();
				m_EmissionAccumulator -= timePerParticle;
				// emittedThisFrame++;
			}

			// static float debugTimer = 0.0f;
			// debugTimer += deltaTime;
			// if (debugTimer >= 1.0f)
			// {
			// 	spe::Log::LogString("ParticleSystem - Active: " + std::to_string(GetActiveParticleCount()) +
			// 		", Emitter pos: (" + std::to_string(m_EmitterPosition.X) + ", " + std::to_string(m_EmitterPosition.Y) + ")");
			// 	debugTimer = 0.0f;
			// }
		}

		// update all active particles
		for (auto& particle : m_Particles)
		{
			if (!particle.Active)
				continue;

			particle.RemainingLife -= deltaTime;
			if (particle.RemainingLife <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.Velocity.X += m_Config.Gravity.X * deltaTime;
			particle.Velocity.Y += m_Config.Gravity.Y * deltaTime;

			if (m_Config.Drag > 0.0f)
			{
				float dragFactor = 1.0f - m_Config.Drag * deltaTime;
				if (dragFactor < 0.0f) dragFactor = 0.0f;
				particle.Velocity.X *= dragFactor;
				particle.Velocity.Y *= dragFactor;
			}

			particle.Position.X += particle.Velocity.X * deltaTime;
			particle.Position.Y += particle.Velocity.Y * deltaTime;

			particle.Rotation += particle.RotationSpeed * deltaTime;

			// update interpolated values (color, size)
			particle.UpdateInterpolatedValues();
		}

		// Rebuild vertex array
		UpdateVertices();
	}

	void ParticleSystem::EmitParticle()
	{
		const size_t index = FindDeadParticle();
		if (index >= m_Particles.size())
			return;  // No available slot

		Particle& p = m_Particles[index];

		// Position
		p.Position = GetSpawnPosition();

		// Velocity
		p.Velocity = GetInitialVelocity();

		// Lifetime
		p.Lifetime = RandomFloat(m_Config.LifetimeMin, m_Config.LifetimeMax);
		p.RemainingLife = p.Lifetime;

		// Size
		p.StartSize = RandomFloat(m_Config.StartSizeMin, m_Config.StartSizeMax);
		p.EndSize = RandomFloat(m_Config.EndSizeMin, m_Config.EndSizeMax);
		p.Size = p.StartSize;

		// Color
		p.StartColor = RandomColor(m_Config.StartColorMin, m_Config.StartColorMax);
		p.EndColor = RandomColor(m_Config.EndColorMin, m_Config.EndColorMax);
		p.Color = p.StartColor;

		// Rotation
		p.Rotation = RandomFloat(m_Config.RotationMin, m_Config.RotationMax);
		p.RotationSpeed = RandomFloat(m_Config.RotationSpeedMin, m_Config.RotationSpeedMax);

		p.Active = true;
	}

	void ParticleSystem::UpdateVertices()
	{
		for (size_t i = 0; i < m_Particles.size(); ++i)
		{
			const Particle& p = m_Particles[i];
			size_t vertexIndex = i * 4;

			if (!p.Active)
			{
				// Set degenerate quad (invisible)
				for (size_t j = 0; j < 4; ++j)
				{
					m_Vertices[vertexIndex + j].position = sf::Vector2f(0.0f, 0.0f);
					m_Vertices[vertexIndex + j].color = sf::Color::Transparent;
				}
				continue;
			}

			float halfSize = p.Size / 2.0f;

			// Calculate rotated quad corners
			float cos_r = std::cos(p.Rotation * static_cast<float>(M_PI) / 180.0f);
			float sin_r = std::sin(p.Rotation * static_cast<float>(M_PI) / 180.0f);

			// Local corner positions (centered at origin)
			sf::Vector2f corners[4] = {
				{-halfSize, -halfSize},  // Top-left
				{ halfSize, -halfSize},  // Top-right
				{ halfSize,  halfSize},  // Bottom-right
				{-halfSize,  halfSize}   // Bottom-left
			};

			// Apply rotation and translate to particle position
			for (size_t j = 0; j < 4; ++j)
			{
				float rotatedX = corners[j].x * cos_r - corners[j].y * sin_r;
				float rotatedY = corners[j].x * sin_r + corners[j].y * cos_r;

				m_Vertices[vertexIndex + j].position = sf::Vector2f(
					p.Position.X + rotatedX,
					p.Position.Y + rotatedY
				);
				m_Vertices[vertexIndex + j].color = p.Color;
			}

			// Set texture coordinates if texture is used
			if (m_Texture)
			{
				sf::Vector2u texSize = m_Texture->getSize();
				m_Vertices[vertexIndex + 0].texCoords = sf::Vector2f(0.0f, 0.0f);
				m_Vertices[vertexIndex + 1].texCoords = sf::Vector2f(static_cast<float>(texSize.x), 0.0f);
				m_Vertices[vertexIndex + 2].texCoords = sf::Vector2f(static_cast<float>(texSize.x), static_cast<float>(texSize.y));
				m_Vertices[vertexIndex + 3].texCoords = sf::Vector2f(0.0f, static_cast<float>(texSize.y));
			}
		}
	}

	size_t ParticleSystem::FindDeadParticle()
	{
		// find first inactive particle
		for (size_t i = 0; i < m_Particles.size(); ++i)
		{
			if (!m_Particles[i].Active)
				return i;
		}
		return m_Particles.size();  // all particle slots filled
	}

	void ParticleSystem::draw(sf::RenderTarget& target, sf::RenderStates states) const
	{
		states.blendMode = m_Config.BlendMode;
		states.texture = m_Texture ? m_Texture : GetWhiteTexture();
		target.draw(m_Vertices, states);
	}

	void ParticleSystem::Play()
	{
		m_IsEmitting = true;
	}

	void ParticleSystem::Stop()
	{
		m_IsEmitting = false;
	}

	void ParticleSystem::Clear()
	{
		for (auto& particle : m_Particles)
		{
			particle.Active = false;
		}
		m_EmissionAccumulator = 0.0f;
		UpdateVertices();
	}

	void ParticleSystem::SetPosition(const spe::Vector2& position)
	{
		m_EmitterPosition = position;
	}

	void ParticleSystem::SetTexture(sf::Texture* texture)
	{
		m_Texture = texture;
	}

	uint32_t ParticleSystem::GetActiveParticleCount() const
	{
		uint32_t count = 0;
		for (const auto& particle : m_Particles)
		{
			if (particle.Active)
				++count;
		}
		return count;
	}


	float ParticleSystem::RandomFloat(float min, float max)
	{
		if (min >= max)
			return min;
		std::uniform_real_distribution<float> dist(min, max);
		return dist(m_RandomEngine);
	}

	sf::Color ParticleSystem::RandomColor(const sf::Color& min, const sf::Color& max)
	{
		return sf::Color(
			static_cast<sf::Uint8>(RandomFloat(min.r, max.r)),
			static_cast<sf::Uint8>(RandomFloat(min.g, max.g)),
			static_cast<sf::Uint8>(RandomFloat(min.b, max.b)),
			static_cast<sf::Uint8>(RandomFloat(min.a, max.a))
		);
	}

	spe::Vector2 ParticleSystem::GetSpawnPosition()
	{
		spe::Vector2 basePosition(
			m_EmitterPosition.X + m_Config.Offset.X,
			m_EmitterPosition.Y + m_Config.Offset.Y
		);

		switch (m_Config.Shape)
		{
		case EmissionShape::Point:
			return basePosition;

		case EmissionShape::Circle:
		{
			const float angle = RandomFloat(0.0f, 360.0f) * static_cast<float>(M_PI) / 180.0f;
			const float radius = RandomFloat(0.0f, m_Config.ShapeRadius);
			return spe::Vector2(
				basePosition.X + std::cos(angle) * radius,
				basePosition.Y + std::sin(angle) * radius
			);
		}

		case EmissionShape::Rectangle:
		{
			const float offsetX = RandomFloat(-m_Config.ShapeSize.X / 2.0f, m_Config.ShapeSize.X / 2.0f);
			const float offsetY = RandomFloat(-m_Config.ShapeSize.Y / 2.0f, m_Config.ShapeSize.Y / 2.0f);
			return spe::Vector2(
				basePosition.X + offsetX,
				basePosition.Y + offsetY
			);
		}


		case EmissionShape::Line:
		{
			// spawn along a line at an angle (example usage: waterfall foam)
			const float halfWidth = m_Config.ShapeSize.X / 2.0f;
			const float offset = RandomFloat(-halfWidth, halfWidth);
			const float angleRad = m_Config.LineAngle * static_cast<float>(M_PI) / 180.0f;
			return spe::Vector2(
				basePosition.X + std::cos(angleRad) * offset,
				basePosition.Y + std::sin(angleRad) * offset
			);
		}

		case EmissionShape::Edge:
		{
			// spawn along the edges of a rectangle (e.g. when highlighting smth)
			const float halfW = m_Config.ShapeSize.X / 2.0f;
			const float halfH = m_Config.ShapeSize.Y / 2.0f;
			const float perimeter = 2.0f * (m_Config.ShapeSize.X + m_Config.ShapeSize.Y);
			const float pos = RandomFloat(0.0f, perimeter);

			float offsetX, offsetY;
			if (pos < m_Config.ShapeSize.X)
			{
				// Top edge
				offsetX = pos - halfW;
				offsetY = -halfH;
			}
			else if (pos < m_Config.ShapeSize.X + m_Config.ShapeSize.Y)
			{
				// Right edge
				offsetX = halfW;
				offsetY = (pos - m_Config.ShapeSize.X) - halfH;
			}
			else if (pos < 2.0f * m_Config.ShapeSize.X + m_Config.ShapeSize.Y)
			{
				// Bottom edge
				offsetX = halfW - (pos - m_Config.ShapeSize.X - m_Config.ShapeSize.Y);
				offsetY = halfH;
			}
			else
			{
				// Left edge
				offsetX = -halfW;
				offsetY = halfH - (pos - 2.0f * m_Config.ShapeSize.X - m_Config.ShapeSize.Y);
			}
			return spe::Vector2(basePosition.X + offsetX, basePosition.Y + offsetY);
		}

		default:
			return basePosition;
		}
	}

	spe::Vector2 ParticleSystem::GetInitialVelocity()
	{
		const float speed = RandomFloat(m_Config.SpeedMin, m_Config.SpeedMax);

		// get emmisson angle from direction
		const float baseAngle = std::atan2(m_Config.Direction.Y, m_Config.Direction.X);

		const float spreadRad = m_Config.DirectionSpread * static_cast<float>(M_PI) / 180.0f;
		const float halfSpread = spreadRad / 2.0f;

		const float finalAngle = baseAngle + RandomFloat(-halfSpread, halfSpread);

		return spe::Vector2(
			std::cos(finalAngle) * speed,
			std::sin(finalAngle) * speed
		);
	}
}

