// This file declares the IActiveScriptSiteDebug Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyIActiveScriptSiteDebug : public PyIUnknown
{
public:
	MAKE_PYCOM_CTOR_ERRORINFO(PyIActiveScriptSiteDebug, IID_IActiveScriptSiteDebug);
	static IActiveScriptSiteDebug *GetI(PyObject *self);
	static PyComTypeObject type;

	// The Python methods
	static PyObject *GetDocumentContextFromPosition(PyObject *self, PyObject *args);
	static PyObject *GetApplication(PyObject *self, PyObject *args);
	static PyObject *GetRootApplicationNode(PyObject *self, PyObject *args);
	static PyObject *OnScriptErrorDebug(PyObject *self, PyObject *args);

protected:
	PyIActiveScriptSiteDebug(IUnknown *pdisp);
	~PyIActiveScriptSiteDebug();
};
// ---------------------------------------------------
//
// Gateway Declaration

class PyGActiveScriptSiteDebug : public PyGatewayBase, public IActiveScriptSiteDebug
{
protected:
	PyGActiveScriptSiteDebug(PyObject *instance) : PyGatewayBase(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT(PyGActiveScriptSiteDebug, IActiveScriptSiteDebug, IID_IActiveScriptSiteDebug)

	// IActiveScriptSiteDebug
	STDMETHOD(GetDocumentContextFromPosition)(
#ifdef _WIN64
		DWORDLONG dwSourceContext,
#else
		DWORD dwSourceContext,
#endif
		ULONG uCharacterOffset,
		ULONG uNumChars,
		IDebugDocumentContext __RPC_FAR *__RPC_FAR * ppsc);

	STDMETHOD(GetApplication)(
		IDebugApplication __RPC_FAR *__RPC_FAR * ppda);

	STDMETHOD(GetRootApplicationNode)(
		IDebugApplicationNode **ppdanRoot);

    STDMETHOD(OnScriptErrorDebug)(
            /* [in] */ IActiveScriptErrorDebug __RPC_FAR *pErrorDebug,
            /* [out] */ BOOL __RPC_FAR *pfEnterDebugger,
            /* [out] */ BOOL __RPC_FAR *pfCallOnScriptErrorWhenContinuing);
};
