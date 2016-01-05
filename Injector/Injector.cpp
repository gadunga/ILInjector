#include "InjectorHdr.h"

#include "Injector.h"

Injector * g_pCallbackObject = NULL;

const WCHAR * const pHookFunctionName = L"One";

HRESULT Injector::CreateObject(REFIID riid, void **ppInterface)
{
	*ppInterface = NULL;
	if ((riid != IID_IUnknown) &&
		(riid != IID_ICorProfilerCallback5) &&
		(riid != IID_ICorProfilerCallback) &&
		(riid != IID_ICorProfilerCallback2) &&
		(riid != IID_ICorProfilerCallback3) &&
		(riid != IID_ICorProfilerCallback4))
	{
		return E_NOINTERFACE;
	}

	
	static volatile LONG nFirstTime = 1;
	if (nFirstTime == 0)
	{
		return CORPROF_E_PROFILER_CANCEL_ACTIVATION;
	}

	if (InterlockedCompareExchange(&nFirstTime, 0, 1) == 0)
	{
		return CORPROF_E_PROFILER_CANCEL_ACTIVATION;
	}
	

	Injector * pCallback = new Injector();
	if (pCallback == NULL)
	{
		return E_OUTOFMEMORY;
	}

	pCallback->AddRef();
	*ppInterface = static_cast<ICorProfilerCallback2 *>(pCallback);

	return S_OK;
}

Injector::Injector() :
m_pProfilerInfo(NULL), m_refCount(0)
{
	g_pCallbackObject = this;
}

Injector::~Injector()
{
}

ULONG Injector::AddRef()
{
	return InterlockedIncrement(&m_refCount);
}

ULONG Injector::Release()
{
	long refCount = InterlockedDecrement(&m_refCount);
	if (refCount == 0)
		delete this;

	return refCount;
}

HRESULT Injector::QueryInterface(REFIID riid, void **ppInterface)
{
	// Get interface from riid
	if (riid == IID_IUnknown)
		*ppInterface = static_cast<IUnknown *>(this);
	else if (riid == IID_ICorProfilerCallback5)
		*ppInterface = static_cast<ICorProfilerCallback5 *>(this);
	else if (riid == IID_ICorProfilerCallback4)
		*ppInterface = static_cast<ICorProfilerCallback4 *>(this);
	else if (riid == IID_ICorProfilerCallback3)
		*ppInterface = static_cast<ICorProfilerCallback3 *>(this);
	else if (riid == IID_ICorProfilerCallback2)
		*ppInterface = static_cast<ICorProfilerCallback2 *>(this);
	else if (riid == IID_ICorProfilerCallback)
		*ppInterface = static_cast<ICorProfilerCallback *>(this);
	else
	{
		*ppInterface = NULL;
		return E_NOINTERFACE;
	}

	reinterpret_cast<IUnknown *>(*ppInterface)->AddRef();

	return S_OK;
}

HRESULT Injector::Initialize(IUnknown *pICorProfilerInfoUnk)
{
	HRESULT hr;
	hr = pICorProfilerInfoUnk->QueryInterface(IID_ICorProfilerInfo4, (void **)&m_pProfilerInfo);
	if (FAILED(hr))
	{
		return hr;
	}

	char pFileNameBuffer[1024];

	if (GetModuleFileNameA(NULL, pFileNameBuffer, 1024) == 0)
	{
		return E_FAIL;
	}

	hr = m_pProfilerInfo->SetEventMask(
		COR_PRF_MONITOR_JIT_COMPILATION |
		COR_PRF_DISABLE_ALL_NGEN_IMAGES);

	return S_OK;
}

HRESULT Injector::JITCompilationStarted(FunctionID functionID, BOOL fIsSafeToBlock)
{
	ClassID classId = 0;
	ModuleID moduleId = 0;
	mdToken tkMethod = 0;
	LPCBYTE pMethodHeader = NULL;
	ULONG iMethodSize = 0;
	mdTypeDef classTypeDef;
	mdToken methodToken = 0;
	CComPtr<IMetaDataImport> metaDataImport;

	wchar_t methodName[FILENAME_MAX];
	wchar_t typeName[FILENAME_MAX];
	wchar_t assemblyName[FILENAME_MAX];

	HRESULT hr = S_OK;
	hr = m_pProfilerInfo->GetTokenAndMetaDataFromFunction(functionID, IID_IMetaDataImport, (LPUNKNOWN *)&metaDataImport, &methodToken);
	if (FAILED(hr))
		return S_OK;
	hr = m_pProfilerInfo->GetFunctionInfo2(functionID, NULL, NULL, &moduleId, &tkMethod, 0, NULL, NULL);
	if (FAILED(hr))
		return S_OK;
	hr = metaDataImport->GetMethodProps(methodToken, &classTypeDef, methodName, FILENAME_MAX, 0, 0, 0, 0, 0, 0);
	if (FAILED(hr))
		return S_OK;
	hr = metaDataImport->GetTypeDefProps(classTypeDef, typeName, FILENAME_MAX, 0, 0, 0);
	if (FAILED(hr))
		return S_OK;
	hr = metaDataImport->GetScopeProps(assemblyName, FILENAME_MAX, 0, NULL);
	if (FAILED(hr))
		return S_OK;

	if (lstrcmpW(typeName, L"DemoApp.Program") != 0 || lstrcmpW(methodName, L"One") != 0)
	{
		return S_OK;
	}

	wprintf(L"Assembly: %s\n", assemblyName);
	wprintf(L"Type: %s\n", typeName);
	wprintf(L"Method: %s\n", methodName);

	hr = m_pProfilerInfo->GetILFunctionBody(moduleId, tkMethod, &pMethodHeader, &iMethodSize);
	if (FAILED(hr))
	{
		return S_OK;
	}

	IMAGE_COR_ILMETHOD* pMethod = (IMAGE_COR_ILMETHOD*)pMethodHeader;	
	CComPtr<IMethodMalloc> pIMethodMalloc = NULL;
	IMAGE_COR_ILMETHOD* pNewMethod = NULL;
	hr = m_pProfilerInfo->GetILFunctionBodyAllocator(moduleId, &pIMethodMalloc);
	if (FAILED(hr))
	{
		return S_OK;
	}

	pNewMethod = (IMAGE_COR_ILMETHOD*)pIMethodMalloc->Alloc(iMethodSize);
	if (pNewMethod == NULL)
	{
		return S_OK;
	}

	memcpy((void*)pNewMethod, (void*)pMethod, iMethodSize);

	COR_ILMETHOD_FAT* newFatImage = (COR_ILMETHOD_FAT*)&pNewMethod->Fat;
	if (!newFatImage->IsFat())
	{
		return S_OK;
	}

	unsigned char * pCode = newFatImage->GetCode();

	for (unsigned int j = 0; j < newFatImage->GetCodeSize(); ++j)
	{
		printf("%02x ", pCode[j]);
	}

	printf("\n");

	pCode[1] = 0x17; //Change initialization

	for (unsigned int j = 0; j < newFatImage->GetCodeSize(); ++j)
	{
		printf("%02x ", pCode[j]);
	}

	printf("\n");

	hr = m_pProfilerInfo->SetILFunctionBody(moduleId, tkMethod, (LPCBYTE)pNewMethod);
	return S_OK;
}

HRESULT Injector::ModuleLoadFinished(ModuleID moduleID, HRESULT hrStatus) { return S_OK; }
HRESULT Injector::GetReJITParameters(ModuleID moduleId, mdMethodDef methodId, ICorProfilerFunctionControl *pFunctionControl) { return S_OK; }
HRESULT Injector::CallRequestReJIT(UINT cFunctionsToRejit, ModuleID * rgModuleIDs, mdMethodDef * rgMethodDefs) { return S_OK; }
HRESULT Injector::CallRequestRevert(UINT cFunctionsToRejit, ModuleID * rgModuleIDs, mdMethodDef * rgMethodDefs) { return S_OK; }
HRESULT Injector::Shutdown() { return S_OK; }
HRESULT Injector::ReJITCompilationStarted(FunctionID functionID, ReJITID rejitId, BOOL fIsSafeToBlock) { return S_OK; }
HRESULT Injector::ReJITError(ModuleID moduleId, mdMethodDef methodId, FunctionID functionId, HRESULT hrStatus) { return hrStatus; }
HRESULT Injector::ModuleUnloadStarted(ModuleID moduleID) { return S_OK; }
HRESULT Injector::DllDetachShutdown() { return S_OK; }
HRESULT Injector::AppDomainCreationStarted(AppDomainID appDomainID) { return S_OK; }
HRESULT Injector::AppDomainCreationFinished(AppDomainID appDomainID, HRESULT hrStatus) { return S_OK; }
HRESULT Injector::AppDomainShutdownStarted(AppDomainID appDomainID) { return S_OK; }
HRESULT Injector::AppDomainShutdownFinished(AppDomainID appDomainID, HRESULT hrStatus) { return S_OK; }
HRESULT Injector::AssemblyLoadStarted(AssemblyID assemblyId) { return S_OK; }
HRESULT Injector::AssemblyLoadFinished(AssemblyID assemblyId, HRESULT hrStatus){ return S_OK; }
HRESULT Injector::AssemblyUnloadStarted(AssemblyID assemblyID) { return S_OK; }
HRESULT Injector::AssemblyUnloadFinished(AssemblyID assemblyID, HRESULT hrStatus) { return S_OK; }
HRESULT Injector::ModuleLoadStarted(ModuleID moduleID) { return S_OK; }
HRESULT Injector::ModuleUnloadFinished(ModuleID moduleID, HRESULT hrStatus) { return S_OK; }
HRESULT Injector::ModuleAttachedToAssembly(ModuleID moduleID, AssemblyID assemblyID) { return S_OK; }
HRESULT Injector::ClassLoadStarted(ClassID classID) { return S_OK; }
HRESULT Injector::ClassLoadFinished(ClassID classID, HRESULT hrStatus){ return S_OK; }
HRESULT Injector::ClassUnloadStarted(ClassID classID) { return S_OK; }
HRESULT Injector::ClassUnloadFinished(ClassID classID, HRESULT hrStatus) { return S_OK; }
HRESULT Injector::FunctionUnloadStarted(FunctionID functionID) { return S_OK; }
HRESULT Injector::JITCompilationFinished(FunctionID functionID, HRESULT hrStatus, BOOL fIsSafeToBlock) { return S_OK; }
HRESULT Injector::JITCachedFunctionSearchStarted(FunctionID functionID, BOOL *pbUseCachedFunction) { return S_OK; }
HRESULT Injector::JITCachedFunctionSearchFinished(FunctionID functionID, COR_PRF_JIT_CACHE result) { return S_OK; }
HRESULT Injector::JITFunctionPitched(FunctionID functionID) { return S_OK; }
HRESULT Injector::JITInlining(FunctionID callerID, FunctionID calleeID, BOOL *pfShouldInline) { return S_OK; }
HRESULT Injector::ReJITCompilationFinished(FunctionID functionId, ReJITID rejitId, HRESULT hrStatus, BOOL fIsSafeToBlock) { return S_OK; }
HRESULT Injector::MovedReferences2(ULONG cMovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], SIZE_T cObjectIDRangeLength[]) { return S_OK; }
HRESULT Injector::SurvivingReferences2(ULONG cSurvivingObjectIDRanges, ObjectID objectIDRangeStart[], SIZE_T   cObjectIDRangeLength[]) { return S_OK; }
HRESULT Injector::ConditionalWeakTableElementReferences(ULONG cRootRefs, ObjectID keyRefIds[], ObjectID valueRefIds[], GCHandleID rootIds[]) { return S_OK; }
HRESULT Injector::ThreadCreated(ThreadID threadID) { return S_OK; }
HRESULT Injector::ThreadDestroyed(ThreadID threadID) { return S_OK; }
HRESULT Injector::ThreadAssignedToOSThread(ThreadID managedThreadID, DWORD osThreadID) { return S_OK; }
HRESULT Injector::RemotingClientInvocationStarted() { return S_OK; }
HRESULT Injector::RemotingClientSendingMessage(GUID *pCookie, BOOL fIsAsync) { return S_OK; }
HRESULT Injector::RemotingClientReceivingReply(GUID *pCookie, BOOL fIsAsync) { return S_OK; }
HRESULT Injector::RemotingClientInvocationFinished() { return S_OK; }
HRESULT Injector::RemotingServerInvocationStarted() { return S_OK; }
HRESULT Injector::RemotingServerReceivingMessage(GUID *pCookie, BOOL fIsAsync) { return S_OK; }
HRESULT Injector::RemotingServerSendingReply(GUID *pCookie, BOOL fIsAsync) { return S_OK; }
HRESULT Injector::RemotingServerInvocationReturned() { return S_OK; }
HRESULT Injector::UnmanagedToManagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason) { return S_OK; }
HRESULT Injector::ManagedToUnmanagedTransition(FunctionID functionID, COR_PRF_TRANSITION_REASON reason) { return S_OK; }
HRESULT Injector::RuntimeSuspendStarted(COR_PRF_SUSPEND_REASON suspendReason) { return S_OK; }
HRESULT Injector::RuntimeSuspendFinished() { return S_OK; }
HRESULT Injector::RuntimeSuspendAborted() { return S_OK; }
HRESULT Injector::RuntimeResumeStarted() { return S_OK; }
HRESULT Injector::RuntimeResumeFinished() { return S_OK; }
HRESULT Injector::RuntimeThreadSuspended(ThreadID threadID) { return S_OK; }
HRESULT Injector::RuntimeThreadResumed(ThreadID threadID) { return S_OK; }
HRESULT Injector::MovedReferences(ULONG cmovedObjectIDRanges, ObjectID oldObjectIDRangeStart[], ObjectID newObjectIDRangeStart[], ULONG cObjectIDRangeLength[]) { return S_OK; }
HRESULT Injector::SurvivingReferences(ULONG cmovedObjectIDRanges, ObjectID objectIDRangeStart[], ULONG cObjectIDRangeLength[]) { return S_OK; }
HRESULT Injector::ObjectsAllocatedByClass(ULONG classCount, ClassID classIDs[], ULONG objects[]) { return S_OK; }
HRESULT Injector::ObjectAllocated(ObjectID objectID, ClassID classID) { return S_OK; }
HRESULT Injector::ObjectReferences(ObjectID objectID, ClassID classID, ULONG objectRefs, ObjectID objectRefIDs[]) { return S_OK; }
HRESULT Injector::RootReferences(ULONG rootRefs, ObjectID rootRefIDs[]) { return S_OK; }
HRESULT Injector::GarbageCollectionStarted(int cGenerations, BOOL generationCollected[], COR_PRF_GC_REASON reason) { return S_OK; }
HRESULT Injector::GarbageCollectionFinished() { return S_OK; }
HRESULT Injector::FinalizeableObjectQueued(DWORD finalizerFlags, ObjectID objectID) { return S_OK; }
HRESULT Injector::RootReferences2(ULONG cRootRefs, ObjectID rootRefIds[], COR_PRF_GC_ROOT_KIND rootKinds[], COR_PRF_GC_ROOT_FLAGS rootFlags[], UINT_PTR rootIds[]) { return S_OK; }
HRESULT Injector::HandleCreated(UINT_PTR handleId, ObjectID initialObjectId) { return S_OK; }
HRESULT Injector::HandleDestroyed(UINT_PTR handleId) { return S_OK; }
HRESULT Injector::ExceptionThrown(ObjectID thrownObjectID) { return S_OK; }
HRESULT Injector::ExceptionSearchFunctionEnter(FunctionID functionID) { return S_OK; }
HRESULT Injector::ExceptionSearchFunctionLeave() { return S_OK; }
HRESULT Injector::ExceptionSearchFilterEnter(FunctionID functionID) { return S_OK; }
HRESULT Injector::ExceptionSearchFilterLeave() { return S_OK; }
HRESULT Injector::ExceptionSearchCatcherFound(FunctionID functionID) { return S_OK; }
HRESULT Injector::ExceptionCLRCatcherFound() { return S_OK; }
HRESULT Injector::ExceptionCLRCatcherExecute() { return S_OK; }
HRESULT Injector::ExceptionOSHandlerEnter(FunctionID functionID) { return S_OK; }
HRESULT Injector::ExceptionOSHandlerLeave(FunctionID functionID) { return S_OK; }
HRESULT Injector::ExceptionUnwindFunctionEnter(FunctionID functionID) { return S_OK; }
HRESULT Injector::ExceptionUnwindFunctionLeave() { return S_OK; }
HRESULT Injector::ExceptionUnwindFinallyEnter(FunctionID functionID) { return S_OK; }
HRESULT Injector::ExceptionUnwindFinallyLeave() { return S_OK; }
HRESULT Injector::ExceptionCatcherEnter(FunctionID functionID, ObjectID objectID) { return S_OK; }
HRESULT Injector::ExceptionCatcherLeave() { return S_OK; }
HRESULT Injector::COMClassicVTableCreated(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable, ULONG cSlots) { return S_OK; }
HRESULT Injector::COMClassicVTableDestroyed(ClassID wrappedClassID, REFGUID implementedIID, void *pVTable) { return S_OK; }
HRESULT Injector::ThreadNameChanged(ThreadID threadId, ULONG cchName, __in_ecount_opt(cchName) WCHAR name[]) { return S_OK; }
HRESULT Injector::InitializeForAttach(IUnknown *pICorProfilerInfoUnk, void *pvClientData, UINT cbClientData) { return S_OK; }
HRESULT Injector::ProfilerAttachComplete() { return S_OK; }
HRESULT Injector::ProfilerDetachSucceeded() { return S_OK; }