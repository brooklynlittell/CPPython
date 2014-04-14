#ifdef _DEBUG
#undef _DEBUG
#endif //_DEBUG
#include <Python.h>
#ifndef _DEBUG
#define _DEBUG
#endif //_DEBUG
/*
#include <stdio.h>


int main( int argc, char *argv[] )
{
	Py_SetProgramName(argv[0]);  // optional but recommended 
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
		"print 'Today is',ctime(time())\n");
	Py_Finalize();
	
	system("pause");

	return 0;
}
*/

#include <stdio.h>

int main(int argc, char *argv[])
{
	int myInt = 0;

	PyObject *pName, *pModule, *pDict, *pFunc;
	PyObject *pArgs, *pValue;
	int i;

	if (argc < 3) {
		fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
		return 1;
	}

	Py_Initialize();
	pName = PyString_FromString(argv[1]);
	/* Error checking of pName left out */

	pModule = PyImport_Import(pName);
	Py_DECREF(pName);

	if (pModule != NULL) {
		pFunc = PyObject_GetAttrString(pModule, argv[2]);
		/* pFunc is a new reference */

		if (pFunc && PyCallable_Check(pFunc)) {
			pArgs = PyTuple_New(argc - 3);
			for (i = 0; i < argc - 3; ++i) {
				pValue = PyInt_FromLong(atoi(argv[i + 3]));
				if (!pValue) {
					Py_DECREF(pArgs);
					Py_DECREF(pModule);
					fprintf(stderr, "Cannot convert argument\n");
					return 1;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem(pArgs, i, pValue);
			}
			pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);
			if (pValue != NULL) {
				// Float
				if(PyFloat_Check(pValue))
				{
					printf("Result of call float: %f\n", PyFloat_AsDouble(pValue));
				}
				// int
				else if( PyInt_Check(pValue) )
				{
					printf("Result of call int: %ld\n", PyInt_AsLong(pValue));
					myInt = PyInt_AsLong(pValue);
				}
				// Everything else
				else
				{
					printf("Result of call string?: %s\n", PyString_AsString(pValue));
				}
				
				Py_DECREF(pValue);
			}
			else {
				Py_DECREF(pFunc);
				Py_DECREF(pModule);
				PyErr_Print();
				fprintf(stderr,"Call failed\n");
				return 1;
			}
		}
		else {
			if (PyErr_Occurred())
				PyErr_Print();
			fprintf(stderr, "Cannot find function \"%s\"\n", argv[2]);
		}
		Py_XDECREF(pFunc);
		Py_DECREF(pModule);
	}
	else {
		PyErr_Print();
		fprintf(stderr, "Failed to load \"%s\"\n", argv[1]);
		return 1;
	}
	Py_Finalize();

	printf( "Value of myInt: %d\n", myInt);

	system("pause");
	return 0;
}