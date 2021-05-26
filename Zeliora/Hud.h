#pragma once

class Hero;
class Texture;

class Hud final
{
public:
	explicit Hud(Hero** pPlayer);
	Hud(const Hud& other) = delete;
	Hud(Hud&& other) noexcept = delete;
	Hud* operator=(const Hud& other) = delete;
	Hud* operator=(Hud&& other) = delete;
	~Hud();

	void Update();
	void Draw() const;

private:
	Hero** m_pPlayer;
	Texture* m_pHealthIcon;
	Texture* m_pLevelTexture;
	int m_PlayerLevel;
};

