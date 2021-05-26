#include "pch.h"
#include "ResourceManager.h"
#include <algorithm>
#include <stdexcept>
#include <fstream>
#include "utils.h"
#include "SpriteSheet.h"

ResourceManager* ResourceManager::m_pInstance = nullptr;

ResourceManager::ResourceManager()
	: m_GameFonts{}
	, m_GameFontColors{}
	, m_pSpriteSheets{}
	, m_pMusics{}
	, m_pSoundEffects{}
	, m_CurrentMusic{ "" }
	, m_SpriteSheetRootDirectory{ "Resources/SpriteSheets/" }
	, m_ResourcesXmlPath{ "Resources/Data/Resources.xml" }
	, m_SoundsRootDirectory{ "Resources/Sounds/" }
{
	LoadFont();
	LoadFontColors();

	std::ifstream iS{};
	iS.open(m_ResourcesXmlPath);

	if (!iS.is_open())
	{
		throw std::runtime_error("Couldn't open file " + m_ResourcesXmlPath);
	}

	LoadSpriteSheets(iS, "SpriteSheets/SpriteSheet");
	LoadSoundEffects(iS, "Sounds/SoundEfects/SoundEffect");
	LoadMusics(iS, "Sounds/Musics/Music");

	iS.close();
}

ResourceManager::~ResourceManager()
{
	auto closeFont = [](std::pair<FontSize, TTF_Font*> fontPair) { TTF_CloseFont(fontPair.second); };
	std::for_each(m_GameFonts.begin(), m_GameFonts.end(), closeFont);

	for (const std::pair<const std::string, const SpriteSheet*>& spriteSheet : m_pSpriteSheets)
		delete spriteSheet.second;

	for (const std::pair<const std::string, const SoundEffect*>& soundEffects : m_pSoundEffects)
		delete soundEffects.second;

	for (const std::pair<const std::string, const SoundStream*>& music : m_pMusics)
		delete music.second;
}

ResourceManager* ResourceManager::GetInstance()
{
	return !m_pInstance ? m_pInstance = new ResourceManager() : m_pInstance;
}

#pragma region Fonts
void ResourceManager::LoadFont()
{
	m_GameFonts.emplace(FontSize::size_18, TTF_OpenFont("Resources/Font/Perfect DOS VGA 437 Win.ttf", (int)FontSize::size_18));
	m_GameFonts.emplace(FontSize::size_36, TTF_OpenFont("Resources/Font/Perfect DOS VGA 437 Win.ttf", (int)FontSize::size_36));
	m_GameFonts.emplace(FontSize::size_72, TTF_OpenFont("Resources/Font/Perfect DOS VGA 437 Win.ttf", (int)FontSize::size_72));
}

void ResourceManager::LoadFontColors()
{
	m_GameFontColors.emplace(FontCategory::classic, Color4f{ .8f, .8f, .8f, 1.f });
	m_GameFontColors.emplace(FontCategory::hover, Color4f{ 0.8f, 0.3f, 0.2f, 1.f });
}

const Color4f& ResourceManager::GetFontColor(FontCategory category) const
{
	return m_GameFontColors.find(category)->second;
}

TTF_Font* ResourceManager::GetFont(FontSize size) const
{
	return m_GameFonts.find(size)->second;
}
#pragma endregion

#pragma region SpriteSheets
const SpriteSheet* ResourceManager::GetSpriteSheet(const std::string& key) const
{
	return m_pSpriteSheets.at(key);
}

void ResourceManager::LoadSpriteSheets(std::ifstream& ifs, const std::string& xPathQuery)
{
	ifs.seekg(0);
	std::vector<std::string> elements = utils::GetXmlElements(ifs, xPathQuery);

	std::string key{};
	SpriteSheet* newSpriteSheet{ nullptr };
	for (std::string& elem : elements)
	{
		key = utils::GetXmlAttributeValue("Path", elem);
		newSpriteSheet = new SpriteSheet(m_SpriteSheetRootDirectory + key, std::stoi(utils::GetXmlAttributeValue("Columns", elem)), std::stoi(utils::GetXmlAttributeValue("Rows", elem)));

		m_pSpriteSheets.emplace(key, newSpriteSheet);
	}
}

#pragma endregion

#pragma region Sounds

void ResourceManager::PlaySoundEffect(const std::string name) const
{
	std::unordered_map<const std::string, SoundEffect*>::const_iterator cIt = m_pSoundEffects.find(name);
	if (cIt != m_pSoundEffects.cend())
		cIt->second->Play(0);
}

void ResourceManager::PlayMusic(std::string name)
{
	if (m_CurrentMusic == name && SoundStream::IsPlaying())
		return;

	std::unordered_map<const std::string, const SoundStream*>::const_iterator cIt = m_pMusics.find(name);
	if (cIt != m_pMusics.cend())
	{
		SoundStream::Stop();
		cIt->second->Play(true);
		m_CurrentMusic = name;
	}
}

void ResourceManager::UpdateMusicVolume(unsigned int newValue)
{
	SoundStream::SetVolume(newValue);
}

void ResourceManager::UpdateEffectVolume(unsigned int newValue)
{
	for (std::pair<std::string, SoundEffect*> soundEffectPair : m_pSoundEffects)
		soundEffectPair.second->SetVolume(newValue);
}

void ResourceManager::LoadSoundEffects(std::ifstream& ifs, const std::string& xPathQuery)
{
	ifs.seekg(0);
	std::vector<std::string> elements = utils::GetXmlElements(ifs, xPathQuery);

	for (std::string& elem : elements)
	{
		m_pSoundEffects.emplace(utils::GetXmlAttributeValue("Name", elem), new SoundEffect(m_SoundsRootDirectory + utils::GetXmlAttributeValue("Path", elem)));
	}
}

void ResourceManager::LoadMusics(std::ifstream& ifs, const std::string& xPathQuery)
{
	ifs.seekg(0);
	std::vector<std::string> elements = utils::GetXmlElements(ifs, xPathQuery);

	for (std::string& elem : elements)
	{
		m_pMusics.emplace(utils::GetXmlAttributeValue("Name", elem), new SoundStream(m_SoundsRootDirectory + utils::GetXmlAttributeValue("Path", elem)));
	}
}

#pragma endregion