#ifdef _DEBUG
#undef _DEBUG
#endif //_DEBUG
#include <Python.h>
#ifndef _DEBUG
#define _DEBUG
#endif //_DEBUG

#include <stdio.h>
#include <string>

using namespace std;

/*
// String execution demo
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

struct PythonModule
{
public:
	~PythonModule()
	{
		Py_DECREF( module );
	}

	static void initializePython()
	{
		Py_Initialize();
	}

	static void shutdownPython()
	{
		Py_Finalize();
	}

	void loadModule( string name )
	{
		auto pName = PyString_FromString( name.c_str() );
		module = PyImport_Import( pName );
		Py_DECREF( pName );
	}

	void callFunc( string funcName, char** args, unsigned int argCount )
	{
		if( module == NULL )
			return;

		auto pFunc = PyObject_GetAttrString( module, funcName.c_str() );
		/* pFunc is a new reference */

		if (pFunc && PyCallable_Check(pFunc))
		{
			auto pArgs = PyTuple_New( argCount );
			for( unsigned int i = 0; i < argCount; ++i )
			{
				auto pValue = PyInt_FromLong( atoi( args[ i ] ) );
				if( !pValue )
				{
					Py_DECREF( pArgs );
					fprintf( stderr, "Cannot convert argument\n" );
					return;
				}
				/* pValue reference stolen here: */
				PyTuple_SetItem( pArgs, i, pValue );
			}

			auto pValue = PyObject_CallObject(pFunc, pArgs);
			Py_DECREF(pArgs);

			if( pValue != NULL )
			{
				// Float
				if( PyFloat_Check( pValue ) )
				{
					printf( "Result of call float: %f\n", PyFloat_AsDouble( pValue ) );
				}
				// int
				else if( PyInt_Check( pValue ) )
				{
					printf( "Result of call int: %ld\n", PyInt_AsLong( pValue ) );
				}
				else if( PyBool_Check( pValue ) )
				{
					printf( "Result of call bool: %b\n", pValue == Py_True );
				}
				// Everything else
				else
				{
					printf( "Result of call string?: %s\n", PyString_AsString( pValue ) );
				}
				
				Py_DECREF( pValue );
			}
			else
			{
				Py_DECREF( pFunc );
				PyErr_Print();
				fprintf( stderr, "Call failed\n" );
				return;
			}
		}
		else {
			if( PyErr_Occurred() )
				PyErr_Print();
			fprintf( stderr, "Cannot find function \"%s\"\n", funcName.c_str() );
		}
		Py_XDECREF( pFunc );
	}

private:
	PyObject* module;
};

// Command line loading demo
int main(int argc, char *argv[])
{
	Py_SetProgramName(argv[0]);  // optional but recommended

	if (argc < 3) 
	{
		fprintf(stderr,"Usage: call pythonfile funcname [args]\n");
		return 1;
	}
	
	PythonModule::initializePython();

	PythonModule mod;
	mod.loadModule( string( argv[ 1 ] ) );
	mod.callFunc( string( argv[ 2 ] ), argv + 3, argc - 3 );

	PythonModule::shutdownPython();

	system("pause");
	return 0;
}