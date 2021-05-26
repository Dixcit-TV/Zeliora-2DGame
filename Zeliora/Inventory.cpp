#include "pch.h"
#include "Inventory.h"
#include "Object.h"
#include <cassert>

Inventory::Inventory(int maxSize)
	: m_pObjects{}
	, m_MaxSize{ maxSize }
{
	m_pObjects.reserve(maxSize);
}

Inventory::~Inventory()
{
	for (const Object* obj : m_pObjects)
		delete obj;
}

const Object* Inventory::operator [](size_t idx) const
{
	assert(m_pObjects.size() != 0 && idx < m_pObjects.size());
	return m_pObjects[idx];
}

size_t Inventory::Capacity() const
{
	return m_pObjects.capacity();
}

size_t Inventory::Size() const
{
	return m_pObjects.size();
}

void Inventory::AddObject(const Object* pObject)
{
	m_pObjects.push_back(pObject);
}

const Object* Inventory::ReplaceObject(size_t idx, const Object* pNewObject)
{
	assert(m_pObjects.size() != 0 && idx < m_pObjects.size());

	const Object* pOldObject{ m_pObjects[idx] };
	m_pObjects[idx] = pNewObject;
	return pOldObject;
}