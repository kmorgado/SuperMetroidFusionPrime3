///////////////////////////////////////////////////////////////////////
//	File	:	"CMessages.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains all of the messages I want in my application.
///////////////////////////////////////////////////////////////////////

#pragma once

#include "CBase.h"

typedef unsigned int MESSAGEID;

enum eGAME_MESSAGES { eGM_COLLISION = 0, eGM_NUM_MESSAGES, MSG_DESTROY_BULLET, MSG_CREATE_BULLET,
						MSG_DESTROY_ENEMY, MSG_CREATE_ENEMY, MSG_DESTROY_HERO, MSG_CREATE_HERO};

class CBaseMessage
{
	protected:
		MESSAGEID m_Msg;				//	The Message type.

	public:
		CBaseMessage() {}
		CBaseMessage(MESSAGEID msg) : m_Msg(msg) {}
		virtual ~CBaseMessage() {}

		//	Accessors.
		inline void			SetMessageID(MESSAGEID msg) { m_Msg = msg;  }
		inline MESSAGEID	GetMessageID(void)			{ return m_Msg; }
};


//////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: Collision Message
//
//  Purpose: Collision Message
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CCollisionMessage : public CBaseMessage
{
	private:
		//	Paramaters that are specific to this message.
		CBase *m_pCollider1;			//	Pointer to first object.
		CBase *m_pCollider2;			//	Pointer to second object.

	public:
		CCollisionMessage() { m_pCollider1 = m_pCollider2 = 0; }
		CCollisionMessage(MESSAGEID msg, CBase *pCol1, CBase *pCol2) :
			CBaseMessage(msg), m_pCollider1(pCol1), m_pCollider2(pCol2) {}
		~CCollisionMessage() {}

		inline CBase *GetCollider1(void)	{ return m_pCollider1; }
		inline CBase *GetCollider2(void)	{ return m_pCollider2; }

		inline void SetColliders(CBase *pCol1, CBase *pCol2)
		{
			m_pCollider1 = pCol1;
			m_pCollider2 = pCol2;
		}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: Destroy Bullet Message
//
//  Purpose: Destroying of the bullet
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CDestroyBulletMessage : public CBaseMessage
{
private:
 void * m_pParam;

public:
	CDestroyBulletMessage(void * pTemp) { SetMessageID(MSG_DESTROY_BULLET); m_pParam = pTemp;};
	~CDestroyBulletMessage() {};
	inline void * GetParam(void) { return m_pParam;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: Create Bullet Message
//
//  Purpose: Creating of the bullet
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CCreateBulletMessage : public CBaseMessage
{
private:
	void * m_pParam;

public:
	CCreateBulletMessage(void * pTemp) { SetMessageID(MSG_CREATE_BULLET); m_pParam = pTemp;};
	~CCreateBulletMessage() {};
	inline void * GetParam(void) { return m_pParam;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: Destroy Enemy Message
//
//  Purpose: Destroying of the enemy
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CDestroyEnemyMessage : public CBaseMessage
{
private:
	void * m_pParam;

public:
	CDestroyEnemyMessage(void * pTemp) { SetMessageID(MSG_DESTROY_ENEMY); m_pParam = pTemp;};
	~CDestroyEnemyMessage() {};
	inline void * GetParam(void) { return m_pParam;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: Create Enemy Message
//
//  Purpose: Creating of the enemy
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CCreateEnemyMessage : public CBaseMessage
{
private:
	void * m_pParam;

public:
	CCreateEnemyMessage(void * pTemp) { SetMessageID(MSG_CREATE_ENEMY); m_pParam = pTemp;};
	~CCreateEnemyMessage() {};
	inline void * GetParam(void) { return m_pParam;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: Destroy Hero Message
//
//  Purpose: Destroying of the hero
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CDestroyHeroMessage : public CBaseMessage
{
private:
	void * m_pParam;

public:
	CDestroyHeroMessage(void * pTemp) { SetMessageID(MSG_DESTROY_HERO); m_pParam = pTemp;};
	~CDestroyHeroMessage() {};
	inline void * GetParam(void) { return m_pParam;}
};

//////////////////////////////////////////////////////////////////////////////////////////////////////
// Class: Create Hero Message
//
//  Purpose: Creating of the hero
//////////////////////////////////////////////////////////////////////////////////////////////////////
class CCreateHeroMessage : public CBaseMessage
{
private:
	void * m_pParam;

public:
	CCreateHeroMessage(void * pTemp) { SetMessageID(MSG_CREATE_HERO); m_pParam = pTemp;};
	~CCreateHeroMessage() {};
	inline void * GetParam(void) { return m_pParam;}
};

