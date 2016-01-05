#include "InjectorHdr.h"
#include "CoClassFactory.h"
#include "Injector.h"

STDAPI DllGetClassObject(REFCLSID rclsid, REFIID riid, LPVOID FAR *ppv)
{
	HRESULT hr = CLASS_E_CLASSNOTAVAILABLE;

	if (CLSID_PROFILER == rclsid)
	{
		CoClassFactory *pClassFactory = new CoClassFactory(Injector::CreateObject);
		if (pClassFactory != NULL)
		{
			hr = pClassFactory->QueryInterface(riid, ppv);
			pClassFactory->Release();
		}
		else
		{
			hr = E_OUTOFMEMORY;
		}
	}
	return hr;
}