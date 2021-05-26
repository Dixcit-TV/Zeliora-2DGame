#include "pch.h"
#include "GameSettings.h"
#include "ResourceManager.h"

GameSettings* GameSettings::m_pInstance = nullptr;
const float GameSettings::m_GameMultiplierRatio = 1.5f;

GameSettings::GameSettings()
	: m_MusicVolume{ m_VolumeMaxValue / 2 }
	, m_EffectVolume{ m_VolumeMaxValue / 2 }
	, m_WorldMultiplier{ 1.f }
	, m_Inputs{}
{
	ResourceManager::GetInstance()->UpdateMusicVolume(m_MusicVolume);
	ResourceManager::GetInstance()->UpdateEffectVolume(m_EffectVolume);

	m_Inputs.emplace("MoveUp", SDL_SCANCODE_W);
	m_Inputs.emplace("MoveDown", SDL_SCANCODE_S);
	m_Inputs.emplace("MoveRight", SDL_SCANCODE_D);
	m_Inputs.emplace("MoveLeft", SDL_SCANCODE_A);
}

GameSettings* GameSettings::GetInstance()
{
	return !m_pInstance ? (m_pInstance = new GameSettings()) : m_pInstance;
}

void GameSettings::SetGameWindow(const Window& gameWindow)
{
	m_GameWindow = gameWindow;
}

const Window& GameSettings::GetGameWindow() const
{
	return m_GameWindow;
}

bool GameSettings::GetInputResponse(std::string name) const
{
	std::unordered_map<std::string, SDL_Scancode>::const_iterator inputcIt{ m_Inputs.find(name) };
	if (inputcIt != m_Inputs.cend())
	{
		const Uint8* pStates = SDL_GetKeyboardState(nullptr);
		return pStates[inputcIt->second];
	}

	return false;
}

void GameSettings::UpdateWorldMultiplier(int newValue)
{
	m_WorldMultiplier = std::pow(m_GameMultiplierRatio, newValue);
}

float GameSettings::GetWorldMultiplier() const
{
	return m_WorldMultiplier;
}

unsigned int GameSettings::GetMusicVolume() const
{
	return m_MusicVolume;
}

unsigned int GameSettings::GetEffectVolume() const
{
	return m_EffectVolume;
}

void GameSettings::IncDecMusicVolume(int delta)
{
	if ((m_MusicVolume == 0 && delta < 0) || (m_MusicVolume == m_VolumeMaxValue && delta > 0))
		return;

	m_MusicVolume += delta;
	ResourceManager::GetInstance()->UpdateMusicVolume(m_MusicVolume);
}
void GameSettings::IncDecEffectsVolume(int delta)
{
	if ((m_EffectVolume == 0 && delta < 0) || (m_EffectVolume == m_VolumeMaxValue && delta > 0))
		return;

	m_EffectVolume += delta;
	ResourceManager::GetInstance()->UpdateEffectVolume(m_EffectVolume);
}
