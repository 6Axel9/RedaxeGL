#include "MyExtension.h"
#include <iostream>
#include <string>
#include <sstream>
#include <thread>

MyExtension::MyExtension(const std::string& path):_path(path)
{

	// Initialize the Python interpreter.
	Py_Initialize();

	// Convert the file name to a Python string.
	pName = PyString_FromString(_path.c_str());

	// Import the file as a Python module.
	pModule = PyImport_Import(pName);

	// Create a dictionary for the contents of the module.
	pDict = PyModule_GetDict(pModule);

	baud = 9600;

	scale = 1000.f;

	connected = false;

	yaw = 0, pitch = 0, roll = 0, analog_r_pin0 = 0;

	Connecting_thread = new std::thread(&MyExtension::Connect, this);
	Connecting_thread->detach();
}

void MyExtension::SetBaud(long _baud)
{
	baud = _baud;
}


void MyExtension::Connect()
{
	// Get the choosen method from the dictionary.
	pFunc = PyDict_GetItemString(pDict, "Serial_connect");

	// Create a Python tuple to hold the arguments to the method.
	pArgs = PyTuple_New(1);

	// Convert to a Python integer.
	pValue = PyInt_FromLong(baud);

	// Set the Python int as the first and second arguments to the method.
	PyTuple_SetItem(pArgs, 0, pValue);

	PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

	pResult == NULL ? connected = false: connected = true;

	if (connected && Connecting_thread->joinable())
	{
		std::cout << "Connected \n";
		Connecting_thread->join();
		delete Connecting_thread;
		//delete Connecting_thread;
	}

}


void MyExtension::IntialiseVarTuple()
{
	// Get the choosen method from the dictionary.
	pFunc = PyDict_GetItemString(pDict, "GetVars");

	// Create a Python tuple to hold the arguments to the method.
	pArgs = NULL;

	PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

	std::string result;
	std::string v1, v2, v3;

	// Print a message if calling the method failed.
	if (pResult == NULL)
	{
		std::cout << "Method failed\n";

		connected = false;

		Connecting_thread = new std::thread(&MyExtension::Connect, this);
		Connecting_thread->detach();
		//Connecting_thread = new std::thread(&MyExtension::Connect, this);
		//Connecting_thread->detach();

		yaw = 0, pitch = 0, roll = 0, analog_r_pin0 = 0;
		

		return;
	}


	result = PyString_AsString(pResult);


	v1 = result.substr(result.find("b") + 1, result.find("e") - 1);
	analog_r_pin0 = StringToNumber(v1);
	result.erase(0, result.find("e") + 1);
		

	v2 = result.substr(result.find("b") + 1, result.find("e") - 1);
	roll = StringToNumber(v2);
	result.erase(0 , result.find("e") + 1);


	v3 = result.substr(result.find("b") + 1, result.find("e") - 1);
	pitch = StringToNumber(v3);
	result.erase(0 , result.find("e") + 1);


	result.erase();

	std::cout << "Pin 0: " << analog_r_pin0 << std::endl << "Pitch: " << pitch << std::endl << "Roll : " << roll << std::endl << std::endl;
}


float MyExtension::GetFlex(fingers f)
{
	return *fingers_value[f];
}

float MyExtension::StringToNumber(std::string s)
{
	int Numb;
	std::stringstream str(s);
	str >> Numb;
	return float(Numb);
}

void MyExtension::Reset()
{
}

MyExtension::~MyExtension()
{
	if (connected)
	{
		SerialClose();
		delete Connecting_thread;
	}

	SerialClose();
	delete Connecting_thread;



	// Destroy the Python interpreter.
	Py_Finalize();
}

void MyExtension::SerialClose()
{
	// Get the choosen method from the dictionary.
	pFunc = PyDict_GetItemString(pDict, "Release");

	// Create a Python tuple to hold the arguments to the method.
	pArgs = NULL;

	PyObject* pResult = PyObject_CallObject(pFunc, pArgs);

	// Print a message if calling the method failed.
	if (pResult == NULL)
	{
		std::cout << "Did not shotdown properly\n";
	}

}