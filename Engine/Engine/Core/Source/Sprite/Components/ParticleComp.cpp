#include "ParticleComp.h"

#include "Core/Time.h"
#include "Core/Log.h"
#include "Sprite/Sprite.h"
#include "UtilityFunctions.h"

namespace spe
{
	void ParticleComp::Init()
	{
		this->BaseComponent = false;
		this->Exist = false;
		this->ptr_Sprite = nullptr;
		this->m_FollowSprite = true;
		this->m_TexturePath = "";
	}

	ParticleComp::ParticleComp()
	{
		this->ParticleComp::Init();
	}

	ParticleComp::ParticleComp(spe::Sprite* sprite)
	{
		this->ParticleComp::Init();
		this->ptr_Sprite = sprite;
	}

	ParticleComp::ParticleComp(spe::Sprite* sprite, const ParticleComp& comp)
	{
		this->ParticleComp::Init();
		this->ptr_Sprite = sprite;
		this->Exist = comp.Exist;
		this->m_FollowSprite = comp.m_FollowSprite;
		this->m_ParticleSystem.SetConfig(comp.m_ParticleSystem.GetConfig());
		if (!comp.m_TexturePath.empty())
		{
			this->SetTexturePath(comp.m_TexturePath);
		}
	}

	void ParticleComp::Reset()
	{
		this->Exist = false;
		this->m_FollowSprite = true;
		this->m_TexturePath = "";
		this->m_ParticleSystem.SetTexture(nullptr);
		this->m_ParticleSystem.Clear();
		this->m_ParticleSystem.SetConfig(ParticleEmitterConfig{});
	}

	void ParticleComp::Update()
	{
		if (!this->Exist)
			return;

		if (m_FollowSprite && ptr_Sprite)
		{
			// world coords
			spe::Vector2 gamePos = ptr_Sprite->Transform.GetPosition();
			// convert to sfml coords; y is inverted in sfml
			spe::Vector2 worldPos(gamePos.X + 960, 540 - gamePos.Y);
			m_ParticleSystem.SetPosition(worldPos);
		}

		m_ParticleSystem.Update(spe::Time::s_DeltaTime);
	}

	void ParticleComp::Play()
	{
		if (!this->Exist)
		{
			spe::Log::LogString("ParticleComp::Play() - Exist is false, not playing");
			return;
		}

		// Set initial position before playing
		if (ptr_Sprite)
		{
			spe::Vector2 gamePos = ptr_Sprite->Transform.GetPosition();
			// Convert to SFML screen coordinates (Y is inverted in SFML)
			spe::Vector2 worldPos(gamePos.X + 960, 540 - gamePos.Y);
			m_ParticleSystem.SetPosition(worldPos);
			// spe::Log::LogString("ParticleComp::Play() - Playing at world position: " + std::to_string(worldPos.X) + ", " + std::to_string(worldPos.Y));
		}
		else
		{
			spe::Log::LogString("ParticleComp::Play() - ptr_Sprite is null!");
		}

		m_ParticleSystem.Play();
	}

	void ParticleComp::Stop()
	{
		m_ParticleSystem.Stop();
	}

	void ParticleComp::Clear()
	{
		m_ParticleSystem.Clear();
	}

	void ParticleComp::SetTexturePath(const std::string& path)
	{
		if (path.empty())
		{
			m_TexturePath = "";
			m_ParticleSystem.SetTexture(nullptr);
			return;
		}

		std::string updatedPath = path;
#ifdef __linux__
		updatedPath = spe::Utility::ToLinuxPath(path);
#else
		updatedPath = spe::Utility::ToWindowsPath(path);
#endif

		if (m_Texture.loadFromFile(updatedPath))
		{
			m_TexturePath = updatedPath;
			m_ParticleSystem.SetTexture(&m_Texture);
		}
		else
		{
			spe::Log::LogString("Failed to load particle texture: " + updatedPath);
			m_TexturePath = "";
			m_ParticleSystem.SetTexture(nullptr);
		}
	}
}

