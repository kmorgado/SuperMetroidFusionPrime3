///////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_MemoryManager.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	Keeps track of and manages all game memory.
///////////////////////////////////////////////////////////////////////

#pragma once

class CSGD_MemoryManager
{
	private:
		struct tMemoryNode
		{
			void	*pMemory;			//	Where the memory gets stored.
			size_t	nSize;				//	Size of the Memory, in Bytes.

			int		nLine;				//	Line of code where the memory gets allocated.
			char	szFile[128];		//	What file the memory got allocated.

			tMemoryNode *pNext;			//	Pointer to the next node. (Memory Block).
		};

		size_t	m_nCurUsedMem;			//	Current amount of memory being used.
		size_t	m_nMaxUsedMem;			//	Total amount of memory used throughout the length of the application.

		tMemoryNode *m_pMemoryHead;		//	Start of my linked list.

		static CSGD_MemoryManager	m_Instance;

		CSGD_MemoryManager();
		CSGD_MemoryManager(CSGD_MemoryManager &ref);
		CSGD_MemoryManager &operator = (CSGD_MemoryManager &ref);

	public:
		~CSGD_MemoryManager();

		//	GetInstance.
		static CSGD_MemoryManager *GetInstance(void);

#ifdef _DEBUG

		friend void *operator new(size_t nBytes, int nLine, char *szFile);

		friend void operator delete(void *pMem);
		friend void operator delete[](void *pMem);
#endif	//	_DEBUG

		//	Logs out information about the memory manager.
		void LogStatus(void);

		//	Flushes all memory from use.
		void Flush(void);

		//	Shutsdown the memory manager.
		void Shutdown(void);
};

#ifdef _DEBUG
/*
	int *pInt = new int;			->		int *pInt = new(sizeof(int));				//  Compiler does this.

	int *pInt = new int;			->		int *pInt = new (__LINE__, __FILE__) int;	//	Macro does this.
											int *pInt = new (sizeof(int), __LINE__, __FILE__)	//	Compiler
*/
#define new new (__LINE__, __FILE__)
#endif
