#include "InjectorHdr.h"

#include "CoClassFactory.h"

CoClassFactory::CoClassFactory()
{
}

CoClassFactory::CoClassFactory(PFN_CREATE_OBJ pfnCreateObj) :
m_refCount(1),
m_pfnCreateObj(pfnCreateObj)
{
}

CoClassFactory::~CoClassFactory()
{
}

ULONG CoClassFactory::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}

ULONG CoClassFactory::Release()
{
	long refCount = InterlockedDecrement(&m_refCount);
	if (refCount == 0)
		delete this;

	return refCount;
}

HRESULT CoClassFactory::QueryInterface(REFIID riid, void **ppInterface)
{
	if (riid == IID_IUnknown)
	{
		*ppInterface = static_cast<IUnknown *>(this);
	}
	else if (riid == IID_IClassFactory)
	{
		*ppInterface = static_cast<IClassFactory *>(this);
	}
	else
	{
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppInterface)->AddRef();

	return S_OK;
}

HRESULT CoClassFactory::CreateInstance(IUnknown *pUnkOuter, REFIID riid, void **ppInstance)
{
	if (pUnkOuter != NULL)
		return CLASS_E_NOAGGREGATION;

	return m_pfnCreateObj(riid, ppInstance);
}

HRESULT CoClassFactory::LockServer(BOOL fLock)
{
	return S_OK;
}