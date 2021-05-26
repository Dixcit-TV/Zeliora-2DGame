#pragma once

class Texture;

class SpriteSheet
{
public:
	explicit SpriteSheet(const std::string& path, int totalColumns, int totalRows);
	SpriteSheet(const SpriteSheet& other) = delete;
	SpriteSheet(SpriteSheet&& other) noexcept = delete;
	SpriteSheet& operator =(const SpriteSheet& other) = delete;
	SpriteSheet& operator =(SpriteSheet&& other) noexcept = delete;
	~SpriteSheet();

	float GetFrameWidth() const;
	float GetFrameHeight() const;
	void Draw(Rectf destRect, Rectf srcRect) const;

private:
	Texture* m_pTexture;

	const int m_TotalColumns;
	const int m_TotalRows;
};

