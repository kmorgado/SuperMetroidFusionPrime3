///////////////////////////////////////////////////////////////////////
//	File	:	"CSGD_ObjectFactory.h"
//
//	Author	:	Keith Morgado
//
//	Purpose	:	A templated class that contains function pointers for 
//				the creation of objects.
///////////////////////////////////////////////////////////////////////					   

#pragma once

#include <map>
using std::map;

template <typename ClassIDType, typename BaseClassType>
class CSGD_ObjectFactory
{
	private:

		//	Function Pointer declaration.
		typedef BaseClassType *(*ObjectCreator)(void);

		//	A map of all the derived classes and the
		//	create functions associated with them.
		map<ClassIDType, ObjectCreator> m_ObjectCreators;

		static CSGD_ObjectFactory<ClassIDType, BaseClassType> m_Instance;

		//	Construct Object.
		template <typename DerivedClassType>
		static BaseClassType *ConstructObject(void)
		{
			return new DerivedClassType;
		}

		CSGD_ObjectFactory() {}
		CSGD_ObjectFactory(const CSGD_ObjectFactory &ref);
		CSGD_ObjectFactory &operator = (const CSGD_ObjectFactory &ref);

	public:

		~CSGD_ObjectFactory() {}

		static CSGD_ObjectFactory<ClassIDType, BaseClassType> *GetInstance(void)
		{
			return &m_Instance;
		}

		template <typename DerivedClassType>
		bool RegisterClassType(ClassIDType id)//, ObjectCreator pFunc)
		{
			//	Create the new class definition.
			std::pair<ClassIDType, ObjectCreator> objTypeDef;
			objTypeDef.first  = id;
			objTypeDef.second = ConstructObject<DerivedClassType>;// pFunc;

			//	Add the class definition to our Registry. (map).
			m_ObjectCreators.insert(objTypeDef);

			//	Success.
			return true;
		}

		bool UnregisterClassType(ClassIDType id)
		{
			//	Find the given key inside of the map.
			map<ClassIDType, ObjectCreator>::iterator mIter = 
						m_ObjectCreators.find(id);

			//	Error to make sure we found it.
			if (mIter == m_ObjectCreators.end())
				return false;

			//	Remove the class type.
			m_ObjectCreators.erase(mIter);
			
			//	Success.
			return true;
		}

		//	CreateObject - Creates a new class object of the given type.
		//				   Can only create objects that have been regsitered.
		BaseClassType *CreateObject(ClassIDType id)
		{
			//	Find the given key inside of the map.
			map<ClassIDType, ObjectCreator>::iterator mIter = 
						m_ObjectCreators.find(id);

			//	Error to make sure we found it.
			if (mIter == m_ObjectCreators.end())
				return false;

			//	Create the classtype and return it.
			return (*mIter).second();
		}

		void ShutdownObjectFactory(void)
		{
			m_ObjectCreators.clear();
		}
};

//	Initialize our static variable member.
template <typename ClassIDType, typename BaseClassType>
CSGD_ObjectFactory<ClassIDType, BaseClassType> CSGD_ObjectFactory<ClassIDType,
		BaseClassType>::m_Instance;