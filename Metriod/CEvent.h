///////////////////////////////////////////////////////////////////////
//	File	:	"CEvent.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains all event information.
///////////////////////////////////////////////////////////////////////

#pragma once

#include <string>
using std::string;

typedef string EVENTID;

class CEvent
{
	private:
		EVENTID		m_EventID;				//	Name of the Event.
		void		*m_pParam;				//	Parameters you want associated with this event.

	public:
		CEvent() { m_pParam = 0; }
		~CEvent() {}

		////////////////////////////////////////////////////////////////////
		//	Function	:	Set Event Params
		//
		//	Return		:	Sets up the parameters for each event
		////////////////////////////////////////////////////////////////////
		void SetEventParams(EVENTID eventID, void *pParam = 0)
		{
			m_EventID = eventID;
			m_pParam  = pParam;
		}

		////////////////////////////////////////////////////////////////////
		//	Function	:	Modifiers.
		//
		//	Return		:	To modify the specified type.
		////////////////////////////////////////////////////////////////////
		inline void SetEventID(EVENTID eventID) { m_EventID = eventID; }
		inline void SetParam(void *pParam)	    { m_pParam = pParam;   }

		////////////////////////////////////////////////////////////////////
		//	Function	:	Accessors.
		//
		//	Return		:	To return the specified type.
		////////////////////////////////////////////////////////////////////
		inline EVENTID GetEventID(void)			{ return m_EventID;	   }
		inline void    *GetParam(void)			{ return m_pParam;	   }	
};