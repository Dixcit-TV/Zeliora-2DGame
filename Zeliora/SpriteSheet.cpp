#include "pch.h"
#include "SpriteSheet.h"
#include "Texture.h"

SpriteSheet::SpriteSheet(const std::string& path, int totalColumns, int totalRows)
	: m_pTexture{ new Texture(path) }
	, m_TotalColumns{ totalColumns }
	, m_TotalRows{ totalRows }
{}

SpriteSheet::~SpriteSheet()
{
	delete m_pTexture;
}

float SpriteSheet::GetFrameWidth() const
{
	return m_pTexture->GetWidth() / m_TotalColumns;
}

float SpriteSheet::GetFrameHeight() const
{
	return m_pTexture->GetHeight() / m_TotalRows;
}

void SpriteSheet::Draw(Rectf destRect, Rectf srcRect) const
{
	m_pTexture->Draw(destRect, srcRect);
}
