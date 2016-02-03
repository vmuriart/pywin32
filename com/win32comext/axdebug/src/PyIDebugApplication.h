// This file declares the IDebugApplication Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyIDebugApplication : public PyIRemoteDebugApplication
{
public:
	MAKE_PYCOM_CTOR_ERRORINFO(PyIDebugApplication, IID_IDebugApplication);
	static IDebugApplication *GetI(PyObject *self);
	static PyComTypeObject type;

	// The Python methods
	static PyObject *SetName(PyObject *self, PyObject *args);
	static PyObject *StepOutComplete(PyObject *self, PyObject *args);
	static PyObject *DebugOutput(PyObject *self, PyObject *args);
	static PyObject *StartDebugSession(PyObject *self, PyObject *args);
	static PyObject *HandleBreakPoint(PyObject *self, PyObject *args);
	static PyObject *Close(PyObject *self, PyObject *args);
	static PyObject *GetBreakFlags(PyObject *self, PyObject *args);
	static PyObject *GetCurrentThread(PyObject *self, PyObject *args);
	static PyObject *CreateAsyncDebugOperation(PyObject *self, PyObject *args);
	static PyObject *AddStackFrameSniffer(PyObject *self, PyObject *args);
	static PyObject *RemoveStackFrameSniffer(PyObject *self, PyObject *args);
	static PyObject *QueryCurrentThreadIsDebuggerThread(PyObject *self, PyObject *args);
	static PyObject *SynchronousCallInDebuggerThread(PyObject *self, PyObject *args);
	static PyObject *CreateApplicationNode(PyObject *self, PyObject *args);
	static PyObject *FireDebuggerEvent(PyObject *self, PyObject *args);
	static PyObject *HandleRuntimeError(PyObject *self, PyObject *args);
	static PyObject *FCanJitDebug(PyObject *self, PyObject *args);
	static PyObject *FIsAutoJitDebugEnabled(PyObject *self, PyObject *args);
	static PyObject *AddGlobalExpressionContextProvider(PyObject *self, PyObject *args);
	static PyObject *RemoveGlobalExpressionContextProvider(PyObject *self, PyObject *args);
protected:
	PyIDebugApplication(IUnknown *pdisp);
	~PyIDebugApplication();
};
// ---------------------------------------------------
//
// Gateway Declaration

class PyGDebugApplication : public PyGRemoteDebugApplication, public IDebugApplication
{
protected:
	PyGDebugApplication(PyObject *instance) : PyGRemoteDebugApplication(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT2(PyGDebugApplication, IDebugApplication, IID_IDebugApplication, PyGRemoteDebugApplication)

	// IRemoteDebugApplication
	STDMETHOD(ResumeFromBreakPoint)(
		IRemoteDebugApplicationThread __RPC_FAR * prptFocus,
		BREAKRESUMEACTION bra,
		ERRORRESUMEACTION era);

	STDMETHOD(CauseBreak)(
		void);

	STDMETHOD(ConnectDebugger)(
		IApplicationDebugger __RPC_FAR * pad);

	STDMETHOD(DisconnectDebugger)(
		void);

	STDMETHOD(GetDebugger)(
		IApplicationDebugger __RPC_FAR *__RPC_FAR * pad);

	STDMETHOD(CreateInstanceAtApplication)(
		REFCLSID rclsid,
		IUnknown __RPC_FAR * pUnkOuter,
		DWORD dwClsContext,
		REFIID riid,
		IUnknown __RPC_FAR *__RPC_FAR * ppvObject);

	STDMETHOD(QueryAlive)(
		void);

	STDMETHOD(EnumThreads)(
		IEnumRemoteDebugApplicationThreads __RPC_FAR * __RPC_FAR * pperdat);

	STDMETHOD(GetName)(
		BSTR __RPC_FAR * pbstrName);

	STDMETHOD (GetRootNode)(
		IDebugApplicationNode __RPC_FAR * __RPC_FAR * ppdanRoot);

    STDMETHOD(EnumGlobalExpressionContexts)(
        /* [out] */ IEnumDebugExpressionContexts __RPC_FAR *__RPC_FAR *ppedec);

	// IDebugApplication
	STDMETHOD(SetName)(
		LPCOLESTR pstrName);

	STDMETHOD(StepOutComplete)(
		void);

	STDMETHOD(DebugOutput)(
		LPCOLESTR pstr);

	STDMETHOD(StartDebugSession)(
		void);

	STDMETHOD(HandleBreakPoint)(
		BREAKREASON br,
		BREAKRESUMEACTION __RPC_FAR * pbra);

	STDMETHOD(Close)(
		void);

	STDMETHOD(GetBreakFlags)(
		APPBREAKFLAGS __RPC_FAR * pabf,
		IRemoteDebugApplicationThread __RPC_FAR* __RPC_FAR* pprdatSteppingThread);

	STDMETHOD(GetCurrentThread)(
		IDebugApplicationThread __RPC_FAR *__RPC_FAR * pat);

	STDMETHOD(CreateAsyncDebugOperation)(
		IDebugSyncOperation __RPC_FAR * psdo,
		IDebugAsyncOperation __RPC_FAR *__RPC_FAR * ppado);

	STDMETHOD(AddStackFrameSniffer)(
		IDebugStackFrameSniffer __RPC_FAR * pdsfs,
		DWORD __RPC_FAR * pdwCookie);

	STDMETHOD(RemoveStackFrameSniffer)(
		DWORD dwCookie);

	STDMETHOD(QueryCurrentThreadIsDebuggerThread)(
		void);

	STDMETHOD(SynchronousCallInDebuggerThread)(
		IDebugThreadCall __RPC_FAR * pptc,
#ifdef _WIN64
		DWORDLONG dwParam1,
		DWORDLONG dwParam2,
		DWORDLONG dwParam3);
#else
		DWORD dwParam1,
		DWORD dwParam2,
		DWORD dwParam3);
#endif

	STDMETHOD(CreateApplicationNode)(
		IDebugApplicationNode __RPC_FAR * __RPC_FAR *ppdanNew);

	STDMETHOD(FireDebuggerEvent)(
		REFGUID riid,
		IUnknown *punk);

	STDMETHOD(HandleRuntimeError)(
		IActiveScriptErrorDebug __RPC_FAR * pErrorDebug,
		IActiveScriptSite __RPC_FAR * pScriptSite,
		BREAKRESUMEACTION __RPC_FAR * pbra,
		ERRORRESUMEACTION __RPC_FAR * perra,
		BOOL __RPC_FAR * pfCallOnScriptError);

	virtual BOOL STDMETHODCALLTYPE FCanJitDebug(
		void);

	virtual BOOL STDMETHODCALLTYPE FIsAutoJitDebugEnabled(
		void);

	STDMETHOD(AddGlobalExpressionContextProvider)(
		IProvideExpressionContexts __RPC_FAR * pdsfs,
#ifdef _WIN64
		DWORDLONG __RPC_FAR * pdwCookie);
#else
		DWORD __RPC_FAR * pdwCookie);
#endif

	STDMETHOD(RemoveGlobalExpressionContextProvider)(
#ifdef _WIN64
		DWORDLONG dwCookie);
#else
		DWORD dwCookie);
#endif

};
