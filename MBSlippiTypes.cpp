#include "MBSlippiTypes.h"

namespace MBSlippi
{
	Event::Event(Event const& EventToCopy)
	{
		m_Data = EventToCopy.m_Data->Copy();
	}
	Event::Event(Event&& EventToSteal) noexcept
	{
		m_Data = std::move(EventToSteal.m_Data);
	}
	Event& Event::operator=(Event EventToSteal)
	{
		m_Data = std::move(EventToSteal.m_Data);
		return(*this);
	}
	Event::Event(std::unique_ptr<EventData> InitialData)
	{
		m_Data = std::move(InitialData);
	}
	EventType Event::GetType() const
	{
		return(m_Data->GetType());
	}
}