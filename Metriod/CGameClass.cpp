//////////////////////////////////////////////////////////////////////
//	File	:	"CGameClass.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains CGameClass function definitions.
//////////////////////////////////////////////////////////////////////

#include "CGameClass.h"

//	Initialize the static variable member.
CGameClass *CGameClass::m_pInstance = 0;

//	GetInstance
CGameClass *CGameClass::GetInstance(void)
{
	//	Make sure one hasn't been created yet.
	if (!m_pInstance)
		m_pInstance = new CGameClass;

	//	Return the memory address of the instance.
	return m_pInstance;
}

//	DeleteInstance.
void CGameClass::DeleteInstance(void)
{
	//	Test to see if memory is valid.
	if (m_pInstance)			
	{
		delete m_pInstance;		
		m_pInstance = 0;		
	}
}

CGameClass::CGameClass()
{
	m_pD3D = 0;
	m_pTM  = 0;
	m_pDS  = 0;
	m_pWM  = 0;
	m_pDI  = 0;
	m_bWin = true;
	g_dwStartTime = GetTickCount();		
	g_dwLastStartTime = GetTickCount();	
	g_dFractionOfSecondPassed = 0.0f;
	lvl1 = 1000;
	lvl2 = 2000;
}

CGameClass::~CGameClass()
{

}

bool CGameClass::GameInit(HWND hWnd, HINSTANCE hInstance,
						  int nWidth, int nHeight, bool bWindowed)
{
	//	Get My Instance.
	m_pD3D = CSGD_Direct3D::GetInstance();
	m_pTM  = CSGD_TextureManager::GetInstance();
	m_pDS  = CSGD_DirectSound::GetInstance();
	m_pWM  = CSGD_WaveManager::GetInstance();
	m_pDI  = CSGD_DirectInput::GetInstance();
	m_pOM = CSGD_ObjectManager::GetInstance();
	m_pStats = CStats::GetInstance();
	m_pObFactory  = CSGD_ObjectFactory<string, CBase>::GetInstance();

	//Register Factories
	m_pObFactory->RegisterClassType<CHero>("CHero");
	m_pObFactory->RegisterClassType<CBullet>("CBullet");
	m_pObFactory->RegisterClassType<CEnemy>("CEnemy");
	m_pObFactory->RegisterClassType<CMainMenu>("CMainMenu");
	m_pObFactory->RegisterClassType<CLvl>("CLvl");

	pMainMenu = (CMainMenu*)m_pObFactory->CreateObject("CMainMenu");
	pLvl = (CLvl*)m_pObFactory->CreateObject("CLvl");

	//seed random
	srand(unsigned int(time(0)));

	if (bWindowed)
		m_Flags.TurnOn(GAME_WINDOWED);

	//	Initialize.
	m_pD3D->InitDirect3D(hWnd, nWidth, nHeight, 
		m_Flags.CheckFlag(GAME_WINDOWED) != 0, false);

	//	Init Texture Manager.
	m_pTM->InitTextureManager(m_pD3D->GetDirect3DDevice(), 
		m_pD3D->GetSprite());

	//	Initialize DirectSound.
	m_pDS->InitDirectSound(hWnd);

	//	Initialize Wave Manager.
	m_pWM->InitWaveManager(hWnd, m_pDS->GetDSObject());

	//	Initialize DirectInput.
	m_pDI->InitDevices(hWnd, hInstance);

	//Initialize Stats
	m_pStats->SetBType(0);
	m_pStats->SetCharacter(0);
	m_pStats->SetDifficulty(0);
	m_pStats->SetHealth(99);
	m_pStats->SetScore(0);
	m_pStats->SetLives(3);
	m_pStats->SetLvlnum(1);

	CSGD_MessageSystem::GetIntance()->SetMsgProc(MessageProc);

	m_pOM->AddObject(pLvl);

	pMainMenu->SetImageID(m_pTM->LoadTexture("Resources/Graphics/KeM_Title.bmp"));
	pMainMenu->SetHeight((float)m_pTM->GetTextureHeight(pMainMenu->GetImageID()));
	pMainMenu->SetWidth((float)m_pTM->GetTextureWidth(pMainMenu->GetImageID()));
	pMainMenu->SetMImageID(m_pTM->LoadTexture("Resources/Graphics/KeM_Metriod.bmp", D3DCOLOR_XRGB(64, 239, 225)));
	pMainMenu->SetPImageID(m_pTM->LoadTexture("Resources/Graphics/KeM_pause.bmp"));

	pLvl->m_ncharge = m_pTM->LoadTexture("Resources/Graphics/KeM_charge.bmp", D3DCOLOR_XRGB(64, 239, 225));
	pLvl->m_nwave = m_pTM->LoadTexture("Resources/Graphics/KeM_wave.bmp", D3DCOLOR_XRGB(64, 239, 225));
	pLvl->m_nice = m_pTM->LoadTexture("Resources/Graphics/KeM_ice.bmp", D3DCOLOR_XRGB(64, 239, 225));
	pLvl->m_nplasma = m_pTM->LoadTexture("Resources/Graphics/KeM_plasma.bmp", D3DCOLOR_XRGB(64, 239, 225));
	pLvl->m_nspazer = m_pTM->LoadTexture("Resources/Graphics/KeM_spazer.bmp", D3DCOLOR_XRGB(64, 239, 225));
	m_nBitFont = m_pTM->LoadTexture("Resources/Graphics/KeM_font.bmp");
	m_nMiniMap = m_pTM->LoadTexture("Resources/Graphics/KeM_Minimap.bmp");

	CSGD_MessageSystem *pTemp = CSGD_MessageSystem::GetIntance();
	CCreateHeroMessage *pMsg = new CCreateHeroMessage(this);
	pTemp->SendMsg(pMsg);

	CSGD_Dispatcher::GetInstance()->RegisterClient("FullScreen", this);
	CSGD_Dispatcher::GetInstance()->RegisterClient("Menu", this);

	CSGD_Dispatcher::GetInstance()->RegisterClient("LoadLvl1", pLvl);
	CSGD_Dispatcher::GetInstance()->RegisterClient("LoadLvl2", pLvl);
	CSGD_Dispatcher::GetInstance()->RegisterClient("LoadLvl3", pLvl);

	//	Return Success.
	return true;
}

bool CGameClass::GameMain(void)
{
	/*	1.	Input.
	2.	Simluation.
	3.	Render.			*/

	//	Store the Previous Frame Time.
	g_dwLastStartTime = g_dwStartTime;

	//	Start counting time.  Store the CURRENT frame time.
	g_dwStartTime = GetTickCount();

	//	Find the difference between the two frames.
	g_dFractionOfSecondPassed = ((double)g_dwStartTime -
		(double)g_dwLastStartTime) / 1000.0f;

	//	1.	Input.
	m_pDI->ReadDevices();
	m_pWM->Update();

	//	Windowed Mode
	if ((m_pDI->GetKey(DIK_LALT) || m_pDI->GetKey(DIK_RALT)) && m_pDI->GetKey(DIK_RETURN))
	{
		CSGD_Dispatcher::GetInstance()->SendEvent("FullScreen", NULL);
	}
	
	/************************************************************************/
	/* Load Levels                                                          */
	/************************************************************************/
	if(m_pStats->GetLvlnum() == 1)
	{
		CSGD_Dispatcher::GetInstance()->SendEvent("LoadLvl1", NULL);

		pLvl->SetLoaded(true);
	}
 	else if(m_pStats->GetLvlnum() == 2  && pLvl->GetLoaded() == false)
 	{
		CSGD_Dispatcher::GetInstance()->SendEvent("LoadLvl2", NULL);

 		pLvl->SetLoaded(true);
 	}
 	else if(m_pStats->GetLvlnum() == 3 && pLvl->GetLoaded() == false)
 	{
		CSGD_Dispatcher::GetInstance()->SendEvent("LoadLvl3", NULL);

 		pLvl->SetLoaded(true);
 	}

	if(m_pStats->GetScore() >= lvl1)
	{
		lvl1 = 0;
		m_pStats->SetLvlnum(2);
		pLvl->SetLoaded(false);
	}
	if(m_pStats->GetScore() >= lvl2)
	{
		lvl2 = 0;
		m_pStats->SetLvlnum(3);
		pLvl->SetLoaded(false);
	}

	//Update Check
	if(pMainMenu->GetVisible() == false && pMainMenu->GetPaused() == false)
	{
		m_pOM->UpdateObjects(g_dFractionOfSecondPassed);
		pLvl->SetVisible(true);
	}
	else if (pMainMenu->GetVisible() == true)
	{
		pMainMenu->Update(g_dFractionOfSecondPassed);
		pLvl->SetVisible(false);
	}

	if(m_pDI->GetBufferedKey(DIK_ESCAPE))
		CSGD_Dispatcher::GetInstance()->SendEvent("Menu", NULL);

	if(m_pDI->GetBufferedKey(DIK_P))
	{
		bool temp = pMainMenu->GetPaused();
		temp = !temp;
		pMainMenu->SetPaused(temp);
	}

	if(m_pStats->GetHealth() <=0 && m_pStats->GetLives() > 0)
	{
		m_pStats->SetHealth(99);
		int temp = m_pStats->GetLives();
		temp--;
		m_pStats->SetLives(temp);
	}
	if(m_pStats->GetLives() <=0)
	{
		pLvl->SetVisible(false);
		pMainMenu->SetVisible(true);
		m_pWM->Stop(pLvl->m_nLvl1Bg);
		m_pWM->Stop(pLvl->m_nLvl2Bg);
		m_pWM->Stop(pLvl->m_nLvl3Bg);

	}

	char szbuffer[24];
	char szbuffer2[24];
	char szbuffer3[24];

	//	2.	Simulation.
	m_pOM->CheckCollision();

	//	3.	Render
	CSGD_MessageSystem::GetIntance()->ProcessMessages();
	CSGD_Dispatcher::GetInstance()->ProcessEvents();

	m_pD3D->DeviceBegin();

	m_pD3D->SpriteBegin();

	m_pD3D->Clear(0, 0, 0);
	if(pMainMenu->GetVisible() == true)
	{
		pMainMenu->Render();
	}
	else
	{
		m_pOM->RenderObjects();
		pMainMenu->Pause();
		sprintf_s(szbuffer,sizeof szbuffer, "Score = %i",CStats::GetInstance()->GetScore());
		DrawBitTex(szbuffer,0,0,1);
		sprintf_s(szbuffer2,sizeof szbuffer2, "Lives = %i",CStats::GetInstance()->GetLives());
		DrawBitTex(szbuffer2,0,20,1);
		sprintf_s(szbuffer3,sizeof szbuffer3, "Health = %i",CStats::GetInstance()->GetHealth());
		DrawBitTex(szbuffer3,0,40,1);
		DrawBitTex("Current Weapon",280,55,1);
	}

	m_pD3D->SpriteEnd();

	m_pD3D->DeviceEnd();

	m_pD3D->Present();

	float nScreenX = pTempHero->GetX() - pLvl->GetX();

	//World Coords
	if(nScreenX <= 500)
	{
		//BG
		float nTempB, nTempCF;
		nTempB = pLvl->GetX();
		nTempB--;
		pLvl->SetX(nTempB);

		//ceiling and floor
		nTempCF = pLvl->GetCX();
		nTempCF -= 0.5f;
		pLvl->SetCX(nTempCF);
		pLvl->SetFX(nTempCF);
	}
	if(nScreenX >= 500)
	{
		//BG
		float nTempB, nTempCF;
		nTempB = pLvl->GetX();
		nTempB++;
		pLvl->SetX(nTempB);

		//ceiling and floor
		nTempCF = pLvl->GetCX();
		nTempCF += 0.5f;
		pLvl->SetCX(nTempCF);
		pLvl->SetFX(nTempCF);
	}

	//	Return Success.
	return true;
}

bool CGameClass::GameShutdown(void)
{
	//	Always shutdown in the opposite order that you created in.
	m_pDI->ShutdownDirectInput();
	m_pWM->ShutdownWaveManager();
	m_pDS->ShutdownDirectSound();
	m_pTM->ShutdownTextureManager();
	m_pD3D->ShutdownDirect3D();
	m_pObFactory->ShutdownObjectFactory();
	DestroyEvents();
	m_pOM->RemoveAllObjects();
	m_pOM->DeleteInstance();
	m_pStats->DeleteInstance();
	CSGD_MessageSystem::DeleteInstance();
	CSGD_Dispatcher::DeleteInstance();
	SAFE_RELEASE(pMainMenu);
	SAFE_RELEASE(pLvl);

	//	Return Success.
	return true;
}

void MessageProc( CBaseMessage *pMsg )
{
	CGameClass * g = CGameClass::GetInstance();
	switch (pMsg->GetMessageID())
	{
	case MSG_CREATE_BULLET:
		{
			CBullet *pBullet = (CBullet*)g->m_pObFactory->CreateObject("CBullet");
			CCreateBulletMessage *pTMsg = (CCreateBulletMessage*)pMsg;
			CHero *pHero = (CHero*)pTMsg->GetParam();

			tVector2D			g_vctObjVel;
			g_vctObjVel.dX = 0;
			g_vctObjVel.dY = -1;


			if(pHero->GetSpeed() < 0)
			{
				if(CStats::GetInstance()->GetCharacter() == 0)
					g_vctObjVel.dY *= (-pHero->GetSpeed() + 200);
				else if(CStats::GetInstance()->GetCharacter() == 1)
					g_vctObjVel.dY *= (-pHero->GetSpeed() + 275);
				else if(CStats::GetInstance()->GetCharacter() == 2)
					g_vctObjVel.dY *= (-pHero->GetSpeed() + 25);
			}
			else
			{
				if(CStats::GetInstance()->GetCharacter() == 0)
					g_vctObjVel.dY *= (pHero->GetSpeed() + 200);
				else if(CStats::GetInstance()->GetCharacter() == 1)
					g_vctObjVel.dY *= (pHero->GetSpeed() + 275);
				else if(CStats::GetInstance()->GetCharacter() == 2)
					g_vctObjVel.dY *= (pHero->GetSpeed() + 25);
			}

			pBullet->SetX(pHero->GetX());
			pBullet->SetY(pHero->GetY());
			pBullet->SetVelY((float)g_vctObjVel.dY);


			if(g->m_pStats->GetBType() == 0)
				pBullet->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_chargeshot.bmp",D3DCOLOR_XRGB(64, 239, 225)));
			else if(g->m_pStats->GetBType() == 1)
				pBullet->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_waveshot.bmp",D3DCOLOR_XRGB(64, 239, 225)));
			else if(g->m_pStats->GetBType() == 2)
				pBullet->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_iceshot.bmp",D3DCOLOR_XRGB(64, 239, 225)));
			else if(g->m_pStats->GetBType() == 3)
				pBullet->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_plasmashot.bmp",D3DCOLOR_XRGB(64, 239, 225)));
			else if(g->m_pStats->GetBType() == 4)
				pBullet->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_spazershot.bmp",D3DCOLOR_XRGB(64, 239, 225)));

			pBullet->SetHeight((float)g->m_pTM->GetTextureHeight(pBullet->GetImageID()));
			pBullet->SetWidth((float)g->m_pTM->GetTextureWidth(pBullet->GetImageID()));
			CSGD_Dispatcher::GetInstance()->RegisterClient("Collision", pBullet);

			g->m_pOM->AddObject(pBullet);
			SAFE_RELEASE(pBullet);
		}
		break;
	case MSG_DESTROY_BULLET:
		{
			CDestroyBulletMessage *pTMsg = (CDestroyBulletMessage*)pMsg;
			CBullet *pBullet = (CBullet*)pTMsg->GetParam();

			CSGD_Dispatcher::GetInstance()->UNRegisterClient("Collision", pBullet);

			CSGD_ObjectManager::GetInstance()->RemoveObject(pBullet);
		}
		break;
	case MSG_CREATE_ENEMY:
		{
			CEnemy *pEnemy = (CEnemy*)g->m_pObFactory->CreateObject("CEnemy");
			CCreateEnemyMessage *pTMsg = (CCreateEnemyMessage*)pMsg;

			pEnemy->SetX(-100);
			int tempy = rand() % 300 + 60;
			pEnemy->SetY(float(tempy));
			int tempx = rand() % 200 + 50;
			pEnemy->SetVelX(float(tempx));
			int temp = rand() % 5;
			pEnemy->SetEType(temp);

			if(pEnemy->GetEType() == 0)
				pEnemy->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_redm.bmp", D3DCOLOR_XRGB(64, 239, 225)));
			else if(pEnemy->GetEType() == 1)
				pEnemy->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_purplem.bmp", D3DCOLOR_XRGB(64, 239, 225)));
			else if(pEnemy->GetEType() == 2)
				pEnemy->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_bluem.bmp", D3DCOLOR_XRGB(64, 239, 225)));
			else if(pEnemy->GetEType() == 3)
				pEnemy->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_greenm.bmp", D3DCOLOR_XRGB(64, 239, 225)));
			else if(pEnemy->GetEType() == 4)
				pEnemy->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_yellowm.bmp", D3DCOLOR_XRGB(64, 239, 225)));

			pEnemy->SetHeight((float)g->m_pTM->GetTextureHeight(pEnemy->GetImageID()));
			pEnemy->SetWidth((float)g->m_pTM->GetTextureWidth(pEnemy->GetImageID()));
			CSGD_Dispatcher::GetInstance()->RegisterClient("Collision", pEnemy);
			g->m_pOM->AddObject(pEnemy);
			SAFE_RELEASE(pEnemy);
		}
		break;
	case MSG_DESTROY_ENEMY:
		{
			CDestroyEnemyMessage *pTMsg = (CDestroyEnemyMessage*)pMsg;
			CEnemy *pEnemy = (CEnemy*)pTMsg->GetParam();

			CSGD_Dispatcher::GetInstance()->UNRegisterClient("Collision", pEnemy);

			CSGD_ObjectManager::GetInstance()->RemoveObject(pEnemy);
		}
		break;
	case MSG_CREATE_HERO:
		{
			CHero *pHero = (CHero*)g->m_pObFactory->CreateObject("CHero");
			CCreateHeroMessage *pTMsg = (CCreateHeroMessage*)pMsg;

			pHero->SetX(400);
			pHero->SetY(455);

			pHero->SetImageID(g->m_pTM->LoadTexture("Resources/Graphics/KeM_Samus.bmp", D3DCOLOR_XRGB(255, 0, 255)));
			pHero->SetHeight((float)g->m_pTM->GetTextureHeight(pHero->GetImageID()));
			pHero->SetWidth((float)g->m_pTM->GetTextureWidth(pHero->GetImageID()));
			pHero->SetVaria(g->m_pTM->LoadTexture("Resources/Graphics/KeM_SamusVaria.bmp", D3DCOLOR_XRGB(255, 0, 255)));
			pHero->SetGravity(g->m_pTM->LoadTexture("Resources/Graphics/KeM_SamusGrav.bmp", D3DCOLOR_XRGB(0, 255, 255)));

			g->m_pOM->AddObject(pHero);
			g->pTempHero = pHero;
			SAFE_RELEASE(pHero)
		}
		break;
	case MSG_DESTROY_HERO:
		{
			CDestroyHeroMessage *pTMsg = (CDestroyHeroMessage*)pMsg;
			CHero *pHero = (CHero*)pTMsg->GetParam();

			CSGD_ObjectManager::GetInstance()->RemoveObject(pHero);
		}
		break;
	};
}
void CGameClass::HandleEvent(CEvent *pEvent)
{
	if (pEvent->GetEventID() == "FullScreen")
	{
		m_bWin = !m_bWin;
		m_pD3D->ChangeDisplayParam(800,600,m_bWin);
	}	
	if (pEvent->GetEventID() == "Menu")
	{
		if(pMainMenu->GetVisible() == false)
		{
			pMainMenu->SetVisible(true);
			pLvl->SetVisible(false);
		}
	}	
}

void CGameClass::DestroyEvents()
{
	CSGD_Dispatcher::GetInstance()->UNRegisterClient("LoadLvl1", pLvl);
	CSGD_Dispatcher::GetInstance()->UNRegisterClient("LoadLvl2", pLvl);
	CSGD_Dispatcher::GetInstance()->UNRegisterClient("LoadLvl3", pLvl);
	CSGD_Dispatcher::GetInstance()->UNRegisterClient("FullScreen", this);
	CSGD_Dispatcher::GetInstance()->UNRegisterClient("Menu", this);
}

void CGameClass::DrawBitTex(char* str, int x, int y, float scale = 1.00f)
{
	int row, colum;
	int initialx = x;
	const int width = 26;
	const int height = 20;
	RECT temp;

	for (int i =0; i < 128; i++)
	{
		if(! str[i])
			break;
		row = (str[i] - 33) / 15;
		colum = (str[i] - 33) % 15;
		temp.left = colum * width + 6;
		temp.right = temp.left + 20;
		temp.top = row * height + 2;
		temp.bottom = temp.top + 17;
		if(str[i] == '\n')
		{
			y += 18;
			x = initialx;
			continue;
		}

		m_pTM->Draw(m_nBitFont,float(x),float(y),scale,scale,&temp);

		x += int(20 * scale);
	}
}