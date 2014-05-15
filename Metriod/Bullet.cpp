///////////////////////////////////////////////////////////////////////
//	File	:	"Bullet.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the inforamtion for any bullet in my game.
///////////////////////////////////////////////////////////////////////

#include "Bullet.h"

CBullet::CBullet(void)
{
	SetType(0);
}

CBullet::~CBullet(void)
{

}

bool CBullet::Update(double dElapsedTime)
{
	if(GetX() < 0 || GetX() > 800 || GetY() < 90 || GetY() > 600)
	{
		CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
		CDestroyBulletMessage *pMsg = new CDestroyBulletMessage(this);
		pTemp->SendMsg(pMsg);
	}
	CBase::Update((double)dElapsedTime);

	return true;
}

bool CBullet::Render( void )
{
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() - (GetWidth() /2),GetY() - (GetHeight() /2), 1, 1 ,0, (GetWidth() /2), (GetHeight() /2),80);

	return true;
}

void CBullet::HandleEvent( CEvent *pEvent )
{
	if(pEvent->GetParam() == (CBase *)this)
	{
		if (pEvent->GetEventID() == "Collision")
		{
			CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
			CDestroyBulletMessage *pMsg = new CDestroyBulletMessage(this);
			pTemp->SendMsg(pMsg);
		}
	}
}