///////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_Dispatcher.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Handles all the events in the game.
///////////////////////////////////////////////////////////////////////

#include "CSGD_Dispatcher.h"

CSGD_Dispatcher *CSGD_Dispatcher::m_pInstance = 0;

CSGD_Dispatcher* CSGD_Dispatcher::GetInstance(void)
{
	if(!m_pInstance)
		m_pInstance = new CSGD_Dispatcher;

	return m_pInstance;
}

void CSGD_Dispatcher::DeleteInstance( void )
{
	if (m_pInstance)			
	{
		delete m_pInstance;		
		m_pInstance = 0;		
	}
}

//	Register Client.
void CSGD_Dispatcher::RegisterClient(EVENTID eventID, IListener *pClient)
{
	//	Error Checking.
	if (!pClient)	return;

	//	Push (Register) our client into our Multimap.
	m_Clients.insert(make_pair(eventID, pClient));
}

void CSGD_Dispatcher::UNRegisterClient(EVENTID eventID, IListener *pClient)
{
	multimap<EVENTID, IListener *>::iterator vIter = m_Clients.begin();

	while (vIter != m_Clients.end())
	{
		if((*vIter).second == pClient)
		{
				vIter = m_Clients.erase(vIter);
				continue;
		}
		else
			vIter++;
	}
}

void CSGD_Dispatcher::DispatchEvent(CEvent *pEvent)
{
	//	Make an iterator that will iterate through all
	//	of our clients that should receive this event.
	pair<multimap<EVENTID, IListener *>::iterator,
		 multimap<EVENTID, IListener *>::iterator> range;

	//	Find all clients that should get this evevnt.
	range = m_Clients.equal_range(pEvent->GetEventID());

	//	Go through my list of clients that can receive this event.
	for (multimap<EVENTID, IListener *>::iterator iter = range.first;
					iter != range.second; iter++)
	{
		//	Pass the event to this client.
		(*iter).second->HandleEvent(pEvent);
	}
}

void CSGD_Dispatcher::SendEvent(EVENTID eventID, void *pParam)
{
	//	Push the event onto the event list.
	CEvent newEvent;
	newEvent.SetEventParams(eventID, pParam);
	m_Events.push_back(newEvent);
}

void CSGD_Dispatcher::ProcessEvents(void)
{
	//	Go through all the events in the event list and dispatch
	//	them to the clients.
	while (m_Events.size())
	{
		DispatchEvent(&m_Events.front());
		m_Events.pop_front();
	}
}