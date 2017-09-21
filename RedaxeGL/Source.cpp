#include "Engine.h"
#include <iostream>



int main()
{


	//==================================================== Create Demo
	Engine* Demo = new Engine("Settings\\Settings.ini");
	//==================================================== Start Demo
	Demo->Inititialize();
	//==================================================== Loop Demo
	Demo->MainLoop();
	//==================================================== End Demo
	Demo->Terminate();
	//==================================================== Destroy Demo
	delete Demo;

	system("pause");

	return 0;
}