///////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_MemoryManager.cpp"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Keeps track of and manages all game memory.
///////////////////////////////////////////////////////////////////////

#include <memory.h>
#include <malloc.h>
#include <fstream>
using namespace std;

//	Always make this the last file included.  Normally you only include this file in .cpp's
#include "CSGD_MemoryManager.h"

#ifdef _DEBUG
//	This protects our actual function definition from
//	being mangled by the #define new directive in the header.
#undef new
#endif

#define LOGFILE	"SGD_MemoryLog.txt"

//	Initialize our static variable member.
CSGD_MemoryManager CSGD_MemoryManager::m_Instance;

//	GetInstance.
CSGD_MemoryManager *CSGD_MemoryManager::GetInstance(void)
{
	return &m_Instance;
}

CSGD_MemoryManager::CSGD_MemoryManager()
{
	m_nCurUsedMem = m_nMaxUsedMem = 0;

	m_pMemoryHead = 0;

	ofstream fout(LOGFILE, ios_base::out | ios_base::trunc);
	fout.close();
}

CSGD_MemoryManager::~CSGD_MemoryManager()
{
	Flush();
}

#ifdef _DEBUG

void *operator new(size_t nBytes, int nLine, char *szFile)
{
	//	Create the node for the linked list first.
	CSGD_MemoryManager::tMemoryNode *pNode = 
		(CSGD_MemoryManager::tMemoryNode *)malloc(sizeof(CSGD_MemoryManager::tMemoryNode));

	//	Error check to make sure memory was allocated.
	if (!pNode)	return 0;

	//	Get the memory that was asked for.
	pNode->pMemory = malloc(nBytes);

	//	Error check again.
	if (!pNode->pMemory)
	{
		free(pNode);
		return 0;
	}

	//	Setup the rest of the nodes info.
	pNode->pNext  = 0;
	pNode->nSize  = nBytes;
	pNode->nLine  = nLine;

	//	Find the file that this allocation to place in.
	//	__FILE__ returns the full path.  For example.
	//	"C:\CPP\SGD_Lectures\CMemoryManager\WinMain.cpp"
	//	All I want is the "WinMain.cpp" part.

	//	Find the length of the string.
	int nLength = (int)strlen(szFile);

	//	Start from the back and read the string until you find the first "\".
	int i = 0;
	for (i = nLength; i > (nLength - 127) && i > 0; --i)
	{
		if (szFile[i] == '\\')
			break;
	}

	//	Copy the name over, +1 so we don't copy the '\\', +1 for Null Terminator.
	memcpy(&pNode->szFile, &szFile[i+1], (nLength - i) + 1);

	//	Get the MemoryManager singleton instance.
	CSGD_MemoryManager *pMM = CSGD_MemoryManager::GetInstance();

	//	Update the tracking variables.
	pMM->m_nCurUsedMem += nBytes;
	pMM->m_nMaxUsedMem += nBytes;

	//	Add the node to the list.
	pNode->pNext			= pMM->m_pMemoryHead;
	pMM->m_pMemoryHead		= pNode;

	//	Return the memory that was asked for.
	return pNode->pMemory;
}
/*
	0x43abcd12		=		Valid Memory -> Accessible.
	0x00000000		=		Invalid Memory -> In-accessible.

	0xcdcdcdcd		=		Valid Memory -> In-accessible (Chode).
	0xfefefefe
*/

void operator delete(void *pMem)
{
	//	Error check to make sure the memory can be deleted.
	if (!pMem) return;

	//	Free the memory.
	free(pMem);

	//	Variables needed.
	CSGD_MemoryManager *pMM = CSGD_MemoryManager::GetInstance();
	CSGD_MemoryManager::tMemoryNode *pCurrent	 = pMM->m_pMemoryHead;
	CSGD_MemoryManager::tMemoryNode *pPrevious   = 0;

	//	Iterate throught the list and find the node we need.
	while (pCurrent)
	{
		//	Compare Memory Addresses.
		if (pCurrent->pMemory == pMem)
			break;

		//	Increment the pointers.
		pPrevious = pCurrent;
		pCurrent   = pCurrent->pNext;
	}

	//	Error Check.
	if (!pCurrent)	return;

	//	Update the tracking variables.
	pMM->m_nCurUsedMem -= pCurrent->nSize;

	//	Remove it from the list, if previous = 0, then the current is the head.
	if (!pPrevious)
		pMM->m_pMemoryHead = pCurrent->pNext;
	else
		pPrevious->pNext = pCurrent->pNext;

	//	Delete the node's memory.
	free (pCurrent);
}

void operator delete[](void *pMem)
{
	//	Error check to make sure the memory can be deleted.
	if (!pMem) return;

	//	Free the memory.
	free(pMem);

	//	Variables needed.
	CSGD_MemoryManager *pMM = CSGD_MemoryManager::GetInstance();
	CSGD_MemoryManager::tMemoryNode *pCurrent	 = pMM->m_pMemoryHead;
	CSGD_MemoryManager::tMemoryNode *pPrevious   = 0;

	//	Iterate throught the list and find the node we need.
	while (pCurrent)
	{
		//	Compare Memory Addresses.
		if (pCurrent->pMemory == pMem)
			break;

		//	Increment the pointers.
		pPrevious = pCurrent;
		pCurrent   = pCurrent->pNext;
	}

	//	Error Check.
	if (!pCurrent)	return;

	//	Update the tracking variables.
	pMM->m_nCurUsedMem -= pCurrent->nSize;

	//	Remove it from the list, if previous = 0, then the current is the head.
	if (!pPrevious)
		pMM->m_pMemoryHead = pCurrent->pNext;
	else
		pPrevious->pNext = pCurrent->pNext;

	//	Delete the node's memory.
	free (pCurrent);
}

#endif // _DEBUG

void CSGD_MemoryManager::LogStatus(void)
{
	//	Let's open the file.
	ofstream fout(LOGFILE, ios_base::out | ios_base::app);

	//	Print out a nice little header for our data.
	fout << "=======================================\n"
	     << "Memory Usage Report " << __TIMESTAMP__ << '\n'
	     << "Max Used: " << m_nMaxUsedMem << "\tCurrent: "
		 << m_nCurUsedMem << '\n'
		 << "---------------------------------------\n"
		 << "Line\t | Size\t | File\n"
		 << "---------------------------------------\n";

	//	Is there anything to log?
	if (!m_pMemoryHead)
	{
		fout << "No Memory Lost\n\n";
		fout.close();
		return;
	}

	//	Get the Head to start the loggin process.
	tMemoryNode *pCurrent = m_pMemoryHead;

	while (pCurrent)
	{
		//	Print out the info.
		fout << pCurrent->nLine << "\t" << pCurrent->nSize << "\t"
			<< pCurrent->szFile << '\n';

		//	Advance the pointer.
		pCurrent = pCurrent->pNext;
	}
	
	//	Add a couple of lines so multiple reports don't bleed together.
	fout << "\n\n";
	fout.close();
}

void CSGD_MemoryManager::Flush(void)
{
	tMemoryNode *pDelete = m_pMemoryHead;

	while (m_pMemoryHead)
	{
		//	Move the head out of the path of destruction.
		m_pMemoryHead = m_pMemoryHead->pNext;
		
		m_nCurUsedMem -= pDelete->nSize;				//	Update Tracking Variable.
		free(pDelete->pMemory);							//	Delete the Main Memory.
		free(pDelete);									//	Delete the node.

		//	Go onto the next node.
		pDelete = m_pMemoryHead;
	}
}

void CSGD_MemoryManager::Shutdown(void)
{
	//	Reset the Max Counter.
	m_nMaxUsedMem = 0;

	LogStatus();
	Flush();
	LogStatus();
}

//	Re-define the new directive for the rest of the program.
#ifdef _DEBUG
#define new new (__LINE__, __FILE__)
#endif	//	_DEBUG

