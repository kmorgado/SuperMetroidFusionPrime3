///////////////////////////////////////////////////////////////////////
//	File	:	"Stats.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Contains all of the Statistical information
///////////////////////////////////////////////////////////////////////
#pragma once

class CStats
{
private:
	int m_nLives;
	int m_nHealth;
	int m_nCharacer;
	int m_nScore;
	int m_nDiff;
	int m_nBType;
	int m_nlvlnum;
	bool m_bSlow;
	bool m_bFast;
	bool m_bInvin;
	bool m_bNew;
	static CStats *m_pInstance;
	int m_nShoot;

	CStats(void){};
	CStats(const CStats &temp);
	CStats operator=(const CStats &temp);
public:
	~CStats(void);
	

	static CStats *GetInstance(void);
	static void DeleteInstance(void);

	//////////////////////////////////////////////////////////////////////////////////////////////////////
	//  Function: Accessors
	//
	//  Purpose : Returns specified type.
	///////////////////////////////////////////////////////////////////////////////////////////////////////
	int GetLives(void);
	int GetHealth(void);
	int GetCharacter(void);
	int GetScore(void);
	int GetDifficulty(void);
	int GetBType(void);
	bool GetSlow(void);
	bool GetFast(void);
	int GetLvlnum(void);
	int	GetShootKey(void);

	///////////////////////////////////////////////////////////////////////////////////////////////////////
	// Function: Modifiers
	//
	//  Purpose: Modifies specified type.
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	void SetLives(int nLives);
	void SetHealth(int nHealth);
	void SetCharacter(int nChar);
	void SetScore(int nScore);
	void SetDifficulty(int nDiff);
	void SetBType(int nBtype);
	void SetSlow(bool bSlow);
	void SetFast(bool bFast);
	void SetLvlnum(int nLvlnum);
	void SetShootKey(int nSKey);
};
