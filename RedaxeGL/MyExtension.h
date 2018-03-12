#pragma once
#ifdef _DEBUG
#undef _DEBUG
#include <Python.h>
#define _DEBUG
	#undef _DEBUG
	#include <Python.h>
	#define _DEBUG
#else
#include <Python.h>
	#include <Python.h>
#endif

#include <iostream>
#include <string>
#include <deque>
#include <thread>
#include <future>


#define MAX_FINGERS 5


class MyExtension
{

private: 
	
	std::string _path;

	float StringToNumber(std::string s);

	float yaw, pitch, roll, analog_r_pin0;

	// Create some Python objects that will later be assigned values.
	PyObject *pName, *pModule, *pDict, *pFunc, *pArgs, *pValue;

	float scale;

	bool connected;
	long baud;

	std::deque <std::thread *> Connecting_thread;


	void SerialClose();

	void Connect();


public:
	MyExtension(const std::string& path);

	void SetBaud(long _baud);


	void Reset();
	
/**********************************************************************/

	inline bool isConnected() { return connected; };

	inline float GetRoll() { return roll / scale; };

	inline float GetPitch() { return pitch / scale; };

	inline float GetYaw() { return yaw / scale; };

	inline void SetScale(float s) {scale = s;	};

	inline float GetScale() { return scale; };

/**********************************************************************/

	~MyExtension();

	
	void IntialiseVarTuple();

	float* fingers_value[MAX_FINGERS];

	enum errors { INITIALISATION_ERROR };
	enum fingers { F_THUMB, F_INDEX, F_MIDDLE, F_RING, F_PINKY };


	float GetFlex(fingers f);
};

