// This file implements the IEnumConnections Interface and Gateway for Python.
// Generated by makegw.py

#include "stdafx.h"
#include "PythonCOM.h"
#include "PythonCOMServer.h"
#include "PyIEnumConnections.h"

// @doc - This file contains autoduck documentation

// ---------------------------------------------------
//
// Interface Implementation

PyIEnumConnections::PyIEnumConnections(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIEnumConnections::~PyIEnumConnections()
{
}

/* static */ IEnumConnections *PyIEnumConnections::GetI(PyObject *self)
{
	return (IEnumConnections *)PyIUnknown::GetI(self);
}

// @pymethod object|PyIEnumConnections|Next|Retrieves a specified number of items in the enumeration sequence.
PyObject *PyIEnumConnections::Next(PyObject *self, PyObject *args)
{
	long celt = 1;
	// @pyparm int|num|1|Number of items to retrieve.
	if ( !PyArg_ParseTuple(args, "|l:Next", &celt) )
		return NULL;

	IEnumConnections *pIEConnections = GetI(self);
	if ( pIEConnections == NULL )
		return NULL;

	CONNECTDATA *rgVar = new CONNECTDATA [celt];
	if ( rgVar == NULL ) {
		PyErr_SetString(PyExc_MemoryError, "allocating result array");
		return NULL;
	}
	memset(rgVar, 0, sizeof(CONNECTDATA)*celt);

	int i;
	ULONG celtFetched = 0;
	HRESULT hr = pIEConnections->Next(celt, rgVar, &celtFetched);
	if (  HRESULT_CODE(hr) != ERROR_NO_MORE_ITEMS && FAILED(hr) )
	{
		delete [] rgVar;
		return PyCom_BuildPyException(hr);
	}

	PyObject *result = PyTuple_New(celtFetched);
	if (result) {
		for ( i = celtFetched; i--; )
		{
			PyObject *ob = PyCom_PyObjectFromIUnknown(rgVar[i].pUnk, IID_IUnknown, FALSE);
			rgVar[i].pUnk = NULL;
			if ( ob == NULL ) {
				Py_DECREF(result);
				result = NULL;
				break;
			}
			PyTuple_SET_ITEM(result, i, Py_BuildValue("Nl",ob, rgVar[i].dwCookie));
		}
	}
	for ( i = celtFetched; i--; ) PYCOM_RELEASE(rgVar[i].pUnk);
	delete [] rgVar;
	return result;
}

// @pymethod |PyIEnumConnections|Skip|Skips over the next specified elementes.
PyObject *PyIEnumConnections::Skip(PyObject *self, PyObject *args)
{
	long celt;
	if ( !PyArg_ParseTuple(args, "l:Skip", &celt) )
		return NULL;

	IEnumConnections *pIEConnections = GetI(self);
	if ( pIEConnections == NULL )
		return NULL;

	HRESULT hr = pIEConnections->Skip(celt);
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr);

	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod |PyIEnumConnections|Reset|Resets the enumeration sequence to the beginning.
PyObject *PyIEnumConnections::Reset(PyObject *self, PyObject *args)
{
	if ( !PyArg_ParseTuple(args, ":Reset") )
		return NULL;

	IEnumConnections *pIEConnections = GetI(self);
	if ( pIEConnections == NULL )
		return NULL;

	HRESULT hr = pIEConnections->Reset();
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr);

	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod <o PyIEnumConnections>|PyIEnumConnections|Clone|Creates another enumerator that contains the same enumeration state as the current one
PyObject *PyIEnumConnections::Clone(PyObject *self, PyObject *args)
{
	if ( !PyArg_ParseTuple(args, ":Clone") )
		return NULL;

	IEnumConnections *pIEConnections = GetI(self);
	if ( pIEConnections == NULL )
		return NULL;

	IEnumConnections *pClone;
	HRESULT hr = pIEConnections->Clone(&pClone);
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr);

	return PyCom_PyObjectFromIUnknown(pClone, IID_IEnumConnections, FALSE);
}

// @object PyIEnumConnections|A Python interface to IEnumConnections
static struct PyMethodDef PyIEnumConnections_methods[] =
{
	{ "Next", PyIEnumConnections::Next, 1 },    // @pymeth Next|Retrieves a specified number of items in the enumeration sequence.
	{ "Skip", PyIEnumConnections::Skip, 1 },	// @pymeth Skip|Skips over the next specified elementes.
	{ "Reset", PyIEnumConnections::Reset, 1 },	// @pymeth Reset|Resets the enumeration sequence to the beginning.
	{ "Clone", PyIEnumConnections::Clone, 1 },	// @pymeth Clone|Creates another enumerator that contains the same enumeration state as the current one.
	{ NULL }
};

PyComEnumTypeObject PyIEnumConnections::type("PyIEnumConnections",
		&PyIUnknown::type, // @base PyIEnumConnections|PyIUnknown
		sizeof(PyIEnumConnections),
		PyIEnumConnections_methods,
		GET_PYCOM_CTOR(PyIEnumConnections));

// ---------------------------------------------------
//
// Gateway Implementation

STDMETHODIMP PyGEnumConnections::Next(
            /* [in] */ ULONG celt,
            /* [length_is][size_is][out] */ LPCONNECTDATA rgVar,
            /* [out] */ ULONG __RPC_FAR *pCeltFetched)
{
	PY_GATEWAY_METHOD;
	PyObject *result;
	HRESULT hr = InvokeViaPolicy("Next", &result, "i", celt);
	if ( FAILED(hr) )
		return hr;

	if ( !PySequence_Check(result) )
		goto error;
	int len;
	len = PyObject_Length(result);
	if ( len == -1 )
		goto error;
	if ( len > (int)celt)
		len = celt;

	if ( pCeltFetched )
		*pCeltFetched = len;

	int i;
	for ( i = 0; i < len; ++i )
	{
		PyObject *ob = PySequence_GetItem(result, i);
		if ( ob == NULL )
			goto error;

		PyObject *obUnk;
		if ( !PyArg_ParseTuple(ob, "Oi", &obUnk, &rgVar[i].dwCookie) ) {
			PyErr_SetString(PyExc_TypeError, "result must be a tuple of (PyIUnknown, dwCookie)");
			return PyCom_HandlePythonFailureToCOM();
		}
		if ( !PyCom_InterfaceFromPyObject(obUnk, IID_IUnknown, (void **)&rgVar[i].pUnk, FALSE) )
		{
			Py_DECREF(result);
			return PyCom_SetCOMErrorFromPyException(IID_IEnumConnections);
		}
	}

	Py_DECREF(result);

	return len < (int)celt ? S_FALSE : S_OK;

  error:
	PyErr_Clear();	// just in case
	Py_DECREF(result);
	return PyCom_SetCOMErrorFromSimple(E_FAIL, IID_IEnumConnections);
}

STDMETHODIMP PyGEnumConnections::Skip(
            /* [in] */ ULONG celt)
{
	PY_GATEWAY_METHOD;
	return InvokeViaPolicy("Skip", NULL, "i", celt);
}

STDMETHODIMP PyGEnumConnections::Reset(void)
{
	PY_GATEWAY_METHOD;
	return InvokeViaPolicy("Reset");
}

STDMETHODIMP PyGEnumConnections::Clone(
            /* [out] */ IEnumConnections __RPC_FAR *__RPC_FAR *ppEnum)
{
	PY_GATEWAY_METHOD;
	PyObject * result;
	HRESULT hr = InvokeViaPolicy("Clone", &result);
	if ( FAILED(hr) )
		return hr;

	/*
	** Make sure we have the right kind of object: we should have some kind
	** of IUnknown subclass wrapped into a PyIUnknown instance.
	*/
	if ( !PyIBase::is_object(result, &PyIUnknown::type) )
	{
		/* the wrong kind of object was returned to us */
		Py_DECREF(result);
		return PyCom_SetCOMErrorFromSimple(E_FAIL, IID_IEnumConnections);
	}

	/*
	** Get the IUnknown out of the thing. note that the Python ob maintains
	** a reference, so we don't have to explicitly AddRef() here.
	*/
	IUnknown *punk = ((PyIUnknown *)result)->m_obj;
	if ( !punk )
	{
		/* damn. the object was released. */
		Py_DECREF(result);
		return PyCom_SetCOMErrorFromSimple(E_FAIL, IID_IEnumConnections);
	}

	/*
	** Get the interface we want. note it is returned with a refcount.
	** This QI is actually going to instantiate a PyGEnumConnections.
	*/
	Py_BEGIN_ALLOW_THREADS
	hr = punk->QueryInterface(IID_IEnumConnections, (LPVOID *)ppEnum);
	Py_END_ALLOW_THREADS

	/* done with the result; this DECREF is also for <punk> */
	Py_DECREF(result);

	return PyCom_SetCOMErrorFromSimple(hr, IID_IEnumConnections);
}
