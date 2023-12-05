#pragma once

#include <iostream>

namespace spe
{
	template<typename T>
	class Repository
	{
	protected:
		uint32_t m_HighestLayer;
		uint32_t m_HighestId;

		Repository() { this->m_HighestId = 0; this->m_HighestLayer = 0; }
	public:
		virtual void Add(T* t) = 0;
		virtual void UpdateLayerIndex() = 0;

		virtual uint32_t GetAmount() const = 0;
		virtual T* GetById(uint32_t id) = 0;
		virtual T* GetByName(const std::string& name) = 0;
	};

}

