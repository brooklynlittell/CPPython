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
	printf("Hello, world!");
	

	return 0;
}