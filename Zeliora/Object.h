#pragma once
#include "enum.h"
#include "structs.h"

class SpriteTile;
class Texture;

class Object
{
public:
	enum class ObjectType
	{
		weapon, armor
	};

	virtual ~Object();

	const SpriteTile* GetSpriteTile() const;
	ObjectType GetType() const;
	void Draw(const Point2f& centerPoint, float size = 0.f) const;
	void DrawWithDescription(const Point2f& botLeft, float size = 0.f) const;

protected:
	explicit Object(const SpriteTile* pSpriteTile, const std::string& name, ObjectType type);

	const SpriteTile* m_pSpriteTile;
	Texture* m_pObjectTextDescription;
	std::string m_Name;
	ObjectType m_Type;

	static std::string GetXmlPath() { return "Resources/Data/Objects.xml"; };

	virtual std::string GetDescriptionText() const = 0;
};

