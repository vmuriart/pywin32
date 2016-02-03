// This file declares the IDebugApplicationNode Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyIDebugApplicationNode : public PyIDebugDocumentProvider
{
public:
	MAKE_PYCOM_CTOR_ERRORINFO(PyIDebugApplicationNode, IID_IDebugApplicationNode);
	static IDebugApplicationNode *GetI(PyObject *self);
	static PyComTypeObject type;

	// The Python methods
	static PyObject *EnumChildren(PyObject *self, PyObject *args);
	static PyObject *GetParent(PyObject *self, PyObject *args);
	static PyObject *SetDocumentProvider(PyObject *self, PyObject *args);
	static PyObject *Close(PyObject *self, PyObject *args);
	static PyObject *Attach(PyObject *self, PyObject *args);
	static PyObject *Detach(PyObject *self, PyObject *args);

protected:
	PyIDebugApplicationNode(IUnknown *pdisp);
	~PyIDebugApplicationNode();
};
// ---------------------------------------------------
//
// Gateway Declaration

class PyGDebugApplicationNode : public PyGDebugDocumentProvider, public IDebugApplicationNode
{
protected:
	PyGDebugApplicationNode(PyObject *instance) : PyGDebugDocumentProvider(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT2(PyGDebugApplicationNode, IDebugApplicationNode, IID_IDebugApplicationNode,PyGDebugDocumentProvider)

	// IDebugDocumentInfo
	STDMETHOD(GetName)(
		DOCUMENTNAMETYPE dnt,
		BSTR *pbstrName);
	STDMETHOD(GetDocumentClassId)(
		CLSID *pclsidDocument);

	// IDebugDocumentProvider
	STDMETHOD(GetDocument)(
		IDebugDocument __RPC_FAR *__RPC_FAR * ppssd);

	// IDebugApplicationNode
	STDMETHOD(EnumChildren)(
		IEnumDebugApplicationNodes __RPC_FAR *__RPC_FAR * pperddp);

	STDMETHOD(GetParent)(
		IDebugApplicationNode __RPC_FAR *__RPC_FAR * pprddp);

	STDMETHOD(SetDocumentProvider)(
		IDebugDocumentProvider __RPC_FAR * pddp);

	STDMETHOD(Close)(
		void);

	STDMETHOD(Attach)(
		IDebugApplicationNode __RPC_FAR * pdanParent);

	STDMETHOD(Detach)(
		void);

};
