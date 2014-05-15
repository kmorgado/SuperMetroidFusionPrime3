///////////////////////////////////////////////////////////////////////
//	File	:	"Hero.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the inforamtion for the Hero of my game.
///////////////////////////////////////////////////////////////////////
#include "Hero.h"

CHero::CHero(void)
{
	m_dSpeed = 0;
	SetType(1);
	m_Left.bottom = 64;
	m_Left.top = 0;
	m_Left.right = 394;
	m_Left.left = 344;
	m_Right.bottom = 126;
	m_Right.top = 66;
	m_Right.right = 394;
	m_Right.left = 344;
	m_n1Up = 1000;
}

CHero::~CHero(void)
{
}

bool CHero::Update(double dElapsedTime)
{
	CSGD_DirectInput	*pDI = CSGD_DirectInput::GetInstance();
	CSGD_WaveManager	*pWM = CSGD_WaveManager::GetInstance();
	tVector2D			g_vctObjVel;
	g_vctObjVel.dX = -1;
	g_vctObjVel.dY = 0;
	m_nchargesound = pWM->LoadWave("Resources/Sounds/KeM_Charge.wav");
	m_nwavesound = pWM->LoadWave("Resources/Sounds/KeM_Wave.wav");
	m_nicesound = pWM->LoadWave("Resources/Sounds/KeM_Ice.wav");
	m_nplasmasound = pWM->LoadWave("Resources/Sounds/KeM_Plasma.wav");
	m_nspazersound = pWM->LoadWave("Resources/Sounds/KeM_Spazer.wav");
	m_n1Upsound = pWM->LoadWave("Resources/Sounds/KeM_1up.wav");

	//Cheat codes
	if (pDI->GetBufferedKey(DIK_1))
	{
		CStats::GetInstance()->SetHealth(999);
	}
	if (pDI->GetBufferedKey(DIK_2))
	{
		bool temp = CStats::GetInstance()->GetSlow();
		temp = !temp;
		CStats::GetInstance()->SetSlow(temp);
	}
	if (pDI->GetBufferedKey(DIK_3))
	{
		bool temp = CStats::GetInstance()->GetFast();
		temp = !temp;
		CStats::GetInstance()->SetFast(temp);
	}


	if(pDI->GetKey(DIK_ESCAPE))
	{
		SetX(400);
		m_dSpeed = 0;
	}

	if(pDI->GetKey(DIK_LEFT))
	{
		if(CStats::GetInstance()->GetCharacter() == 0)
			m_dSpeed += 250 * dElapsedTime;
		else if(CStats::GetInstance()->GetCharacter() == 1)
			m_dSpeed += 50 * dElapsedTime;
		else if(CStats::GetInstance()->GetCharacter() == 2)
			m_dSpeed += 350 * dElapsedTime;

		static double left = 0;
		left += dElapsedTime;
		if(left >= 0.1)
		{
			left = 0;
				if(m_Left.left <= 0)
				{
					m_Left.left = 344;
					m_Left.right = 394;
				}
				m_Left.left -= 50;
				m_Left.right -= 50;
		}

	}
	else
	{
		if(m_dSpeed > 0)
		{
			m_dSpeed -= 300 * dElapsedTime;
			if(m_dSpeed < 0)
				m_dSpeed = 0;
		}
	}

	if(pDI->GetKey(DIK_RIGHT))
	{
		if(CStats::GetInstance()->GetCharacter() == 0)
			m_dSpeed -= 250 * dElapsedTime;
		else if(CStats::GetInstance()->GetCharacter() == 1)
			m_dSpeed -= 50 * dElapsedTime;
		else if(CStats::GetInstance()->GetCharacter() == 2)
			m_dSpeed -= 350 * dElapsedTime;

		static double right = 0;
		right += dElapsedTime;
		if(right >= 0.1)
		{
			right = 0;
			if(m_Right.left <= 0)
			{
				m_Right.left = 344;
				m_Right.right = 394;
			}
			m_Right.left -= 50;
			m_Right.right -= 50;
		}
	}
	else
	{
		if(m_dSpeed < 0)
		{
			m_dSpeed += 300 * dElapsedTime;
			if(m_dSpeed > 0)
				m_dSpeed = 0;
		}
	}

	if(this->GetX() <=0 )
	{
		m_dSpeed = 0;
		SetX(5);
	}
	if(this->GetX() >=800 )
	{
		m_dSpeed = 0;
		SetX(795);
	}

	if(GetX() <= 399)
	{
		pWM->SetPan(m_nchargesound,-1000);
		pWM->SetPan(m_nwavesound,-1000);
		pWM->SetPan(m_nicesound,-1000);
		pWM->SetPan(m_nplasmasound,-1000);
		pWM->SetPan(m_nspazersound,-1000);
	}
	if(GetX() >= 401)
	{
		pWM->SetPan(m_nchargesound,1000);
		pWM->SetPan(m_nwavesound,1000);
		pWM->SetPan(m_nicesound,1000);
		pWM->SetPan(m_nplasmasound,1000);
		pWM->SetPan(m_nspazersound,1000);
	}
	if(pDI->GetBufferedKey(DIK_SPACE))
	{
		if(CStats::GetInstance()->GetBType() == 0)
			pWM->Play(m_nchargesound);
		else if(CStats::GetInstance()->GetBType() == 1)
			pWM->Play(m_nwavesound);
		else if(CStats::GetInstance()->GetBType() == 2)
			pWM->Play(m_nicesound);
		else if(CStats::GetInstance()->GetBType() == 3)
			pWM->Play(m_nplasmasound);
		else if(CStats::GetInstance()->GetBType() == 4)
			pWM->Play(m_nspazersound);

		CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
		CCreateBulletMessage *pMsg = new CCreateBulletMessage(this);
		pTemp->SendMsg(pMsg);
	}

 	if(pDI->GetBufferedKey(DIK_TAB))
 	{
		int temp;
		temp = CStats::GetInstance()->GetBType();
		temp++;
		if(temp == 5)
			temp = 0;
		CStats::GetInstance()->SetBType(temp);
 	}

	static double spawn = 0;
	spawn += dElapsedTime;
	if(CStats::GetInstance()->GetDifficulty() == 0)
	{
		if(spawn >= 4)
		{
			int temp = CStats::GetInstance()->GetHealth();
			temp -= 4;
			CStats::GetInstance()->SetHealth(temp);
			spawn = 0;
			CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
			CCreateEnemyMessage *pMsg = new CCreateEnemyMessage(this);
			pTemp->SendMsg(pMsg);
		}
	}
	else if(CStats::GetInstance()->GetDifficulty() == 1)
	{
		if(spawn >= 2)
		{

			int temp = CStats::GetInstance()->GetHealth();
			temp -= 6;
			CStats::GetInstance()->SetHealth(temp);
			spawn = 0;
			CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
			CCreateEnemyMessage *pMsg = new CCreateEnemyMessage(this);
			pTemp->SendMsg(pMsg);
		}
	}
	else if(CStats::GetInstance()->GetDifficulty() == 2)
	{
		if(spawn >= 1)
		{
		int temp = CStats::GetInstance()->GetHealth();
		temp -= 8;
		CStats::GetInstance()->SetHealth(temp);
		spawn = 0;
		CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
		CCreateEnemyMessage *pMsg = new CCreateEnemyMessage(this);
		pTemp->SendMsg(pMsg);
		}
	}

	if(CStats::GetInstance()->GetScore() >= m_n1Up)
	{
		m_n1Up += 1000;
		pWM->Play(m_n1Upsound);
		int temp = CStats::GetInstance()->GetScore();
		temp = CStats::GetInstance()->GetLives();
		temp++;
		CStats::GetInstance()->SetLives(temp);
	}

	g_vctObjVel.dX *= m_dSpeed;
	g_vctObjVel.dY *= m_dSpeed;

	CBase::SetVelX((float)g_vctObjVel.dX);
	CBase::SetVelY((float)g_vctObjVel.dY);
	CBase::Update((double)dElapsedTime);

	return true;
}

bool CHero::Render(void)
{
	if(GetSpeed() == 0)
	{
		RECT pTemp;
		pTemp.bottom = 137;
		pTemp.top = 197;
		pTemp.left = 0;
		pTemp.right = 40;
		if(CStats::GetInstance()->GetCharacter() == 0)
		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() -38,GetY() +110,2,2,&pTemp,29,48);

		if(CStats::GetInstance()->GetCharacter() == 1)
			CSGD_TextureManager::GetInstance()->Draw(GetVaria(),GetX() -38,GetY() +110,2,2,&pTemp,29,48);

		if(CStats::GetInstance()->GetCharacter() == 2)
			CSGD_TextureManager::GetInstance()->Draw(GetGravity(),GetX() -38,GetY() +110,2,2,&pTemp,29,48);
	}
	if(GetSpeed() > 0)
	{
		if(CStats::GetInstance()->GetCharacter() == 0)
		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() -38,GetY() -20,2,2,&m_Left,29,48);

		if(CStats::GetInstance()->GetCharacter() == 1)
			CSGD_TextureManager::GetInstance()->Draw(GetVaria(),GetX() -38,GetY() -20,2,2,&m_Left,29,48);

		if(CStats::GetInstance()->GetCharacter() == 2)
			CSGD_TextureManager::GetInstance()->Draw(GetGravity(),GetX() -38,GetY() -20,2,2,&m_Left,29,48);
	}
	if(GetSpeed() < 0)
	{
		if(CStats::GetInstance()->GetCharacter() == 0)
		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() -64,GetY() - 8,2,2,&m_Right,29,48);

		if(CStats::GetInstance()->GetCharacter() == 1)
			CSGD_TextureManager::GetInstance()->Draw(GetVaria(),GetX() -64,GetY() - 8,2,2,&m_Right,29,48);

		if(CStats::GetInstance()->GetCharacter() == 2)
			CSGD_TextureManager::GetInstance()->Draw(GetGravity(),GetX() -64,GetY() - 8,2,2,&m_Right,29,48);
	}

	return true;
}

double CHero::GetSpeed(void)
{
	return m_dSpeed;
}
int CHero::GetVaria(void)
{
	return m_nVaria;
}
int CHero::GetGravity(void)
{
	return m_nGravity;
}

void CHero::SetSpeed(double dSpeed)
{
	m_dSpeed = dSpeed;
}

void CHero::SetVaria(int nVaria)
{
	m_nVaria = nVaria;
}
void CHero::SetGravity(int nGravity)
{
	m_nGravity= nGravity;
}
