#pragma once
#include <unordered_map>

class GameSettings final
{
public:
	static GameSettings* GetInstance();

	static const float m_GameMultiplierRatio;

	void SetGameWindow(const Window& gameWindow);
	const Window& GetGameWindow() const;
	bool GetInputResponse(std::string name) const;
	void UpdateWorldMultiplier(int playerLevel);
	float GetWorldMultiplier() const;

	unsigned int GetMusicVolume() const;
	unsigned int GetEffectVolume() const;
	void IncDecMusicVolume(int delta);
	void IncDecEffectsVolume(int delta);

private:
	explicit GameSettings();

	static GameSettings* m_pInstance;

	Window m_GameWindow;
	const unsigned int m_VolumeMaxValue{ 128 };
	unsigned int m_MusicVolume;
	unsigned int m_EffectVolume;
	float m_WorldMultiplier;
	std::unordered_map<std::string, SDL_Scancode> m_Inputs;

};

