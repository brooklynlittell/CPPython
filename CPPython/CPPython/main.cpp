#ifdef _DEBUG
#undef _DEBUG
#endif //_DEBUG
#include <Python.h>
#ifndef _DEBUG
#define _DEBUG
#endif //_DEBUG

#include <stdio.h>


int main( int argc, char *argv[] )
{
	Py_SetProgramName(argv[0]);  /* optional but recommended */
	Py_Initialize();
	PyRun_SimpleString("from time import time,ctime\n"
		"print 'Today is',ctime(time())\n");
	Py_Finalize();
	
	system("pause");

	return 0;
}