#pragma once
#define STRICT
#include <tchar.h>
#include <windows.h>
#include <stdio.h>
#include <string.h>
#include "Serial.h"

class Listener
{
public:

	Listener();
	~Listener();

	void Listen();

	enum { EOF_Char = 27 };

	static int ShowError(LONG lError, LPCTSTR lptszMessage)
	{
		// Generate a message text
		TCHAR tszMessage[256];
		wsprintf(tszMessage, _T("%s\n(error code %d)"), lptszMessage, lError);

		// Display message-box and return with an error-code
		::MessageBox(0, tszMessage, _T("Listener"), MB_ICONSTOP | MB_OK);
		return 1;
	}
};

