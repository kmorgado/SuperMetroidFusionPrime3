///////////////////////////////////////////////////////////////////////
//	File	:	"MoreMenu.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains the information for the More options menu
///////////////////////////////////////////////////////////////////////
#pragma once
#include "CSGD_TextureManager.h"

class CMoreMenu
{
private:
	int m_nOKID;
	int m_nCharacterID;
	int m_nKeyBindID;
	int m_nDiffID;
	int m_nHighScoreID;
	int m_nBackstoryID;
	int m_nHelpID;
	int m_nSelected;
	int m_nInMenuSelected;
	bool m_bVisible;
	bool m_bLoaded;
	bool m_bInMenu;
	bool m_bChanges;

public:
	CMoreMenu(void);
	~CMoreMenu(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//
	//  Purpose : Returns specified type.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	 int GetOKImageID(void);
	 int GetMCImageID(void);
	 int GetMKImageID(void);
	 int GetMDImageID(void);
	 int GetMSImageID(void);
	 int GetMBImageID(void);
	 int GetMHImageID(void);
	 int GetSelected(void);
	 int GetInMenuSelected(void);
	 bool GetVisible(void);
	 bool GetLoaded(void);
	 bool GetInMenu(void);
	 bool GetChanges(void);

	 ///////////////////////////////////////////////////////////////////////////////////////////////////////
	 // Function: Modifiers
	 //
	 //  Purpose: Modifies specified type.
	 ////////////////////////////////////////////////////////////////////////////////////////////////////////
	 void SetOKImageID(int nID);
	 void SetMCImageID(int nID);
	 void SetMKImageID(int nID);
	 void SetMDImageID(int nID);
	 void SetMSImageID(int nID);
	 void SetMBImageID(int nID);
	 void SetMHImageID(int nID);
	 void SetSelected(int nSelected);
	 void SetInMenuSelected(int nSelected);
	 void SetVisible(bool bVisible);
	 void SetLoaded(bool bLoaded);
	 void SetInMenu(bool bInMenu);
	 void SetChanges(bool bChange);

	 ///////////////////////////////////////////////////////////////////////////////////////////////////////
	 // Function: Load
	 //
	 //  Purpose: Loads up all the Bitmaps for the Menu
	 ////////////////////////////////////////////////////////////////////////////////////////////////////////
	 void Load(void);
};
