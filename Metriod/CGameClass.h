///////////////////////////////////////////////////////////////////////
//	File	:	"CGameClass.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains all game specific functionality.
///////////////////////////////////////////////////////////////////////

#pragma once
#pragma warning (disable: 4291)

#include "CSGD_Direct3D.h"
#include "CSGD_TextureManager.h"
#include "CSGD_DirectSound.h"
#include "CSGD_WaveManager.h"
#include "CSGD_DirectInput.h"
#include "CSGD_Flags.h"
#include "CObjectManager.h"
#include "CSGD_ObjectFactory.h"
#include "Hero.h"
#include "Bullet.h"
#include "Enemy.h"
#include "MainMenu.h"
#include "Lvl.h"
#include "CSGD_MessageSystem.h"
#include "CSGD_Dispatcher.h"
#include "Stats.h"
#include <ctime>
#include <string>
using namespace std;
#include "CSGD_MemoryManager.h"

#define GAME_WINDOWED	BIT_ONE

class CGameClass : public IListener
{
	private:
		CSGD_Direct3D		*m_pD3D;
		CSGD_TextureManager	*m_pTM;
		CSGD_DirectSound	*m_pDS;
		CSGD_WaveManager	*m_pWM;
		CSGD_DirectInput	*m_pDI;
		CStats				*m_pStats;
		bool				m_bWin;
		CSGD_Flags			m_Flags;
		CSGD_ObjectManager  *m_pOM;
		CSGD_ObjectFactory<string, CBase>  *m_pObFactory; 
		CMainMenu			*pMainMenu;
		CLvl				*pLvl;
		CHero				*pTempHero;
		int				    lvl1, lvl2;
		int					m_nBitFont;
		DWORD g_dwStartTime;		
		DWORD g_dwLastStartTime;	
		double g_dFractionOfSecondPassed;
		tVector2D	vctVel;

		//	Singleton Instance for this class.
		static CGameClass *m_pInstance;		

		//	Constructor. Private.
		CGameClass();
		//	Copy Constructor.
		CGameClass(const CGameClass &ref);
		//	Assignment Operator.
		CGameClass &operator = (const CGameClass &ref);

	public:
		int					m_nMiniMap;
		//	Destructor.
		~CGameClass();

		//	GetInstance	:	Create the first and only instance of this class
		//					as well as allows access to this class by
		//					returning the memory address.
		static CGameClass *GetInstance(void);

		//	DeleteInstance	:	Deletes the memory for this instance.
		static void DeleteInstance(void);

		friend void MessageProc(CBaseMessage *pMsg);

		///////////////////////////////////////////////////////////////
		//	Function	:	GameInit
		//
		//	Purpose		:	To Load up and initialize any necessary
		//					resources for the game.
		///////////////////////////////////////////////////////////////
		bool GameInit(HWND hWnd, HINSTANCE hInstance,
					  int nWidth, int nHeight, bool bWindowed);

		bool GameMain(void);

		bool GameShutdown(void);
	
		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// Function: HandleEvent
		//
		//  Purpose: Handles any Event the class is in charge of
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void HandleEvent(CEvent *pEvent);

		//////////////////////////////////////////////////////////////////////////////////////////////////////
		// Function: DestroyEvents
		//
		//  Purpose: Destroys all events in the class
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DestroyEvents();

		///////////////////////////////////////////////////////////////////////////////////////////////////////
		// Function: DrawBitTex
		//
		//  Purpose: Draws my Bitmap Texture
		////////////////////////////////////////////////////////////////////////////////////////////////////////
		void DrawBitTex(char * str, int x, int y, float scale);
};