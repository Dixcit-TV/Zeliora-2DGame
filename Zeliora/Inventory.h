#pragma once
#include <vector>

class Object;

class Inventory final
{
public:
	explicit Inventory(int maxSize = 300);
	Inventory(const Inventory& other) = delete;
	Inventory(Inventory&& other) noexcept = delete;
	Inventory& operator =(const Inventory& other) = delete;
	Inventory& operator =(Inventory&& other) noexcept = delete;
	~Inventory();

	const Object* operator [](size_t idx) const;
	size_t Capacity() const;
	size_t Size() const;
	void AddObject(const Object* pObject);
	const Object* ReplaceObject(size_t idx, const Object* pNewObject);

private:
	std::vector<const Object*> m_pObjects;
	int m_MaxSize;
};

