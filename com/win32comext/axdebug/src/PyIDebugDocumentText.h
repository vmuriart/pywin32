// This file declares the IDebugDocumentText Interface and Gateway for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyIDebugDocumentText : public PyIDebugDocument
{
public:
	MAKE_PYCOM_CTOR_ERRORINFO(PyIDebugDocumentText, IID_IDebugDocumentText);
	static IDebugDocumentText *GetI(PyObject *self);
	static PyComTypeObject type;

	// The Python methods
	static PyObject *GetDocumentAttributes(PyObject *self, PyObject *args);
	static PyObject *GetSize(PyObject *self, PyObject *args);
	static PyObject *GetPositionOfLine(PyObject *self, PyObject *args);
	static PyObject *GetLineOfPosition(PyObject *self, PyObject *args);
	static PyObject *GetText(PyObject *self, PyObject *args);
	static PyObject *GetPositionOfContext(PyObject *self, PyObject *args);
	static PyObject *GetContextOfPosition(PyObject *self, PyObject *args);

protected:
	PyIDebugDocumentText(IUnknown *pdisp);
	~PyIDebugDocumentText();
};
// ---------------------------------------------------
//
// Gateway Declaration

class PyGDebugDocumentText : public PyGDebugDocument, public IDebugDocumentText
{
protected:
	PyGDebugDocumentText(PyObject *instance) : PyGDebugDocument(instance) { ; }
	PYGATEWAY_MAKE_SUPPORT2(PyGDebugDocumentText, IDebugDocumentText, IID_IDebugDocumentText, PyGDebugDocument)

	// IDebugDocumentInfo
	STDMETHOD(GetName)(
		DOCUMENTNAMETYPE dnt,
		BSTR *pbstrName);
	STDMETHOD(GetDocumentClassId)(
		CLSID *pclsidDocument);

	// IDebugDocument

	// IDebugDocumentText
	STDMETHOD(GetDocumentAttributes)(
		TEXT_DOC_ATTR __RPC_FAR * ptextdocattr);

	STDMETHOD(GetSize)(
		ULONG __RPC_FAR * pcNumLines,
		ULONG __RPC_FAR * pcNumChars);

	STDMETHOD(GetPositionOfLine)(
		ULONG cLineNumber,
		ULONG __RPC_FAR * pcCharacterPosition);

	STDMETHOD(GetLineOfPosition)(
		ULONG cCharacterPosition,
		ULONG __RPC_FAR * pcLineNumber,
		ULONG __RPC_FAR * pcCharacterOffsetInLine);

	STDMETHOD(GetText)(
		ULONG cCharacterPosition,
		WCHAR __RPC_FAR * pcharText,
		SOURCE_TEXT_ATTR __RPC_FAR * pstaTextAttr,
		ULONG __RPC_FAR * pcNumChars,
		ULONG cMaxChars);

	STDMETHOD(GetPositionOfContext)(
		IDebugDocumentContext __RPC_FAR * psc,
		ULONG __RPC_FAR * pcCharacterPosition,
		ULONG __RPC_FAR * cNumChars);

	STDMETHOD(GetContextOfPosition)(
		ULONG cCharacterPosition,
		ULONG cNumChars,
		IDebugDocumentContext __RPC_FAR *__RPC_FAR * ppsc);

};
