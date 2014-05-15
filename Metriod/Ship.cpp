#include "Hero.h"

CHero::CHero(void)
{
 m_dSpeed = 0;
 m_dRotation = 0;
 SetX(64);
 SetY(64);
 SetType(1);
}

CHero::~CHero(void)
{
}

bool CHero::Update(double dElapsedTime)
{
	CSGD_DirectInput	*pDI = CSGD_DirectInput::GetInstance();
	tVector2D			g_vctObjVel;
	g_vctObjVel.dX = 0;
	g_vctObjVel.dY = -1;

	if(pDI->GetKey(DIK_UP))
	{
		m_dSpeed += (50.0 * dElapsedTime);
	}
	if(pDI->GetKey(DIK_DOWN))
	{
		m_dSpeed -= (50.0 * dElapsedTime);
	}
	if(pDI->GetKey(DIK_LEFT))
	{
		m_dRotation += (-PI * dElapsedTime);
	}
	if(pDI->GetKey(DIK_RIGHT))
	{
		m_dRotation += (PI * dElapsedTime);
	}
	if(pDI->GetBufferedKey(DIK_SPACE))
	{
		CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
		CCreateBulletMessage *pMsg = new CCreateBulletMessage(this);
		pTemp->SendMsg(pMsg);
	}
	g_vctObjVel = Vector2DRotate(g_vctObjVel,m_dRotation);
	g_vctObjVel.dX *= m_dSpeed;
	g_vctObjVel.dY *= m_dSpeed;

	CBase::SetVelX((double)g_vctObjVel.dX);
	CBase::SetVelY((double)g_vctObjVel.dY);
	CBase::Update((double)dElapsedTime);
	
	return true;
}

bool CHero::Render(void)
{
	RECT pTemp;
	pTemp.bottom = 128;
	pTemp.top = 32;
	pTemp.left = 42;
	pTemp.right = 100;
	CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() - 29,GetY() - 48,1,1,&pTemp,29,48,m_dRotation);

	return true;
}

double CHero::GetRot()
{
	return m_dRotation;
}

void CHero::SetRot(double dRot)
{
	m_dRotation = dRot;
}

double CHero::GetSpeed(void)
{
	return m_dSpeed;
}

void CHero::SetSpeed(double dSpeed)
{
	m_dSpeed = dSpeed;
}