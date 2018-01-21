#pragma once
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
#else
#include <Python.h>
#endif

#include <iostream>
#include <string>
#include <vector>





class MyExtension
{
public:
	MyExtension(const std::string& path);

	void SetBaud(long _baud);

	bool Connect();


	void Reset();

	float GetRoll();
	float GetPitch();
	float GetYaw();

	~MyExtension();
	
	void IntialiseVarTuple();

private: 
	
	std::string _path;

	float StringToNumber(std::string s);

	float yaw, pitch, roll, analog_r_pin0;

	// Create some Python objects that will later be assigned values.
	PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue;

	long baud;

};

