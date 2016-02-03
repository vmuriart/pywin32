// This file implements the IEnumIDList Interface and Gateway for Python.
// Generated by makegw.py

#include "shell_pch.h"
#include "PyIEnumIDList.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyIEnumIDList::PyIEnumIDList(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyIEnumIDList::~PyIEnumIDList()
{
}

/* static */ IEnumIDList *PyIEnumIDList::GetI(PyObject *self)
{
	return (IEnumIDList *)PyIUnknown::GetI(self);
}

// @pymethod object|PyIEnumIDList|Next|Retrieves a specified number of items in the enumeration sequence.
PyObject *PyIEnumIDList::Next(PyObject *self, PyObject *args)
{
	long celt = 1;
	// @pyparm int|num|1|Number of items to retrieve.
	if ( !PyArg_ParseTuple(args, "|l:Next", &celt) )
		return NULL;

	IEnumIDList *peidl = GetI(self);
	if ( peidl == NULL )
		return NULL;

	LPITEMIDLIST *rgVar = new LPITEMIDLIST [celt];
	if ( rgVar == NULL ) {
		PyErr_SetString(PyExc_MemoryError, "allocating result ITEMIDLISTs");
		return NULL;
	}
	memset(rgVar, 0, sizeof(ITEMIDLIST *) * celt);

	int i;
/*	for ( i = celt; i--; )
		// *** possibly init each structure element???
*/

	ULONG celtFetched = 0;
	PY_INTERFACE_PRECALL;
	HRESULT hr = peidl->Next(celt, rgVar, &celtFetched);
	PY_INTERFACE_POSTCALL;
	if (  HRESULT_CODE(hr) != ERROR_NO_MORE_ITEMS && FAILED(hr) )
	{
		delete [] rgVar;
		return PyCom_BuildPyException(hr,peidl, IID_IEnumIDList);
	}

	PyObject *result = PyList_New(celtFetched);
	if ( result != NULL )
	{
		for ( i = celtFetched; i--; )
		{
			PyObject *ob = PyObject_FromPIDL(rgVar[i], TRUE);
			if ( ob == NULL )
			{
				Py_DECREF(result);
				result = NULL;
				break;
			}
			PyList_SET_ITEM(result, i, ob);
		}
	}

	// Each item free'd by PyObject_FromPIDL'd TRUE param
	delete [] rgVar;
	return result;
}

// @pymethod |PyIEnumIDList|Skip|Skips over the next specified elementes.
PyObject *PyIEnumIDList::Skip(PyObject *self, PyObject *args)
{
	long celt;
	if ( !PyArg_ParseTuple(args, "l:Skip", &celt) )
		return NULL;

	IEnumIDList *peidl = GetI(self);
	if ( peidl == NULL )
		return NULL;

	PY_INTERFACE_PRECALL;
	HRESULT hr = peidl->Skip(celt);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, peidl, IID_IEnumIDList);

	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod |PyIEnumIDList|Reset|Resets the enumeration sequence to the beginning.
PyObject *PyIEnumIDList::Reset(PyObject *self, PyObject *args)
{
	if ( !PyArg_ParseTuple(args, ":Reset") )
		return NULL;

	IEnumIDList *peidl = GetI(self);
	if ( peidl == NULL )
		return NULL;

	PY_INTERFACE_PRECALL;
	HRESULT hr = peidl->Reset();
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, peidl, IID_IEnumIDList);

	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod <o PyIEnumIDList>|PyIEnumIDList|Clone|Creates another enumerator that contains the same enumeration state as the current one
PyObject *PyIEnumIDList::Clone(PyObject *self, PyObject *args)
{
	if ( !PyArg_ParseTuple(args, ":Clone") )
		return NULL;

	IEnumIDList *peidl = GetI(self);
	if ( peidl == NULL )
		return NULL;

	IEnumIDList *pClone;
	PY_INTERFACE_PRECALL;
	HRESULT hr = peidl->Clone(&pClone);
	PY_INTERFACE_POSTCALL;
	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, peidl, IID_IEnumIDList);

	return PyCom_PyObjectFromIUnknown(pClone, IID_IEnumIDList, FALSE);
}

// @object PyIEnumIDList|A Python interface to IEnumIDList
static struct PyMethodDef PyIEnumIDList_methods[] =
{
	{ "Next", PyIEnumIDList::Next, 1 },    // @pymeth Next|Retrieves a specified number of items in the enumeration sequence.
	{ "Skip", PyIEnumIDList::Skip, 1 },	// @pymeth Skip|Skips over the next specified elementes.
	{ "Reset", PyIEnumIDList::Reset, 1 },	// @pymeth Reset|Resets the enumeration sequence to the beginning.
	{ "Clone", PyIEnumIDList::Clone, 1 },	// @pymeth Clone|Creates another enumerator that contains the same enumeration state as the current one.
	{ NULL }
};

PyComEnumTypeObject PyIEnumIDList::type("PyIEnumIDList",
		&PyIUnknown::type,
		sizeof(PyIEnumIDList),
		PyIEnumIDList_methods,
		GET_PYCOM_CTOR(PyIEnumIDList));

// ---------------------------------------------------
//
// Gateway Implementation

STDMETHODIMP PyGEnumIDList::Next(
            /* [in] */ ULONG celt,
            /* [length_is][size_is][out] */ LPITEMIDLIST *pi,
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

		if ( !PyObject_AsPIDL(ob, &pi[i] ))
		{
			Py_DECREF(result);
			return PyCom_SetCOMErrorFromPyException(IID_IEnumIDList);
		}
	}

	Py_DECREF(result);

	return len < (int)celt ? S_FALSE : S_OK;

  error:
	PyErr_Clear();	// just in case
	Py_DECREF(result);
	return PyCom_SetCOMErrorFromSimple(E_FAIL, IID_IEnumIDList, "Next() did not return a sequence of objects");
}

STDMETHODIMP PyGEnumIDList::Skip(
            /* [in] */ ULONG celt)
{
	PY_GATEWAY_METHOD;
	return InvokeViaPolicy("Skip", NULL, "i", celt);
}

STDMETHODIMP PyGEnumIDList::Reset(void)
{
	PY_GATEWAY_METHOD;
	return InvokeViaPolicy("Reset");
}

STDMETHODIMP PyGEnumIDList::Clone(
            /* [out] */ IEnumIDList __RPC_FAR *__RPC_FAR *ppEnum)
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
		return PyCom_SetCOMErrorFromSimple(E_FAIL, IID_IEnumIDList);
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
		return PyCom_SetCOMErrorFromSimple(E_FAIL, IID_IEnumIDList);
	}

	/*
	** Get the interface we want. note it is returned with a refcount.
	** This QI is actually going to instantiate a PyGEnumIDList.
	*/
	Py_BEGIN_ALLOW_THREADS
	hr = punk->QueryInterface(IID_IEnumIDList, (LPVOID *)ppEnum);
	Py_END_ALLOW_THREADS

	/* done with the result; this DECREF is also for <punk> */
	Py_DECREF(result);

	return PyCom_SetCOMErrorFromSimple(hr, IID_IEnumIDList, "Python could not convert the result from Next() into the required COM interface");
}
