#include "pch.h"
#include "Hud.h"
#include "Hero.h"
#include "utils.h"
#include "GameSettings.h"
#include "Texture.h"
#include "ResourceManager.h"

Hud::Hud(Hero** pPlayer)
	: m_pPlayer{ pPlayer }
	, m_pHealthIcon{ new Texture("Resources/SpriteSheets/Misc/heart.png") }
	, m_pLevelTexture{ nullptr }
	, m_PlayerLevel{ -1 }
{}

Hud::~Hud()
{
	delete m_pHealthIcon;
	delete m_pLevelTexture;
}

void Hud::Update()
{
	if (!m_pLevelTexture || m_PlayerLevel != (*m_pPlayer)->GetLevel())
	{
		m_PlayerLevel = (*m_pPlayer)->GetLevel();
		delete m_pLevelTexture;
		m_pLevelTexture = new Texture("Lvl" + std::to_string(m_PlayerLevel), ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic));
	}
}

void Hud::Draw() const
{
	float const healthBarWidth{ 300.f };
	utils::SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.f });
	utils::FillRect(36.f, GameSettings::GetInstance()->GetGameWindow().height - 50.f, healthBarWidth, 30.f);

	utils::SetColor(Color4f{ 1.f, 0.2f, 0.2f, 1.f });
	utils::FillRect(36.f, GameSettings::GetInstance()->GetGameWindow().height - 45.f, healthBarWidth * (*m_pPlayer)->GetHealthRatio(), 20.f);

	m_pHealthIcon->Draw(Point2f{ 20.f, GameSettings::GetInstance()->GetGameWindow().height - 50.f });

	float const expBarWidth{ 300.f };
	utils::SetColor(Color4f{ 0.5f, 0.5f, 0.5f, 1.f });
	utils::FillRect(20.f, GameSettings::GetInstance()->GetGameWindow().height - 90.f, expBarWidth, 30.f);

	utils::SetColor(Color4f{ 0.5f, 0.5f, 1.f, 1.f });
	utils::FillRect(20.f, GameSettings::GetInstance()->GetGameWindow().height - 85.f, expBarWidth * (*m_pPlayer)->GetExpToLevelRatio(), 20.f);

	m_pLevelTexture->Draw(Point2f{ 20.f, GameSettings::GetInstance()->GetGameWindow().height - 90.f });
}