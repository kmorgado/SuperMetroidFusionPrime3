///////////////////////////////////////////////////////////////////////
//	File	:	"CObjectManager.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Manages all in-game objects for our application.
///////////////////////////////////////////////////////////////////////

#include "CObjectManager.h"
#include "CGameClass.h"

CSGD_ObjectManager *CSGD_ObjectManager::m_pInstance = 0;

CSGD_ObjectManager::CSGD_ObjectManager()
{

}

CSGD_ObjectManager::~CSGD_ObjectManager()
{

}


CSGD_ObjectManager* CSGD_ObjectManager::GetInstance(void)
{
	if(!m_pInstance)
		m_pInstance = new CSGD_ObjectManager;

	return m_pInstance;
}

void CSGD_ObjectManager::DeleteInstance(void)
{
	if (m_pInstance)			
	{
		delete m_pInstance;		
		m_pInstance = 0;		
	}
}

//	Automatic AddRef.
void CSGD_ObjectManager::AddObject(CBase *pObj)
{
	if (!pObj) return;		//	Error check to make sure the pointer is valid.

	//	AddRef - We are making another reference to this pointer.
	pObj->AddRef();

	//	Add the object to be managed.
	m_vObjects.push_back(pObj);
}

//	Automatic Release.
void CSGD_ObjectManager::RemoveObject(CBase *pObj)
{
	vector<CBase *>::iterator vIter = m_vObjects.begin();

	while (vIter != m_vObjects.end())
	{
		//	Compare memory addresses.
		if ((*vIter) == pObj)
		{
			SAFE_RELEASE(pObj);					//	Release the memory.
			vIter = m_vObjects.erase(vIter);	//	Remove the node from list.
			return;								//	DONE.
		}

		vIter++;				//	Move on to the next node.
	}
}

//	Automatic Release for all of my objects.
void CSGD_ObjectManager::RemoveAllObjects(void)
{
	vector<CBase *>::iterator vIter = m_vObjects.begin();

	while (vIter != m_vObjects.end())
	{
		SAFE_RELEASE((*vIter));					//	Release the memory.
		vIter = m_vObjects.erase(vIter);		//	Remove the node.
	}

	//	Clear the vector.
	m_vObjects.clear();
}

void CSGD_ObjectManager::UpdateObjects(double dElapsedTime)
{
	if(CStats::GetInstance()->GetSlow() == true)
		dElapsedTime *= SLOW;

	else if(CStats::GetInstance()->GetFast() == true)
		dElapsedTime *= FAST;

	std::vector<CBase *>::iterator vIter = m_vObjects.begin();

	while (vIter != m_vObjects.end())
	{
		(*vIter)->Update(dElapsedTime);
		vIter++;
	}
}


void CSGD_ObjectManager::RenderObjects(void)
{
	for (unsigned int i = 0; i < m_vObjects.size(); i++)
		m_vObjects[i]->Render();

	PrintMyMap();
}

void CSGD_ObjectManager::CheckCollision( void )
{
	std::vector<CBase*>::iterator iter = m_vObjects.begin();
	std::vector<CBase*>::iterator iter2 = m_vObjects.begin() ;

	while (iter != m_vObjects.end())
	{
		if(iter != iter2)
		{
			iter2 = m_vObjects.begin();

			while (iter2 != m_vObjects.end())
			{
				if((*iter)->GetType() != (*iter2)->GetType() && (*iter)->GetType() == 2 /*Enemy*/)
				{
					if((*iter)->CheckCollision((*iter2)) && (*iter2)->GetType() == 0 /*Bullet*/)
					{
						CSGD_Dispatcher::GetInstance()->SendEvent("Collision", (*iter));
						CSGD_Dispatcher::GetInstance()->SendEvent("Collision", (*iter2));
						break;
					}
				}
				iter2++;

			}
		}
		iter++;
	}
}

void CSGD_ObjectManager::PrintMyMap( void )
{
	RECT temp;

	CSGD_TextureManager::GetInstance()->Draw(CGameClass::GetInstance()->m_nMiniMap,650, 30, 100, 45, 0, 0, 0, 0, D3DCOLOR_ARGB(100,0,0,255));

	for (unsigned int i =0; i < m_vObjects.size(); i++)
	{
 		temp = m_vObjects[i]->GetRect();
		
		switch(m_vObjects[i]->GetType())
		{

		case 0:
			CSGD_TextureManager::GetInstance()->Draw(CGameClass::GetInstance()->m_nMiniMap,float(650+(temp.left/8)),float(10+(temp.top /8)),2, 2,0,0,0,0,D3DCOLOR_ARGB(255,255,255,0));
			break;
		case 1:
			CSGD_TextureManager::GetInstance()->Draw(CGameClass::GetInstance()->m_nMiniMap,float(650+(temp.left/8)), float(10+(temp.top /8)),2, 6,0,0,0,0,D3DCOLOR_ARGB(255,0,255,0));
			break;
		case 2:
			CSGD_TextureManager::GetInstance()->Draw(CGameClass::GetInstance()->m_nMiniMap,float(650+(temp.left/8)), float(10+(temp.top /8)),4, 4,0,0,0,0,D3DCOLOR_ARGB(255,255,0,0));
			break;
		}

	}
}
