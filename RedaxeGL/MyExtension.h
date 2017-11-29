#pragma once
#include <Python.h>
#include <iostream>
#include <string>
#include <vector>



class MyExtension
{
public:
	MyExtension(std::string path);

	void SetBaud(int _baud);

	bool Connect();


	void Reset();

	float GetRoll();
	float GetPitch();
	float GetYaw();

	~MyExtension();
	
	void IntialiseVarTuple();

private: 
	

	int StringToNumber(std::string s);

	float yaw, pitch, roll, analog_r_pin0;

	// Create some Python objects that will later be assigned values.
	PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue;

	float baud;

};

