#pragma once

#include "InjectorHdr.h"

#include <Unknwn.h>

typedef HRESULT(__stdcall * PFN_CREATE_OBJ)(REFIID riid, void **ppInterface);

class CoClassFactory : public IClassFactory
{
private:
	CoClassFactory();

public:
	CoClassFactory(PFN_CREATE_OBJ pfnCreateObj);
	~CoClassFactory();

public:
	// IUnknown 
	COM_METHOD(ULONG) AddRef();
	COM_METHOD(ULONG) Release();
	COM_METHOD(HRESULT) QueryInterface(REFIID riid, void **ppInterface);

	// IClassFactory 
	COM_METHOD(HRESULT) LockServer(BOOL fLock);
	COM_METHOD(HRESULT) CreateInstance(IUnknown *pUnkOuter,
		REFIID riid,
		void **ppInterface);

private:
	long m_refCount;
	PFN_CREATE_OBJ m_pfnCreateObj;
};