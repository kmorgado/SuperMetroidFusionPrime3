///////////////////////////////////////////////////////////////////////
//	File	:	"Lvl.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the inforamtion for all the levels in my game
///////////////////////////////////////////////////////////////////////
#include "Lvl.h"

CLvl::CLvl(void)
{
	m_blvlloaded = false;
	m_bVisible = false;
	m_fX = -600;
	m_fcelingX = -600;
	m_ffloorX = -600;
}

CLvl::~CLvl(void)
{
}


bool CLvl::Update(double dElapsedTime)
{
 	CSGD_WaveManager	*pWM = CSGD_WaveManager::GetInstance();
 
  	if(m_bVisible == true)
  	{
		if(CStats::GetInstance()->GetLvlnum() == 1)
		{
			m_nLvl1Bg = CSGD_WaveManager::GetInstance()->LoadWave("Resources/Sounds/KeM_Crateria.wav");
  			if(!pWM->IsWavePlaying(m_nLvl1Bg))
  				pWM->Play(m_nLvl1Bg,DSBPLAY_LOOPING);

			if(CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_P) || CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_ESCAPE))
				pWM->Stop(m_nLvl1Bg);
		}
		if(CStats::GetInstance()->GetLvlnum() == 2)
		{
			m_nLvl2Bg = CSGD_WaveManager::GetInstance()->LoadWave("Resources/Sounds/KeM_Brinstar.wav");
			if(!pWM->IsWavePlaying(m_nLvl2Bg))
				pWM->Play(m_nLvl2Bg,DSBPLAY_LOOPING);

			if(CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_P) || CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_ESCAPE))
				pWM->Stop(m_nLvl2Bg);
		}
		if(CStats::GetInstance()->GetLvlnum() == 3)
		{
			m_nLvl3Bg = CSGD_WaveManager::GetInstance()->LoadWave("Resources/Sounds/KeM_Norfair.wav");
			if(!pWM->IsWavePlaying(m_nLvl3Bg))
				pWM->Play(m_nLvl3Bg,DSBPLAY_LOOPING);

			if(CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_P) || CSGD_DirectInput::GetInstance()->GetBufferedKey(DIK_ESCAPE))
				pWM->Stop(m_nLvl3Bg);
		}
  	}

	return true;
}

bool CLvl::Render(void)
{
	if(CStats::GetInstance()->GetLvlnum() == 1)
	{
		//bg
		RECT Temp;
		Temp.bottom = 600;
		Temp.top = 0;
		Temp.left = 0;
		Temp.right = 1600;
		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX() ,GetY(),1,1,&Temp);
		//floor
		CSGD_TextureManager::GetInstance()->Draw(GetFImageID(),GetFX(),570,1,1,&Temp);
		//ceiling
		CSGD_TextureManager::GetInstance()->Draw(GetCImageID(),GetCX(),78,1,1,&Temp);
	}
	else if(CStats::GetInstance()->GetLvlnum() == 2)
	{
		//bg
		RECT Temp;
		Temp.bottom = 600;
		Temp.top = 0;
		Temp.left = 0;
		Temp.right = 1600;
		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX(),GetY(),1,1,&Temp);
		//floor
		CSGD_TextureManager::GetInstance()->Draw(GetFImageID(),GetFX(),550,1,1,&Temp);
		//ceiling
		CSGD_TextureManager::GetInstance()->Draw(GetCImageID(),GetCX(),75,1,1,&Temp);
	}
	else if(CStats::GetInstance()->GetLvlnum() == 3)
	{
		//bg
		RECT Temp;
		Temp.bottom = 600;
		Temp.top = 0;
		Temp.left = 0;
		Temp.right = 1600;
		CSGD_TextureManager::GetInstance()->Draw(GetImageID(),GetX(),GetY(),1,1,&Temp);
		//floor
		CSGD_TextureManager::GetInstance()->Draw(GetFImageID(),GetFX(),560,1,1,&Temp);
		//ceiling
		CSGD_TextureManager::GetInstance()->Draw(GetCImageID(),GetCX(),75,1,1,&Temp);
	}

	if(CStats::GetInstance()->GetBType() == 0)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_ncharge,400,20,2,2,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nwave,450,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nice,500,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nplasma,300,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nspazer,350,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(CStats::GetInstance()->GetBType() == 1)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_ncharge,350,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nwave,400,20,2,2,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nice,450,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nplasma,500,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nspazer,300,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(CStats::GetInstance()->GetBType() == 2)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_ncharge,300,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nwave,350,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nice,400,20,2,2,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nplasma,450,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nspazer,500,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(CStats::GetInstance()->GetBType() == 3)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_ncharge,500,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nwave,300,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nice,350,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nplasma,400,20,2,2,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nspazer,450,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
	}
	else if(CStats::GetInstance()->GetBType() == 4)
	{
		CSGD_TextureManager::GetInstance()->Draw(m_ncharge,450,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nwave,500,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nice,300,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nplasma,350,20,2,2,0,0,0,0,D3DCOLOR_ARGB(100,255,255,255));
		CSGD_TextureManager::GetInstance()->Draw(m_nspazer,400,20,2,2,0,0,0,0,D3DCOLOR_ARGB(255,255,255,255));
	}

	return true;
}

int CLvl::GetCImageID()
{
	return m_nceling;
}

void CLvl::SetCImageID( int nID )
{
	m_nceling = nID;
}

int CLvl::GetFImageID()
{
	return m_nfloor;
}

void CLvl::SetFImageID( int nID )
{
	m_nfloor = nID;
}
bool CLvl::GetLoaded()
{
	return m_blvlloaded;
}
void CLvl::SetLoaded( bool bLoaded )
{
	m_blvlloaded = bLoaded;
}

void CLvl::SetVisible(bool bVisible)
{
	m_bVisible = bVisible;
}

bool CLvl::GetVisible(void)
{
	return m_bVisible;
}

void CLvl::SetCX(float nCX)
{
	m_fcelingX = nCX;
}

float CLvl::GetCX(void)
{
	return m_fcelingX;
}

void CLvl::SetFX(float nFX)
{
	m_ffloorX = nFX;
}

float CLvl::GetFX(void)
{
	return m_ffloorX;
}

void CLvl::HandleEvent(CEvent *pEvent)
{

	if (pEvent->GetEventID() == "LoadLvl1")
	{
		this->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl1BG.bmp"));
		this->SetFImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl1ground.bmp",D3DCOLOR_XRGB(0, 0, 0)));
		this->SetCImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl1ceiling.bmp",D3DCOLOR_XRGB(0, 0, 0)));
	}	
	if (pEvent->GetEventID() == "LoadLvl2")
	{
		CSGD_WaveManager::GetInstance()->Stop(m_nLvl1Bg);

		this->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl2BG.bmp"));
		this->SetFImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl2ground.bmp",D3DCOLOR_XRGB(64, 239, 225)));
		this->SetCImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl2ceiling.bmp",D3DCOLOR_XRGB(0, 0, 0)));
	}	
	if (pEvent->GetEventID() == "LoadLvl3")
	{
		CSGD_WaveManager::GetInstance()->Stop(m_nLvl2Bg);

		this->SetImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl3BG.bmp",D3DCOLOR_XRGB(64, 239, 225)));
		this->SetFImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl3ground.bmp",D3DCOLOR_XRGB(64, 239, 225)));
		this->SetCImageID(CSGD_TextureManager::GetInstance()->LoadTexture("Resources/Graphics/KeM_lvl3ceiling.bmp",D3DCOLOR_XRGB(0, 0, 0)));
	}	
}