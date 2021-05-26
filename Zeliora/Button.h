#pragma once
#include <functional>
#include "Texture.h"
#include "utils.h"
#include "ResourceManager.h"

template <typename RetType, typename TargetType>
class Button final
{
public:
	Button(std::function<RetType(TargetType*)> function, TargetType* pTarget, const std::string& text, const Point2f& botLeft);
	Button(std::function<RetType(TargetType*)> function, TargetType* pTarget, Texture* texture, Texture* textureHover, const Point2f& botLeft);
	Button(const Button& other) = delete;
	Button& operator=(const Button& other) = delete;
	Button(Button&& other) noexcept;
	Button& operator=(Button&& other) noexcept = delete;
	~Button();

	RetType operator()();
	Point2f GetBotLeft() const;
	float GetWidth() const;
	float GetHeight() const;
	bool IsClicked(const Point2f& mousePos);
	bool IsFocused(const Point2f& mousePos);
	void Draw() const;

private:
	std::function<RetType(TargetType*)> m_Function;
	TargetType* m_pTarget;

	Texture* m_pTitleTexture;
	Texture* m_pTitleTextureFocus;
	Point2f m_BotLeft;
	bool m_IsFocused;
};

template <typename RetType, typename TargetType>
Button<RetType, TargetType>::Button(std::function<RetType(TargetType*)> function, TargetType* pTarget, const std::string& text, const Point2f& botLeft)
	: Button(
		function
		, pTarget
		, new Texture(text, ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::classic))
		, new Texture(text, ResourceManager::GetInstance()->GetFont(ResourceManager::FontSize::size_36), ResourceManager::GetInstance()->GetFontColor(ResourceManager::FontCategory::hover))
		, botLeft)
{}

template <typename RetType, typename TargetType>
Button<RetType, TargetType>::Button(std::function<RetType(TargetType*)> function, TargetType* pTarget, Texture* pTextureNormal, Texture* pTextureHover, const Point2f& botLeft)
	: m_Function{ function }
	, m_pTarget{ pTarget }
	, m_pTitleTexture{ pTextureNormal }
	, m_pTitleTextureFocus{ pTextureHover }
	, m_BotLeft{ botLeft }
	, m_IsFocused{}
{}

template <typename RetType, typename TargetType>
Button<RetType, TargetType>::Button(Button&& other) noexcept
	: m_Function{ other.m_Function }
	, m_pTarget{ other.m_pTarget }
	, m_pTitleTexture{ other.m_pTitleTexture }
	, m_pTitleTextureFocus{ other.m_pTitleTextureFocus }
	, m_BotLeft{ other.m_BotLeft }
	, m_IsFocused{ other.m_IsFocused }
{
	other.m_pTitleTexture = other.m_pTitleTextureFocus = nullptr;
}

template <typename RetType, typename TargetType>
Button<RetType, TargetType>::~Button()
{
	delete m_pTitleTexture;
	delete m_pTitleTextureFocus;
}

template <typename RetType, typename TargetType>
RetType Button<RetType, TargetType>::operator()()
{
	return m_Function(m_pTarget);
}

template <typename RetType, typename TargetType>
Point2f Button<RetType, TargetType>::GetBotLeft() const
{
	return m_BotLeft;
}

template <typename RetType, typename TargetType>
float Button<RetType, TargetType>::GetWidth() const
{
	return m_pTitleTexture->GetWidth();
}

template <typename RetType, typename TargetType>
float Button<RetType, TargetType>::GetHeight() const
{
	return m_pTitleTexture->GetHeight();
}

template <typename RetType, typename TargetType>
bool Button<RetType, TargetType>::IsClicked(const Point2f& mousePos)
{
	return utils::IsPointInRect(mousePos, Rectf{ m_BotLeft.x, m_BotLeft.y, m_pTitleTexture->GetWidth(), m_pTitleTexture->GetHeight() });
}

template <typename RetType, typename TargetType>
bool Button<RetType, TargetType>::IsFocused(const Point2f& mousePos)
{
	bool isHovered{ utils::IsPointInRect(mousePos, Rectf{ m_BotLeft.x, m_BotLeft.y, m_pTitleTexture->GetWidth(), m_pTitleTexture->GetHeight() }) };
	if (isHovered && !m_IsFocused)
	{
		ResourceManager::GetInstance()->PlaySoundEffect("MenuNavigation");
	}

	return m_IsFocused = isHovered;
}

template <typename RetType, typename TargetType>
void Button<RetType, TargetType>::Draw() const
{
	if (m_IsFocused && m_pTitleTextureFocus)
		m_pTitleTextureFocus->Draw(m_BotLeft);
	else
		m_pTitleTexture->Draw(m_BotLeft);
}