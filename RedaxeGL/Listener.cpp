#include "Listener.h"
#include <fstream>

Listener::Listener()
{
}


Listener::~Listener()
{
}

void Listener::Listen()
{


	CSerial serial;
	LONG    lLastError = ERROR_SUCCESS;

	std::ofstream File("Readings.dat");

	// Attempt to open the serial port (COM1)
	lLastError = serial.Open(_T("COM3"), 0, 0, false);


	// Setup the serial port (9600,8N1, which is the default setting)
	lLastError = serial.Setup(CSerial::EBaud115200, CSerial::EData8, CSerial::EParNone, CSerial::EStop1);


	// Register only for the receive event
	lLastError = serial.SetMask(CSerial::EEventBreak |
		CSerial::EEventCTS |
		CSerial::EEventDSR |
		CSerial::EEventError |
		CSerial::EEventRing |
		CSerial::EEventRLSD |
		CSerial::EEventRecv);


	// Use 'non-blocking' reads, because we don't know how many bytes
	// will be received. This is normally the most convenient mode
	// (and also the default mode for reading data).
	lLastError = serial.SetupReadTimeouts(CSerial::EReadTimeoutNonblocking);

	// Keep reading data, until an EOF (CTRL-Z) has been received
	bool fContinue = true;


	do{
		// Wait for an event
		lLastError = serial.WaitEvent();

		// Save event
		const CSerial::EEvent eEvent = serial.GetEventType();

		// Handle break event
		if (eEvent & CSerial::EEventBreak)
		{
			printf("\n### BREAK received ###\n");
		}

		// Handle CTS event
		if (eEvent & CSerial::EEventCTS)
		{
			printf("\n### Clear to send %s ###\n", serial.GetCTS() ? "on" : "off");
		}

		// Handle DSR event
		if (eEvent & CSerial::EEventDSR)
		{
			printf("\n### Data set ready %s ###\n", serial.GetDSR() ? "on" : "off");
		}

		// Handle error event
		if (eEvent & CSerial::EEventError)
		{
			printf("\n### ERROR: ");
			switch (serial.GetError())
			{
			case CSerial::EErrorBreak:		printf("Break condition");			break;
			case CSerial::EErrorFrame:		printf("Framing error");			break;
			case CSerial::EErrorIOE:		printf("IO device error");			break;
			case CSerial::EErrorMode:		printf("Unsupported mode");			break;
			case CSerial::EErrorOverrun:	printf("Buffer overrun");			break;
			case CSerial::EErrorRxOver:		printf("Input buffer overflow");	break;
			case CSerial::EErrorParity:		printf("Input parity error");		break;
			case CSerial::EErrorTxFull:		printf("Output buffer full");		break;
			default:						printf("Unknown");					break;
			}
			printf(" ###\n");
		}

		// Handle ring event
		if (eEvent & CSerial::EEventRing)
		{
			printf("\n### RING ###\n");
		}

		// Handle RLSD/CD event
		if (eEvent & CSerial::EEventRLSD)
		{
			printf("\n### RLSD/CD %s ###\n", serial.GetRLSD() ? "on" : "off");
		}

		// Handle data receive event
		if (eEvent & CSerial::EEventRecv)
		{
			// Read data, until there is nothing left
			DWORD dwBytesRead = 0;
			char szBuffer[101];

			do
			{
				// Read data from the COM-port
				lLastError = serial.Read(szBuffer, sizeof(szBuffer) - 1, &dwBytesRead);

				if (dwBytesRead > 0)
				{
					// Finalize the data, so it is a valid string
					szBuffer[dwBytesRead] = '\0';

					// Display the data
					printf("%s", szBuffer);


					//WRITE TO FILE 
					File << szBuffer;

					// Check if EOF (CTRL+'[') has been specified
					if (strchr(szBuffer, EOF_Char))
						fContinue = false;
				}
			} while (dwBytesRead == sizeof(szBuffer) - 1);
		}


		// CLOSEFILE & REOPEN
		File.close();
		File.open("Readings.dat");
		File.clear();

	} while (fContinue);

	// Close the port again
	serial.Close();
}
