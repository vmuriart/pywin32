// This file implements the ITaskTrigger Interface for Python.
// Generated by makegw.py

#include "PyITaskTrigger.h"

// @doc - This file contains autoduck documentation
// ---------------------------------------------------
//
// Interface Implementation

PyITaskTrigger::PyITaskTrigger(IUnknown *pdisp):
	PyIUnknown(pdisp)
{
	ob_type = &type;
}

PyITaskTrigger::~PyITaskTrigger()
{
}

/* static */ ITaskTrigger *PyITaskTrigger::GetI(PyObject *self)
{
	return (ITaskTrigger *)PyIUnknown::GetI(self);
}

// @pymethod |PyITaskTrigger|SetTrigger|Set trigger parameters from a PyTASK_TRIGGER object
PyObject *PyITaskTrigger::SetTrigger(PyObject *self, PyObject *args)
{
	ITaskTrigger *pITT = GetI(self);
	if ( pITT == NULL )
		return NULL;
	PyObject *obtt;
	// @pyparm <o PyTASK_TRIGGER>|Trigger||Python object representing a TASK_TRIGGER struct
	if ( !PyArg_ParseTuple(args, "O:PyITaskTrigger::SetTrigger",&obtt))
		return NULL;
	if (!PyTASK_TRIGGER_check(obtt))
		return NULL;

	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITT->SetTrigger(&((PyTASK_TRIGGER *)obtt)->task_trigger);
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		return PyCom_BuildPyException(hr, pITT, IID_ITaskTrigger );
	Py_INCREF(Py_None);
	return Py_None;
}

// @pymethod <o PyTASK_TRIGGER>|PyITaskTrigger|GetTrigger|Retrieves trigger parms as a PyTASK_TRIGGER object
PyObject *PyITaskTrigger::GetTrigger(PyObject *self, PyObject *args)
{
	TASK_TRIGGER tt;
	ITaskTrigger *pITT = GetI(self);
	if ( pITT == NULL )
		return NULL;
	if ( !PyArg_ParseTuple(args, ":PyITaskTrigger::GetTrigger"))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITT->GetTrigger(&tt);
	PY_INTERFACE_POSTCALL;

	if (FAILED(hr))
		return PyCom_BuildPyException(hr, pITT, IID_ITaskTrigger);
	return new PyTASK_TRIGGER(&tt);
}

// @pymethod <o PyUnicode>|PyITaskTrigger|GetTriggerString|Build text summary of trigger
PyObject *PyITaskTrigger::GetTriggerString(PyObject *self, PyObject *args)
{
	PyObject *ret=NULL;
	ITaskTrigger *pITT = GetI(self);
	if ( pITT == NULL )
		return NULL;
	LPWSTR TriggerString;
	if ( !PyArg_ParseTuple(args, ":PyITaskTrigger::GetTriggerString"))
		return NULL;
	HRESULT hr;
	PY_INTERFACE_PRECALL;
	hr = pITT->GetTriggerString(&TriggerString);
	PY_INTERFACE_POSTCALL;

	if ( FAILED(hr) )
		ret=PyCom_BuildPyException(hr, pITT, IID_ITaskTrigger);
	else
		ret=PyWinObject_FromWCHAR(TriggerString);
	CoTaskMemFree(TriggerString);
	return ret;
}

// @object PyITaskTrigger|Python object that encapsulates the ITaskTrigger interface
static struct PyMethodDef PyITaskTrigger_methods[] =
{
	{ "SetTrigger", PyITaskTrigger::SetTrigger, 1 }, // @pymeth SetTrigger|Set trigger parameters from a PyTASK_TRIGGER object
	{ "GetTrigger", PyITaskTrigger::GetTrigger, 1 }, // @pymeth GetTrigger|Retrieves trigger parms as a PyTASK_TRIGGER object
	{ "GetTriggerString", PyITaskTrigger::GetTriggerString, 1 }, // @pymeth GetTriggerString|Build text summary of trigger
	{ NULL }
};

PyComTypeObject PyITaskTrigger::type("PyITaskTrigger",
		&PyIUnknown::type,
		sizeof(PyITaskTrigger),
		PyITaskTrigger_methods,
		GET_PYCOM_CTOR(PyITaskTrigger));

static struct PyMethodDef PyTASK_TRIGGER_methods[] =
{
	{ NULL }
};

#define OFF(e) offsetof(PyTASK_TRIGGER, e)
static struct PyMemberDef PyTASK_TRIGGER_members[] = {
	{"Reserved1", T_USHORT, OFF(task_trigger.Reserved1), 0, "Reserved, do not use"},
	{"Reserved2", T_USHORT, OFF(task_trigger.Reserved2), 0, "Reserved, do not use"},
	{"BeginYear", T_USHORT, OFF(task_trigger.wBeginYear), 0, NULL},
	{"BeginMonth", T_USHORT, OFF(task_trigger.wBeginMonth), 0, NULL},
	{"BeginDay", T_USHORT, OFF(task_trigger.wBeginDay), 0, NULL},
	{"EndYear", T_USHORT, OFF(task_trigger.wEndYear), 0, NULL},
	{"EndMonth", T_USHORT, OFF(task_trigger.wEndMonth), 0, NULL},
	{"EndDay", T_USHORT, OFF(task_trigger.wEndDay), 0, NULL},
	{"StartHour", T_USHORT, OFF(task_trigger.wStartHour), 0, NULL},
	{"StartMinute", T_USHORT, OFF(task_trigger.wStartMinute), 0, NULL},
	{"MinutesDuration", T_ULONG, OFF(task_trigger.MinutesDuration), 0, NULL},
	{"MinutesInterval", T_ULONG, OFF(task_trigger.MinutesInterval), 0, NULL},
	{"RandomMinutesInterval", T_USHORT, OFF(task_trigger.wRandomMinutesInterval), 0, NULL},
	{"Flags", T_ULONG, OFF(task_trigger.rgFlags), 0, "Combination of TASK_TRIGGER_FLAG_HAS_END_DATE,TASK_TRIGGER_FLAG_KILL_AT_DURATION_END,TASK_TRIGGER_FLAG_DISABLED"},
	{"TriggerType", T_ULONG, OFF(task_trigger.TriggerType), 0, "Value from TASK_TRIGGER_TYPE enum:\n"
			"TASK_TIME_TRIGGER_ONCE,TASK_TIME_TRIGGER_DAILY,\n"
			"TASK_TIME_TRIGGER_WEEKLY,TASK_TIME_TRIGGER_MONTHLYDATE,\n"
			"TASK_TIME_TRIGGER_MONTHLYDOW,TASK_EVENT_TRIGGER_ON_IDLE,\n"
			"TASK_EVENT_TRIGGER_AT_SYSTEMSTART,TASK_EVENT_TRIGGER_AT_LOGON"},
	{"Daily_DaysInterval", T_USHORT, OFF(task_trigger.Type.Daily.DaysInterval),0,"TASK_TIME_TRIGGER_DAILY"},
	{"Weekly_WeeksInterval", T_USHORT, OFF(task_trigger.Type.Weekly.WeeksInterval),0,"TASK_TIME_TRIGGER_WEEKLY"},
	{"Weekly_DaysOfTheWeek", T_USHORT, OFF(task_trigger.Type.Weekly.rgfDaysOfTheWeek),0,"TASK_TIME_TRIGGER_WEEKLY"},
	{"MonthlyDate_Days", T_ULONG, OFF(task_trigger.Type.MonthlyDate.rgfDays),0,"TASK_TIME_TRIGGER_MONTHLYDATE"},
	{"MonthlyDate_Months", T_USHORT, OFF(task_trigger.Type.MonthlyDate.rgfMonths),0,"TASK_TIME_TRIGGER_MONTHLYDATE"},
	{"MonthlyDOW_WhichWeek", T_USHORT, OFF(task_trigger.Type.MonthlyDOW.wWhichWeek),0,"Only used for TASK_TIME_TRIGGER_MONTHLYDOW"},
	{"MonthlyDOW_DaysOfTheWeek", T_USHORT, OFF(task_trigger.Type.MonthlyDOW.rgfDaysOfTheWeek),0,"Only used for TASK_TIME_TRIGGER_MONTHLYDOW"},
	{"MonthlyDOW_Months", T_USHORT, OFF(task_trigger.Type.MonthlyDOW.rgfMonths),0,"Only used for TASK_TIME_TRIGGER_MONTHLYDOW"},
	{NULL}
};

// @object PyTASK_TRIGGER|Python object representing a TASK_TRIGGER structure via the structmember Api
PyTypeObject PyTASK_TRIGGERType =
{
	PYWIN_OBJECT_HEAD
	"PyTASK_TRIGGER",
	sizeof(PyTASK_TRIGGER),
	0,
	PyTASK_TRIGGER::deallocFunc,
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
	PyObject_GenericGetAttr, // PyTASK_TRIGGER::getattro,
	PyObject_GenericSetAttr, // PyTASK_TRIGGER::setattro,
	0,		// tp_as_buffer;
	Py_TPFLAGS_DEFAULT,		// tp_flags;
	0,		// tp_doc; /* Documentation string */
	0,		// traverseproc tp_traverse;
	0,		// tp_clear;
	0,		// tp_richcompare;
	0,		// tp_weaklistoffset;
	0,		// tp_iter
	0,		// iternextfunc tp_iternext
	PyTASK_TRIGGER_methods,
	PyTASK_TRIGGER_members
};

PyTASK_TRIGGER::PyTASK_TRIGGER(PTASK_TRIGGER ptt)
{
	ob_type = &PyTASK_TRIGGERType;
	task_trigger=*ptt;
	task_trigger.cbTriggerSize=sizeof(TASK_TRIGGER);
	_Py_NewReference(this);
}

PyTASK_TRIGGER::PyTASK_TRIGGER(void)
{
	ob_type =  &PyTASK_TRIGGERType;
	// ob_type->tp_members=PyTASK_TRIGGER::memberdef;
	ZeroMemory(&task_trigger,sizeof(TASK_TRIGGER));
	task_trigger.cbTriggerSize=sizeof(TASK_TRIGGER);
	_Py_NewReference(this);
}

PyTASK_TRIGGER::~PyTASK_TRIGGER()
{
}

BOOL PyTASK_TRIGGER_check(PyObject *ob)
{
	if (ob->ob_type!=&PyTASK_TRIGGERType){
		PyErr_SetString(PyExc_TypeError,"Object must be a PyTASK_TRIGGER");
		return FALSE;
		}
	return TRUE;
}

void PyTASK_TRIGGER::deallocFunc(PyObject *ob)
{
	delete (PyTASK_TRIGGER *)ob;
}


