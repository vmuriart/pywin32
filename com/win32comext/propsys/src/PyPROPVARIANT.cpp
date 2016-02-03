// This file implements the IPropertyStorage Interface and Gateway for Python.
// Generated by makegw.py

#include "PythonCOM.h"
#include "PyPROPVARIANT.h"
#include "propvarutil.h"
#include "float.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------


// @object PyPROPVARIANT|Encapsulates a PROPVARIANT structure.
// Constructed using PROPVARIANTType(Value, Type=VT_ILLEGAL).
// Value can be any object that can be be converted to the requested variant type.
// Type should be a combination of VARENUM values (pythoncom.VT_*).
// VT_ILLEGAL indicates that an appropriate variant type should be inferred from the Value.
// If the requested Type includes VT_VECTOR, Value should be a sequence of compatible objects.
// Currently VT_ARRAY and VT_BYREF are not supported, although some types can be coerced
// into a safearray using <om PyPROPVARIANT.ChangeType>.
PyTypeObject PyPROPVARIANTType =
{
	PYWIN_OBJECT_HEAD
	"PyPROPVARIANT",
	sizeof(PyPROPVARIANT),
	0,
	PyPROPVARIANT::deallocFunc,
	0,		/* tp_print */
	0,				/* tp_getattr */
	0,				/* tp_setattr */
	0,	/* tp_compare */
	0,						/* tp_repr */
	0,						/* tp_as_number */
	0,	/* tp_as_sequence */
	0,						/* tp_as_mapping */
	0,
	0,						/* tp_call */
	0,		/* tp_str */
	PyObject_GenericGetAttr, // PyPROPVARIANT::getattro,
	PyObject_GenericSetAttr, // PyPROPVARIANT::setattro,
	0,		// tp_as_buffer;
	Py_TPFLAGS_DEFAULT,		// tp_flags;
	0,		// tp_doc; /* Documentation string */
	0,		// traverseproc tp_traverse;
	0,		// tp_clear;
	0,		// tp_richcompare;
	0,		// tp_weaklistoffset;
	0,		// tp_iter
	0,		// iternextfunc tp_iternext
	PyPROPVARIANT::methods,
	PyPROPVARIANT::members,
	0,			// tp_getset
	0,			// tp_base
	0,			// tp_dict
	0,			// tp_descr_get
	0,			// tp_descr_set
	0,			// tp_dictoffset
	0,			// tp_init
	0,			// tp_alloc
	PyPROPVARIANT::tp_new
};

#define OFF(e) offsetof(PyPROPVARIANT, e)
struct PyMemberDef PyPROPVARIANT::members[] = {
	// @prop int|vt|The variant type, a combination of VARENUM values including flags.  (read only)
	{"vt", T_USHORT, OFF(Py_propvariant.vt), READONLY, "VARTYPE that includes variant type and flags"},
	{NULL}
};

struct PyMethodDef PyPROPVARIANT::methods[] = {
	// @pymeth GetValue|Returns an object representing the variant value
	{"GetValue", PyPROPVARIANT::GetValue, METH_NOARGS},
	// @pymeth ToString|Returns the value as a string
	{"ToString", PyPROPVARIANT::ToString, METH_NOARGS},
	// @pymeth ChangeType|Coerce to a different variant type
	{"ChangeType", PyPROPVARIANT::ChangeType, METH_VARARGS},
	{NULL}
};

PyPROPVARIANT::PyPROPVARIANT(PROPVARIANT *ppv)
{
	ob_type = &PyPROPVARIANTType;
	Py_propvariant=*ppv;
	ClearOnDestruction = TRUE;
	_Py_NewReference(this);
}

// Mostly for const PROPVARIANTS passed to COM servers, to avoid expensive copy operations
PyPROPVARIANT::PyPROPVARIANT(REFPROPVARIANT rpv)
{
	ob_type = &PyPROPVARIANTType;
	Py_propvariant=rpv;
	ClearOnDestruction = FALSE;
	_Py_NewReference(this);
}

PyPROPVARIANT::PyPROPVARIANT(void)
{
	ob_type =  &PyPROPVARIANTType;
	// ob_type->tp_members=PyPROPVARIANT::memberdef;
	PropVariantInit(&Py_propvariant);
	_Py_NewReference(this);
}

PyPROPVARIANT::~PyPROPVARIANT()
{
	if(!ClearOnDestruction)
		return;
	// will already have been cleared, need to know which type couldn't
	//	be deallocated correctly
	VARTYPE vt = Py_propvariant.vt;
	HRESULT hr = PropVariantClear(&Py_propvariant);
	// Allocation scheme is not well documented, take a look if deallocation failed
#ifdef Py_DEBUG
	if (FAILED(hr))
		DebugBreak();
#endif
}

BOOL PyWinObject_AsPROPVARIANT(PyObject *ob, PROPVARIANT **pppv)
{
	if (ob->ob_type!=&PyPROPVARIANTType){
		PyErr_SetString(PyExc_TypeError,"Object must be a PyPROPVARIANT");
		return FALSE;
		}
	*pppv=&((PyPROPVARIANT *)ob)->Py_propvariant;
	return TRUE;
}

PyObject *PyWinObject_FromPROPVARIANT(PROPVARIANT *ppv)
{
	PyObject *ret = new PyPROPVARIANT(ppv);
	if (ret==NULL)
		PyErr_NoMemory();
	return ret;
}

PyObject *PyWinObject_FromPROPVARIANT(REFPROPVARIANT rpv)
{
	PyObject *ret = new PyPROPVARIANT(rpv);
	if (ret==NULL)
		PyErr_NoMemory();
	return ret;
}

void PyPROPVARIANT::deallocFunc(PyObject *ob)
{
	delete (PyPROPVARIANT *)ob;
}

PyObject *PyPROPVARIANT::tp_new(PyTypeObject *tp, PyObject *args, PyObject *kwargs)
{
	VARTYPE vt = VT_ILLEGAL;
	PyObject *val;
	PROPVARIANT pv;
	PropVariantInit(&pv);

	static char *keywords[]={"Value", "Type", NULL};
	if (!PyArg_ParseTupleAndKeywords(args, kwargs, "O|H:PyPROPVARIANT", keywords, &val, &vt))
		return NULL;
	if (!PyWin_NewPROPVARIANT(val, vt, &pv))
		return NULL;
	return PyWinObject_FromPROPVARIANT(&pv);
}

// @pymethod object|PyPROPVARIANT|GetValue|Returns an object representing the variant value
PyObject* PyPROPVARIANT::GetValue(PyObject *self, PyObject *args)
{
	return PyObject_FromPROPVARIANT(&((PyPROPVARIANT *)self)->Py_propvariant);
}

// @pymethod str|PyPROPVARIANT|ToString|Returns the value as a string
// @pyseeapi PropVariantToString
PyObject* PyPROPVARIANT::ToString(PyObject *self, PyObject *args)
{
	PROPVARIANT pv = ((PyPROPVARIANT *)self)->Py_propvariant;
	WCHAR *s;
	HRESULT hr;
	hr=PropVariantToStringAlloc(pv, &s);
	if (FAILED(hr))
		return PyCom_BuildPyException(hr);
	PyObject *ret = PyWinObject_FromWCHAR(s);
	CoTaskMemFree(s);
	return ret;
}

// @pymethod <o PyPROPVARIANT>|PyPROPVARIANT|ChangeType|Coerce to a different variant type
// @pyseeapi PropVariantChangeType
PyObject* PyPROPVARIANT::ChangeType(PyObject *self, PyObject *args)
{
	VARTYPE vt;
	DWORD flags = 0;
	HRESULT hr;
	PROPVARIANT pv = ((PyPROPVARIANT *)self)->Py_propvariant;
	PROPVARIANT pvnew;
	PropVariantInit(&pvnew);
	// @pyparm int|Type||New variant type, combination of pythoncom.VT_* values
	// @pyparm int|Flags|0|Reserved (PROPVAR_CHANGE_FLAGS)
	if (!PyArg_ParseTuple(args, "H|k:ChangeType", &vt, &flags))
		return NULL;
	hr=PropVariantChangeType(&pvnew, pv, flags, vt);
	if (FAILED(hr))
		return PyCom_BuildPyException(hr);
	return PyWinObject_FromPROPVARIANT(&pvnew);
}

// Some conversion functions for specific variant types
// VT_I1
BOOL PyWinObject_AsCHAR(PyObject *ob, CHAR *pchar){
	long val = PyLong_AsLong(ob);
	if (val == -1 && PyErr_Occurred())
		return FALSE;
	if ((val > CHAR_MAX) || (val < CHAR_MIN)){
		PyErr_Format(PyExc_ValueError, "Value out of range for CHAR (VT_I1)");
		return FALSE;
		}
	*pchar = (CHAR)val;
	return TRUE;
}

// VT_UI1
BOOL PyWinObject_AsUCHAR(PyObject *ob, UCHAR *puchar){
	long val = PyLong_AsLong(ob);
	if (val == -1 && PyErr_Occurred())
		return FALSE;
	if ((val > UCHAR_MAX) || (val < 0)){
		PyErr_Format(PyExc_ValueError, "Value out of range for UCHAR (VT_UI1)");
		return FALSE;
		}
	*puchar = (UCHAR)val;
	return TRUE;
}

// VT_I2
BOOL PyWinObject_AsSHORT(PyObject *ob, PSHORT pshort){
	long val = PyLong_AsLong(ob);
	if (val == -1 && PyErr_Occurred())
		return FALSE;
	if ((val > SHRT_MAX) || (val < SHRT_MIN)){
		PyErr_Format(PyExc_ValueError, "Value out of range for SHORT (VT_I2)");
		return FALSE;
		}
	*pshort = (SHORT)val;
	return TRUE;
}

// VT_UI2
BOOL PyWinObject_AsUSHORT(PyObject *ob, PUSHORT pushort){
	long val = PyLong_AsLong(ob);
	if (val == -1 && PyErr_Occurred())
		return FALSE;
	if ((val > USHRT_MAX) || (val < 0)){
		PyErr_Format(PyExc_ValueError, "Value out of range for USHORT (VT_UI2)");
		return FALSE;
		}
		*pushort = (USHORT)val;
		return TRUE;
}

// VT_I4 and VT_INT
BOOL PyWinObject_AsLONG(PyObject *ob, PLONG plong){
	*plong = PyLong_AsLong(ob);
	return !(*plong == -1 && PyErr_Occurred());
}

// VT_UI4 and VT_UINT
BOOL PyWinObject_AsULONG(PyObject *ob, PULONG pulong){
	*pulong = PyLong_AsUnsignedLong(ob);
	return !(*pulong == -1 && PyErr_Occurred());
}

// VT_R4
BOOL PyWinObject_AsFLOAT(PyObject *ob, FLOAT *f){
	DOUBLE d = PyFloat_AsDouble(ob);
	if (d == -1 && PyErr_Occurred())
		return FALSE;
	if ((d > FLT_MAX) || (d < FLT_MIN)){
		PyErr_Format(PyExc_ValueError, "Value out of range for FLOAT (VT_R4)");
		return FALSE;
		}
	*f = (FLOAT)d;
	return TRUE;
}

// VT_R8
BOOL PyWinObject_AsDOUBLE(PyObject *ob, DOUBLE *d){
	*d = PyFloat_AsDouble(ob);
	return !(*d == -1 && PyErr_Occurred());
}

// VT_BOOL
BOOL PyWinObject_AsVARIANT_BOOL(PyObject *ob, VARIANT_BOOL *pb){
	*pb = PyObject_IsTrue(ob) ? VARIANT_TRUE : VARIANT_FALSE;
	return TRUE;
}

// Creates a new PROPVARIANT that must be freed with PropVariantClear.
// Used mostly as a helper for PyPROPVARIANT::tp_new.
BOOL PyWin_NewPROPVARIANT(PyObject *ob, VARTYPE vt, PROPVARIANT *ppv)
{
	BOOL ret = FALSE;
	PropVariantInit(ppv);
	VARTYPE type_only = vt & VT_TYPEMASK;
	VARTYPE flags = vt & ~VT_TYPEMASK;

	// use this to signal that variant type should be guessed from type of python object
	if (vt == VT_ILLEGAL){
		if (ob->ob_type == &PyPROPVARIANTType){
			// already a propvariant, take a copy
			HRESULT hr = PropVariantCopy(ppv, &((PyPROPVARIANT *)ob)->Py_propvariant);
			if (FAILED(hr)){
				PyCom_BuildPyException(hr);
				return FALSE;
				}
			return TRUE;
			}
		// Attempt to find a variant type that matches type of python object
		return PyObject_AsPROPVARIANT(ob, ppv);
		}

	ppv->vt=vt;
	switch (vt){
		case VT_EMPTY:
		case VT_NULL:
			if (ob != Py_None)
				PyErr_Format(PyExc_ValueError, "Value for this property type must be None");
			else
				ret = TRUE;
			break;
		case VT_I1:
			ret = PyWinObject_AsCHAR(ob, &ppv->cVal);
			break;
		case VT_I1|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cac.pElems, &ppv->cac.cElems, PyWinObject_AsCHAR);
			break;
		case VT_UI1:
			ret = PyWinObject_AsUCHAR(ob, &ppv->bVal);
			break;
		case VT_UI1|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->caub.pElems, &ppv->caub.cElems, PyWinObject_AsUCHAR);
			break;
		case VT_I2:
			ret = PyWinObject_AsSHORT(ob, &ppv->iVal);
			break;
		case VT_I2|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cai.pElems, &ppv->cai.cElems, PyWinObject_AsSHORT);
			break;
		case VT_UI2:
			ret = PyWinObject_AsUSHORT(ob, &ppv->uiVal);
			break;
		case VT_UI2|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->caui.pElems, &ppv->caui.cElems, PyWinObject_AsUSHORT);
			break;
		case VT_INT:
			ppv->intVal = PyLong_AsLong(ob);
			ret = !(ppv->intVal == -1 && PyErr_Occurred());
			break;
		case VT_UINT:
			ppv->uintVal = PyLong_AsUnsignedLong(ob);
			ret = !(ppv->uintVal == -1 && PyErr_Occurred());
			break;
		case VT_I4:
			ppv->lVal = PyLong_AsLong(ob);
			ret = !(ppv->lVal == -1 && PyErr_Occurred());
			break;
		case VT_I4|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cal.pElems, &ppv->cal.cElems, PyWinObject_AsLONG);
			break;
		case VT_UI4:
			ppv->ulVal = PyLong_AsUnsignedLong(ob);
			ret = !(ppv->ulVal == -1 && PyErr_Occurred());
			break;
		case VT_UI4|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->caul.pElems, &ppv->caul.cElems, PyWinObject_AsULONG);
			break;
		case VT_ERROR:
			ppv->scode = PyLong_AsLong(ob);
			ret = !(ppv->scode == -1 && PyErr_Occurred());
			break;
		case VT_ERROR|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cascode.pElems, &ppv->cascode.cElems, PyWinObject_AsLONG);
			break;
 		case VT_I8:
			ret = PyWinObject_AsLARGE_INTEGER(ob, &ppv->hVal);
			break;
		case VT_I8|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cah.pElems, &ppv->cah.cElems, PyWinObject_AsLARGE_INTEGER);
			break;
		case VT_UI8:
			ret = PyWinObject_AsULARGE_INTEGER(ob, &ppv->uhVal);
			break;
		case VT_UI8|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cauh.pElems, &ppv->cauh.cElems, PyWinObject_AsULARGE_INTEGER);
			break;
		case VT_LPWSTR:
			ret = PyWinObject_AsTaskAllocatedWCHAR(ob, &ppv->pwszVal, FALSE, NULL);
			break;
		case VT_LPWSTR|VT_VECTOR:{
				TmpPyObject seq = PyWinSequence_Tuple(ob, &ppv->calpwstr.cElems);
				if (seq == NULL)
					break;
				// For VT_VECTOR, PropVariantClear deallocates pointer arrays using CoTaskMemFree
				ppv->calpwstr.pElems = (LPWSTR *)CoTaskMemAlloc(ppv->calpwstr.cElems * sizeof(LPWSTR));
				if (ppv->calpwstr.pElems == NULL){
					PyErr_NoMemory();
					break;
					}
				ret = TRUE;
				for (ULONG i=0; i<ppv->calpwstr.cElems; i++){
					PyObject *obstr = PyTuple_GET_ITEM((PyObject *)seq, i);
					ret = PyWinObject_AsTaskAllocatedWCHAR(obstr, &ppv->calpwstr.pElems[i], FALSE, NULL);
					if (!ret)
						break;
					}
				}
			break;
		case VT_BSTR:
			ret = PyWinObject_AsBstr(ob, &ppv->bstrVal);
			break;
		case VT_BSTR|VT_VECTOR:{
				TmpPyObject seq = PyWinSequence_Tuple(ob, &ppv->cabstr.cElems);
				if (seq == NULL)
					break;
				ppv->cabstr.pElems = (BSTR *)CoTaskMemAlloc(ppv->cabstr.cElems * sizeof(BSTR));
				if (ppv->cabstr.pElems == NULL){
					PyErr_NoMemory();
					break;
					}
				ret = TRUE;
				for (ULONG i=0; i<ppv->cabstr.cElems; i++){
					PyObject *obstr = PyTuple_GET_ITEM((PyObject *)seq, i);
					ret = PyWinObject_AsBstr(obstr, &ppv->cabstr.pElems[i]);
					if (!ret)
						break;
					}
				}
			break;
		case VT_R4:
			ret = PyWinObject_AsFLOAT(ob, &ppv->fltVal);
			break;
		case VT_R4|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->caflt.pElems, &ppv->caflt.cElems, PyWinObject_AsFLOAT);
			break;
		case VT_R8:
			ret = PyWinObject_AsDOUBLE(ob, &ppv->dblVal);
			break;
		case VT_R8|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cadbl.pElems, &ppv->cadbl.cElems, PyWinObject_AsDOUBLE);
			break;
		case VT_CY:
			ret = PyObject_AsCurrency(ob, &ppv->cyVal);
			break;
		case VT_CY|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cacy.pElems, &ppv->cacy.cElems, PyObject_AsCurrency);
			break;
		case VT_BOOL:
			ret = PyWinObject_AsVARIANT_BOOL(ob, &ppv->boolVal);
			break;
		case VT_BOOL|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cabool.pElems, &ppv->cabool.cElems, PyWinObject_AsVARIANT_BOOL);
			break;
		case VT_DATE:
			ret = PyWinObject_AsDATE(ob, &ppv->date);
			break;
		case VT_DATE|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cadate.pElems, &ppv->cadate.cElems, PyWinObject_AsDATE);
			break;
		case VT_FILETIME:
			ret = PyWinObject_AsFILETIME(ob, &ppv->filetime);
			break;
		case VT_FILETIME|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cafiletime.pElems, &ppv->cafiletime.cElems, PyWinObject_AsFILETIME);
			break;
		case VT_CLSID:
			// There's only a pointer for CLSID, and msdn doesn't specify how it should be allocated.
			// PropVariantClear apparently deallocates it with CoTaskMemFree, as this doesn't leak.
			ppv->puuid = (CLSID *)CoTaskMemAlloc(sizeof(CLSID));
			if (ppv->puuid == NULL)
				PyErr_NoMemory();
			else
				ret = PyWinObject_AsIID(ob, ppv->puuid);
			break;
		case VT_CLSID|VT_VECTOR:
			ret = SeqToVector(ob, &ppv->cauuid.pElems, &ppv->cauuid.cElems, PyWinObject_AsIID);
			break;
		case VT_STREAM:
		case VT_STREAMED_OBJECT:
			ret = PyCom_InterfaceFromPyObject(ob, IID_IStream, (void **)&ppv->pStream, FALSE);
			break;
		// case VT_VERSIONED_STREAM :
		//	ret = PyCom_InterfaceFromPyObject(ob, IID_IStream, (void **)&ppv->pVersionedStream, FALSE);
		//	break;
		case VT_STORAGE:
		case VT_STORED_OBJECT:
			ret = PyCom_InterfaceFromPyObject(ob, IID_IStorage, (void **)&ppv->pStorage, FALSE);
			break;
		case VT_BLOB:
		case VT_BLOB_OBJECT:{
			void *buf;
			DWORD buflen;
			ret = PyWinObject_AsReadBuffer(ob, &buf, &buflen, FALSE);
			if (ret){
				ppv->blob.cbSize=buflen;
				ppv->blob.pBlobData=(BYTE *)CoTaskMemAlloc(buflen);
				if (ppv->blob.pBlobData == NULL){
					PyErr_NoMemory();
					ret = FALSE;
					}
				else
					memcpy(ppv->blob.pBlobData, buf, buflen);
				}
			break;
			}
		case VT_UNKNOWN:
			ret = PyCom_InterfaceFromPyObject(ob, IID_IUnknown, (void **)&ppv->punkVal, FALSE);
			break;
		case VT_DISPATCH:
			ret = PyCom_InterfaceFromPyObject(ob, IID_IDispatch, (void **)&ppv->pdispVal, FALSE);
			break;
		case VT_VARIANT|VT_VECTOR:{
				TmpPyObject seq = PyWinSequence_Tuple(ob, &ppv->capropvar.cElems);
				if (seq == NULL)
					break;
				ppv->capropvar.pElems = (PROPVARIANT *)CoTaskMemAlloc(ppv->capropvar.cElems * sizeof(PROPVARIANT));
				if (ppv->capropvar.pElems == NULL){
					PyErr_NoMemory();
					break;
					}
				ret = TRUE;
				for (ULONG i=0; i<ppv->capropvar.cElems; i++){
					PyObject *obpv = PyTuple_GET_ITEM((PyObject *)seq, i);
					ret = PyWin_NewPROPVARIANT(obpv, VT_ILLEGAL, &ppv->capropvar.pElems[i]);
					if (!ret)
						break;
					}
				break;
				}
			// Docs are contradictory as to whether VT_VARIANT can be used with VT_ARRAY
			// http://msdn.microsoft.com/en-us/library/aa380072%28VS.85%29.aspx
			// In the section on VT_ARRAY, it says it can be or'ed with VT_VARIANT.  However,
			// under VT_VARIANT it says it can only be used with VT_VECTOR or VT_BYREF

		default:
			PyErr_Format(PyExc_TypeError, "Unsupported property type 0x%x", ppv->vt);
			ret = FALSE;
		}
	// Some conversions may have already done allocations before failing
	if (!ret)
		PropVariantClear(ppv);
	return ret;
}
