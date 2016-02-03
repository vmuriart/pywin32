// Requires Windows 7 SDK to build
#if WINVER >= 0x0601
// This file declares the IApplicationDestinations Interface for Python.
// Generated by makegw.py
// ---------------------------------------------------
//
// Interface Declaration

class PyIApplicationDestinations : public PyIUnknown
{
public:
	MAKE_PYCOM_CTOR(PyIApplicationDestinations);
	static IApplicationDestinations *GetI(PyObject *self);
	static PyComTypeObject type;

	// The Python methods
	static PyObject *SetAppID(PyObject *self, PyObject *args);
	static PyObject *RemoveDestination(PyObject *self, PyObject *args);
	static PyObject *RemoveAllDestinations(PyObject *self, PyObject *args);

protected:
	PyIApplicationDestinations(IUnknown *pdisp);
	~PyIApplicationDestinations();
};

#endif // WINVER
