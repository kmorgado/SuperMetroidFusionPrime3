///////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_MessageSystem.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the Messaging System for use in my application.
///////////////////////////////////////////////////////////////////////

#include "CSGD_MessageSystem.h"

CSGD_MessageSystem *CSGD_MessageSystem::m_pInstance = 0;

void CSGD_MessageSystem::SetMsgProc(MESSAGEPROC msgProc)
{
	//	Make sure the function pointer is valid.
	if (!msgProc)	return;

	m_MsgProc = msgProc;
}

void CSGD_MessageSystem::SendMsg(CBaseMessage *pMsg)
{
	//	Make sure the messsage is valid.
	if (!pMsg)	return;

	//	Add the message to the queue.
	m_MsgQueue.push(pMsg);
}

void CSGD_MessageSystem::PostMsg(CBaseMessage *pMsg)
{
	//	Make sure the messages is valid.
	if (!pMsg)	return;

	//	Make sure the message proc was set.
	if (!m_MsgProc)	return;

	//	Process the messsage immediately.
	m_MsgProc(pMsg);

	//	Get Rid of the memory.
	delete pMsg;
}

void CSGD_MessageSystem::ProcessMessages(void)
{
	//	Error check.
	if (!m_MsgProc)	return;

	//	Go through our queue and proccess the messages that are waiting.
	//	We will delete them after they have been processed.
	while (!m_MsgQueue.empty())
	{
		m_MsgProc(m_MsgQueue.front());		//	Process the message in the front.
		delete m_MsgQueue.front();			//	We don't need the message anymore.
		m_MsgQueue.pop();					//	Get rid of the first element.
	}
}

CSGD_MessageSystem* CSGD_MessageSystem::GetIntance()
{
	if(!m_pInstance)
		m_pInstance = new CSGD_MessageSystem;

	return m_pInstance;
}

void CSGD_MessageSystem::DeleteInstance( void )
{
	if (m_pInstance)			
	{
		delete m_pInstance;		
		m_pInstance = 0;		
	}
}