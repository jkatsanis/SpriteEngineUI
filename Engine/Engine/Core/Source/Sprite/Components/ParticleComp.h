#pragma once

#include <string>
#include "Sprite/Components/Component.h"
#include "Particle/ParticleSystem.h"

namespace spe
{
	class Sprite;

	class ParticleComp : public spe::Component
	{
	private:
		Sprite* ptr_Sprite;
		ParticleSystem m_ParticleSystem;
		bool m_FollowSprite;  // Update particle position to sprite position
		std::string m_TexturePath;
		sf::Texture m_Texture;

		void Init() override;

	public:
		ParticleComp();
		ParticleComp(spe::Sprite* sprite);
		ParticleComp(spe::Sprite* sprite, const ParticleComp& comp);

		void Reset() override;
		void Update();

		void Play();
		void Stop();
		void Clear();

		void SetTexturePath(const std::string& path);
		const std::string& GetTexturePath() const { return m_TexturePath; }
		void SetFollowSprite(bool follow) { m_FollowSprite = follow; }
		bool GetFollowSprite() const { return m_FollowSprite; }

		ParticleSystem& GetParticleSystem() { return m_ParticleSystem; }
		const ParticleSystem& GetParticleSystem() const { return m_ParticleSystem; }

		ParticleEmitterConfig& GetConfig() { return m_ParticleSystem.GetConfig(); }
		const ParticleEmitterConfig& GetConfig() const { return m_ParticleSystem.GetConfig(); }
	};
}

