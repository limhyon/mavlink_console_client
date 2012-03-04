#include <stdio.h>
#include <windows.h>

/// MAVLink
#include <common/mavlink.h>

HANDLE hSerial;
DCB dcbSerialParams = {0};
COMMTIMEOUTS timeouts = {0};
bool run = true;

int main(int argc, char** argv)
{
	int portNum = -1;
	char portArgBuf[6];

	if(argc < 2)
	{
		printf("Specify port (e.g., COM1)\n");
		return -1;
	}

	sscanf_s(argv[1],"COM%d",&portNum);

	if(portNum < 0)
	{
		printf("Specify valid port %d\n",portNum);
		return -1;
	}
	else
	{
		printf("Port COM%d opening... \n",portNum);
		sprintf_s(portArgBuf,"COM%d",portNum);
	}

	hSerial = CreateFile(portArgBuf,
							GENERIC_READ | GENERIC_WRITE,
							0,
							NULL,
							OPEN_EXISTING,
							FILE_ATTRIBUTE_NORMAL,
							NULL);

	if(hSerial == INVALID_HANDLE_VALUE)
	{
		if(GetLastError() == ERROR_FILE_NOT_FOUND)
		{
			printf("Invalid port.\n");
			return -1;
		}
		else
		{
			printf("ErrorNo %d\n",GetLastError());
			return -1;
		}
	}

	dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

	if(!GetCommState(hSerial,&dcbSerialParams))
	{
		printf("Error getting state\n");
		return -1;
	}

	dcbSerialParams.BaudRate = CBR_57600;
	dcbSerialParams.ByteSize = 8;
	dcbSerialParams.StopBits = ONESTOPBIT;
	dcbSerialParams.Parity = NOPARITY;

	if(!SetCommState(hSerial,&dcbSerialParams))
	{
		printf("Error setting serial port state\n");
		return -1;
	}

	timeouts.ReadIntervalTimeout = 50000;
	timeouts.ReadTotalTimeoutConstant = 50000;
	timeouts.ReadTotalTimeoutMultiplier = 10000;
	timeouts.WriteTotalTimeoutConstant = 50000;
	timeouts.WriteTotalTimeoutMultiplier = 10000;

	if(!SetCommTimeouts(hSerial,&timeouts))
	{
		printf("Error setting timeout\n");
		return -1;
	}

	unsigned char szBuff = 0;
	DWORD dwBytesRead = 0;

	while(run == true)
	{
		if(!ReadFile(hSerial,&szBuff,sizeof(szBuff),&dwBytesRead,NULL))
		{
			printf("Read error %d\n",GetLastError());
			dwBytesRead = 0;
		}
		else
		{
			printf("%02X ", szBuff);
		}
	}

	CloseHandle(hSerial);

	return 0;
}
