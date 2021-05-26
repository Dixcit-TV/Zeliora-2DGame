#include "pch.h"
#include "Loot.h"
#include <cassert>

Loot::Loot(const Object* pObject, const Point2f& centerPos)
	: m_pObject{ pObject }
	, m_CenterPos{ centerPos }
{}

Loot::Loot(Loot&& other) noexcept
	: m_pObject{ other.m_pObject }
	, m_CenterPos{ other.m_CenterPos }
{
	other.m_pObject = nullptr;
}

Loot& Loot::operator =(Loot&& other) noexcept
{
	assert(this != &other);

	delete m_pObject;
	m_pObject = other.m_pObject;
	m_CenterPos = other.m_CenterPos;
	other.m_pObject = nullptr;

	return *this;
}

Loot::~Loot()
{
	delete m_pObject;
}

const Object* Loot::ReleaseObject()
{
	const Object* ptemp{ m_pObject };
	m_pObject = nullptr;
	return ptemp;
}

const Point2f& Loot::GetPos() const
{
	return m_CenterPos;
}

bool Loot::IsPickedUp() const
{
	return !m_pObject;
}

void Loot::Draw() const
{
	m_pObject->Draw(m_CenterPos);
}