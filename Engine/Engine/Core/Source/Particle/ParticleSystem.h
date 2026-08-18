#pragma once

#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

#include "Math/Vector2.h"
#include "Particle/Particle.h"
#include "Particle/ParticleEmitterConfig.h"

namespace spe
{
	class ParticleSystem : public sf::Drawable
	{
	private:
		std::vector<Particle> m_Particles;
		sf::VertexArray m_Vertices;
		sf::Texture* m_Texture; // null = no texture (white pixel)
		ParticleEmitterConfig m_Config;

		spe::Vector2 m_EmitterPosition;
		float m_EmissionAccumulator;
		bool m_IsEmitting;

		// Random number generation
		std::mt19937 m_RandomEngine;

		void EmitParticle();
		void UpdateVertices();
		size_t FindDeadParticle();

		void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

		float RandomFloat(float min, float max);
		sf::Color RandomColor(const sf::Color& min, const sf::Color& max);
		spe::Vector2 GetSpawnPosition();
		spe::Vector2 GetInitialVelocity();

	public:
		ParticleSystem();
		ParticleSystem(const ParticleEmitterConfig& config);

		void Update(float deltaTime);
		void Play();
		void Stop();
		void Clear();

		void SetPosition(const spe::Vector2& position);
		void SetTexture(sf::Texture* texture);
		void SetConfig(const ParticleEmitterConfig& config);

		bool IsEmitting() const { return m_IsEmitting; }
		bool HasTexture() const { return m_Texture != nullptr; }
		uint32_t GetActiveParticleCount() const;
		const ParticleEmitterConfig& GetConfig() const { return m_Config; }
		ParticleEmitterConfig& GetConfig() { return m_Config; }
	};
}

