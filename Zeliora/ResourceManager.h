#pragma once
#include <unordered_map>
#include <fstream>
#include "SoundStream.h"
#include "SoundEffect.h"

class SpriteSheet;

class ResourceManager final
{
public:
	enum class FontSize
	{
		size_18 = 18, size_36 = 36, size_72 = 72
	};

	enum class FontCategory
	{
		hover, classic
	};

	~ResourceManager();

	static ResourceManager* GetInstance();

	const Color4f& GetFontColor(FontCategory category) const;
	TTF_Font* GetFont(FontSize size) const;

	const SpriteSheet* GetSpriteSheet(const std::string& key) const;

	void PlaySoundEffect(const std::string name) const;
	void PlayMusic(const std::string name);
	void UpdateMusicVolume(unsigned int newValue);
	void UpdateEffectVolume(unsigned int newValue);

private:
	explicit ResourceManager();

	static ResourceManager* m_pInstance;

	std::unordered_map<FontSize, TTF_Font*> m_GameFonts;
	std::unordered_map<FontCategory, Color4f> m_GameFontColors;
	std::unordered_map<std::string, const SpriteSheet*> m_pSpriteSheets;
	std::unordered_map<std::string, const SoundStream*> m_pMusics;
	std::unordered_map<std::string, SoundEffect*> m_pSoundEffects;
	std::string m_CurrentMusic;
	const std::string m_ResourcesXmlPath;
	const std::string m_SpriteSheetRootDirectory;
	const std::string m_SoundsRootDirectory;

	void LoadFont();
	void LoadFontColors();
	void LoadSpriteSheets(std::ifstream& ifs, const std::string& xPathQuery);
	void LoadSoundEffects(std::ifstream& ifs, const std::string& xPathQuery);
	void LoadMusics(std::ifstream& ifs, const std::string& xPathQuery);
};

