///////////////////////////////////////////////////////////////////////
//	File	:	"Enemy.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the inforamtion for any Enemy in my game.
///////////////////////////////////////////////////////////////////////
#include "Enemy.h"

CEnemy::CEnemy(void)
{
	m_ucAmount = 0;
	m_nHealth = 50;
	SetX(200);
	SetY(300);
	SetType(2);
}

CEnemy::~CEnemy(void)
{
}

bool CEnemy::Update( double dElapsedTime )
{
	if(GetX() < -101 || GetX() > 800 || GetY() < 0 || GetY() > 600)
	{
		CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
		CDestroyBulletMessage *pMsg = new CDestroyBulletMessage(this);
		pTemp->SendMsg(pMsg);
	}
	CBase::Update((double)dElapsedTime);

	return true;
}

bool CEnemy::Render(void)
{
	DWORD color = D3DCOLOR_ARGB(255, 255, 255 - m_ucAmount, 255 - m_ucAmount);

	CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() ,GetY(),1,1,0,GetWidth(),GetHeight(),0,color);

	return true;
}

void CEnemy::HandleEvent( CEvent *pEvent )
{
	if(pEvent->GetParam() == (CBase *)this)
	{
		if (pEvent->GetEventID() == "Collision")
		{
			if(GetEType() == CStats::GetInstance()->GetBType())
			{
				SetHealth(0);
				int temp = CStats::GetInstance()->GetScore();
				temp += 100;
				CStats::GetInstance()->SetScore(temp);
			}
			else
			{
				m_nHealth -=2;
				m_ucAmount += 10;
				int temp = CStats::GetInstance()->GetScore();
				temp += 2;
					CStats::GetInstance()->SetScore(temp);
			}

			//CBullet * pTarget = (CBullet*)pEvent->GetParam();

			if(GetHealth() <= 0)
			{
				int temp = CStats::GetInstance()->GetHealth();
				temp +=5;
				CStats::GetInstance()->SetHealth(temp);

				CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
				CDestroyEnemyMessage *pMsg = new CDestroyEnemyMessage(this);
				pTemp->SendMsg(pMsg);
			}
		}
	}
}

int CEnemy::GetHealth(void)
{
	return m_nHealth;
}
int CEnemy::GetEType(void)
{
	return m_nEType;
}

void CEnemy::SetHealth(int nHealth)
{
	m_nHealth = nHealth;
}
void CEnemy::SetEType(int nEType)
{
	m_nEType = nEType;
}