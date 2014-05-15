///////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_Dispatcher.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Handles all the events in the game.
///////////////////////////////////////////////////////////////////////

#pragma once

#include <list>
#include <map>
using std::multimap;
using std::pair;
using std::list;

#include "CEvent.h"
#include "IListener.h"

class CSGD_Dispatcher
{
	private:
		//	Our Client List.  Classes that can receive information.
		multimap<EVENTID, IListener *>			m_Clients;

		//	Events waiting to be processed every single frame.
		list<CEvent>							m_Events;

		//	DispatchEvent : Sends an event to all the clients that
		//					are registered to receive them.
		void DispatchEvent(CEvent *pEvent);

		static CSGD_Dispatcher *m_pInstance;

		CSGD_Dispatcher(){};
		CSGD_Dispatcher(const CSGD_Dispatcher &ref);
		CSGD_Dispatcher &operator = (const CSGD_Dispatcher &ref);

	public:
		~CSGD_Dispatcher() { }
		static CSGD_Dispatcher *GetInstance(void);
		static void DeleteInstance(void);

		//	Registers a client to receive events from the dispatcher.
		void RegisterClient(EVENTID eventID, IListener *pClient);

		void UNRegisterClient(EVENTID eventID, IListener *pClient);

		//	Sends an event to the dispatcher to be processed.
		void SendEvent(EVENTID eventID, void *pParam);

		//	Processes all events.
		void ProcessEvents(void);
};