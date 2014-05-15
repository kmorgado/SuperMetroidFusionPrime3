///////////////////////////////////////////////////////////////////////
//	File	:	"MainMenu.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the inforamtion for the Main Menu of my game.
//				This Menu also acts as a pause menu.
///////////////////////////////////////////////////////////////////////
#include "MainMenu.h"

CMainMenu::CMainMenu(void)
{
	SetX(0);
	SetY(0);
	m_bVisible = true;
	m_bPaused = false;
	m_nSelected = 0;
	m_nBGSound = m_nMetriodSound = -1;
	this->SetType(3);
	pMore = new CMoreMenu;
}

CMainMenu::~CMainMenu(void)
{
	delete pMore;
}

bool CMainMenu::Update( double dElapsedTime )
{
	if(m_bVisible == true)
	{
		CSGD_DirectInput	*pDI = CSGD_DirectInput::GetInstance();
		CSGD_WaveManager	*pWM = CSGD_WaveManager::GetInstance();
		m_nBGSound = pWM->LoadWave("Resources/Sounds/KeM_Title.wav");
		m_nMetriodSound = pWM->LoadWave("Resources/Sounds/KeM_Metriod.wav");

		if(!pWM->IsWavePlaying(m_nBGSound))
			pWM->Play(m_nBGSound,DSBPLAY_LOOPING);

		if(pMore->GetLoaded() == false)
		{
			pMore->Load();
			pMore->SetLoaded(true);
		}
			
		if(GetSomeKeys() == true)
		{
			CStats::GetInstance()->SetShootKey(Waitforit());
			SetSomeKeys(false);
		}
		//Controls:
		//Up and down Navigate the menu and return executes the currently selected item
		if(pDI->GetBufferedKey(DIK_UP))
		{
			if(pMore->GetVisible() == false)
			{
				if(m_nSelected == 0)
				{
					m_nSelected = 2;
				}
				else if(m_nSelected == 1)
				{
					m_nSelected = 0;
				}
				else
					m_nSelected = 1;
			}
			else if (pMore->GetInMenu() == false)
			{
				if(pMore->GetSelected() == 0)
				{
					pMore->SetSelected(5);
				}
				else if(pMore->GetSelected() == 1)
				{
					pMore->SetSelected(0);
				}
				else if(pMore->GetSelected() == 2)
				{
					pMore->SetSelected(1);
				}
				else if(pMore->GetSelected() == 3)
				{
					pMore->SetSelected(2);
				}
				else if(pMore->GetSelected() == 4)
				{
					pMore->SetSelected(3);
				}
				else if(pMore->GetSelected() == 5)
				{
					pMore->SetSelected(4);
				}
			}

		}
		if(pDI->GetBufferedKey(DIK_DOWN))
		{
			if(pMore->GetVisible() == false)
			{
				if(m_nSelected == 0)
				{
					m_nSelected = 1;
				}
				else if(m_nSelected == 1)
				{
					m_nSelected = 2;
				}
				else
					m_nSelected = 0;
			}
			else if (pMore->GetInMenu() == false)
			{
				if(pMore->GetSelected() == 0)
				{
					pMore->SetSelected(1);
				}
				else if(pMore->GetSelected() == 1)
				{
					pMore->SetSelected(2);
				}
				else if(pMore->GetSelected() == 2)
				{
					pMore->SetSelected(3);
				}
				else if(pMore->GetSelected() == 3)
				{
					pMore->SetSelected(4);
				}
				else if(pMore->GetSelected() == 4)
				{
					pMore->SetSelected(5);
				}
				else if(pMore->GetSelected() == 5)
				{
					pMore->SetSelected(0);
				}

			}
		}
		if(pDI->GetBufferedKey(DIK_RETURN))
		{
			if(pMore->GetVisible() == false)
			{
				if(m_nSelected == 0)
				{
					//Enter the Game
					pWM->Play(m_nMetriodSound);
					CSGD_WaveManager::GetInstance()->Stop(m_nBGSound);
						CStats::GetInstance()->SetBType(0);
						CStats::GetInstance()->SetHealth(99);
						CStats::GetInstance()->SetScore(0);
						CStats::GetInstance()->SetLives(3);
						CStats::GetInstance()->SetLvlnum(1);
		
					SetPaused(false);
					m_bVisible = false;
				}
				else if(m_nSelected == 1)
				{
					//Quit
					PostQuitMessage(0);
				}
				else if (m_nSelected == 2)
				{
					//More
					pMore->SetVisible(true);
					pWM->Play(m_nMetriodSound);
				}
			}
			else if(pMore->GetInMenu() == false && pMore->GetVisible() == true)
			{
				if(pMore->GetSelected() == 0)
				{
					pWM->Play(m_nMetriodSound);
					pMore->SetInMenu(true);
				}
				if(pMore->GetSelected() == 1)
				{
					pWM->Play(m_nMetriodSound);
					pMore->SetInMenu(true);
				}
				if(pMore->GetSelected() == 2)
				{
					pWM->Play(m_nMetriodSound);
					pMore->SetInMenu(true);
				}
			}
			else if(pMore->GetInMenu() == true)
			{
				pMore->SetChanges(true);
				if(pMore->GetInMenuSelected() == 0)
				{
					//Change Diff, Char, or Key
					if(pMore->GetSelected() == 0)
						CStats::GetInstance()->SetCharacter(0);
					if(pMore->GetSelected() == 1)
						SetSomeKeys(true);
					if(pMore->GetSelected() == 2)
						CStats::GetInstance()->SetDifficulty(0);
				}
				if(pMore->GetInMenuSelected() == 1)
				{
					//Change Diff, Char, or Key
					if(pMore->GetSelected() == 0)
						CStats::GetInstance()->SetCharacter(1);
					if(pMore->GetSelected() == 2)
						CStats::GetInstance()->SetDifficulty(1);
				}
				if(pMore->GetInMenuSelected() == 2)
				{
					//Change Diff, Char, or Key
					if(pMore->GetSelected() == 0)
						CStats::GetInstance()->SetCharacter(2);
					if(pMore->GetSelected() == 2)
						CStats::GetInstance()->SetDifficulty(2);
				}
				if(pMore->GetInMenuSelected() == 3)
				{
					//Change Diff, Char, or Key
				}
			}
		}

		if(pDI->GetBufferedKey(DIK_ESCAPE))
		{
			if( pMore->GetVisible() == true && pMore->GetInMenu() == false)
			{
				pMore->SetVisible(false);
			}
			if (pMore->GetInMenu() == true)
			{
				pMore->SetInMenu(false);
				pMore->SetChanges(false);
				pMore->SetInMenuSelected(0);
			}

		}

		if(pDI->GetBufferedKey(DIK_LEFT))
		{
			if(pMore->GetInMenu() == true)
			{
				if(pMore->GetInMenuSelected() == 0)
				{
					pMore->SetInMenuSelected(2);

					if(pMore->GetSelected() == 1)
						pMore->SetInMenuSelected(3);
				}
				else if(pMore->GetInMenuSelected() == 1)
				{
					pMore->SetInMenuSelected(0);
				}
				else if(pMore->GetInMenuSelected() == 2)
				{
					pMore->SetInMenuSelected(1);
				}
				else if(pMore->GetInMenuSelected() == 3)
				{
					pMore->SetInMenuSelected(2);
				}
			}

		}

		if(pDI->GetBufferedKey(DIK_RIGHT))
		{
			if(pMore->GetInMenu() == true)
			{
				if(pMore->GetInMenuSelected() == 0)
				{
					pMore->SetInMenuSelected(1);
				}
				else if(pMore->GetInMenuSelected() == 1)
				{
					pMore->SetInMenuSelected(2);
				}
				else if(pMore->GetInMenuSelected() == 2)
				{
					pMore->SetInMenuSelected(0);

					if(pMore->GetSelected() == 1)
						pMore->SetInMenuSelected(3);
				}
				else if(pMore->GetInMenuSelected() == 3)
				{
					pMore->SetInMenuSelected(0);
				}
			}
		}	
	}

	return true;
}

bool CMainMenu::Render( void )
{
	if(m_bVisible == true)
	{
		RECT Temp;
		Temp.bottom = 600;
		Temp.top = 0;
		Temp.left = 0;
		Temp.right = 800;

		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() ,GetY(),1,1,&Temp);

		if(m_nSelected == 0)
		{
			CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),650,325,.5,.5);
		}
		else if(m_nSelected == 1)
		{
			CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),650,375,.5,.5);
		}
		else if (m_nSelected == 2)
		{
			CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),650,425,.5,.5);
		}
	}

	if(pMore->GetVisible() == true)
	{
		RECT Temp;
		Temp.bottom = 600;
		Temp.top = 0;
		Temp.left = 0;
		Temp.right = 800;
		if(pMore->GetSelected() == 0)
		{
			CSGD_TextureManager::GetInstance()->Draw(pMore->GetMCImageID(),0,0,1,1,&Temp);
			if(pMore->GetInMenu() == false)
			{
				CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),15,90,.5,.5);
			}
			else if(pMore->GetInMenu() == true)
			{
				if(pMore->GetInMenuSelected() == 0)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),370,400,.5,.5);
				}
				else if(pMore->GetInMenuSelected() == 1)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),500,400,.5,.5);
				}
				else if(pMore->GetInMenuSelected() == 2)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),650,400,.5,.5);
				}
			}

			if(pMore->GetChanges() == true)
			{
				CSGD_TextureManager::GetInstance()->Draw(pMore->GetOKImageID(),200,200,1,1);
			}
		}
		else if(pMore->GetSelected() == 1)
		{
			CSGD_TextureManager::GetInstance()->Draw(pMore->GetMKImageID(),0,0,1,1,&Temp);
			if(pMore->GetInMenu() == false)
			{
				CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),15,160,.5,.5);
			}
			else
			{
				if(pMore->GetInMenuSelected() == 0)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),360,160,.5,.5);
				}
				else if(pMore->GetInMenuSelected() == 1)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),505,160,.5,.5);
				}
				else if(pMore->GetInMenuSelected() == 2)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),660,160,.5,.5);
				}
				else if(pMore->GetInMenuSelected() == 3)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),500,295,.5,.5);
				}
			}

			if(pMore->GetChanges() == true)
			{
				CSGD_TextureManager::GetInstance()->Draw(pMore->GetOKImageID(),200,200,1,1);
			}
		}
		else if(pMore->GetSelected() == 2)
		{
			CSGD_TextureManager::GetInstance()->Draw(pMore->GetMDImageID(),0,0,1,1,&Temp);
			if(pMore->GetInMenu() == false)
			{
				CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),15,230,.5,.5);
			}
			else if(pMore->GetInMenu() == true)
			{
				if(pMore->GetInMenuSelected() == 0)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),360,350,.5,.5);
				}
				else if(pMore->GetInMenuSelected() == 1)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),520,350,.5,.5);
				}
				else if(pMore->GetInMenuSelected() == 2)
				{
					CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),675,350,.5,.5);
				}
			}

			if(pMore->GetChanges() == true)
			{
				CSGD_TextureManager::GetInstance()->Draw(pMore->GetOKImageID(),200,200,1,1);
			}
		}
		else if(pMore->GetSelected() == 3)
		{
			CSGD_TextureManager::GetInstance()->Draw(pMore->GetMSImageID(),0,0,1,1,&Temp);
			CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),15,300,.5,.5);
		}
		else if(pMore->GetSelected() == 4)
		{
			CSGD_TextureManager::GetInstance()->Draw(pMore->GetMBImageID(),0,0,1,1,&Temp);
			CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),15,380,.5,.5);
		}
		else if(pMore->GetSelected() == 5)
		{
			CSGD_TextureManager::GetInstance()->Draw(pMore->GetMHImageID(),0,0,1,1,&Temp);
			CSGD_TextureManager::GetInstance()->Draw(GetMImageID(),15,440,.5,.5);
		}
	}
	return true;
}

bool CMainMenu::Pause( void )
{
	if(m_bPaused == true)
	{
		RECT Temp;
		Temp.bottom = 600;
		Temp.top = 0;
		Temp.left = 0;
		Temp.right = 800;
		CSGD_TextureManager::GetInstance()->Draw(GetPImageID(),GetX() ,GetY(),1,1,&Temp,0,0,0,D3DCOLOR_ARGB(205,255,255,255));
	}
	return true;
}

void CMainMenu::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool CMainMenu::GetVisible(void)
{
	return m_bVisible;
}

void CMainMenu::SetSelected(int nSelected)
{
	m_nSelected = nSelected;
}

int CMainMenu::GetSelected(void)
{
	return m_nSelected;
}

int CMainMenu::GetMImageID( void )
{
	return m_nMetriodID;
}

void CMainMenu::SetMImageID( int nID )
{
	m_nMetriodID = nID;
}

int CMainMenu::GetPImageID( void )
{
	return m_nPauseID;
}

void CMainMenu::SetPImageID( int nID )
{
	m_nPauseID = nID;
}

bool CMainMenu::GetPaused( void )
{
	return m_bPaused;
}

void CMainMenu::SetPaused( bool bPaused )
{
	m_bPaused = bPaused;
}

int CMainMenu::Waitforit( void )
{
	return CSGD_DirectInput::GetInstance()->CheckBufferedKeys();
}

bool CMainMenu::GetSomeKeys( void )
{
	return m_bKey;
}

void CMainMenu::SetSomeKeys( bool bKey )
{
	m_bKey = bKey;
}