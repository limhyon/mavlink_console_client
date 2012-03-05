#include <stdio.h>
#include <windows.h>
#include <conio.h>
#include <time.h>

/// MAVLink
#include <common/mavlink.h>

/// VICON
#include "vicon.h"

HANDLE hSerial;
DCB dcbSerialParams = {0};
COMMTIMEOUTS timeouts = {0};
bool run = true;
bool isLiveChecked = false;

class FlightData
{
public:
	float roll;
	float pitch;
	float yaw;
};

/// Telemetry
mavlink_optical_flow_t optical_flow;
mavlink_state_correction_t state_error;
FILE *fp;

VOID CALLBACK TimerProc(HWND hwnd,UINT uMsg,UINT_PTR idEvent,DWORD dwTime)
{
	/// Do your job.
	static DWORD prevTime = 0;
	//printf("dwTime : %d\n",dwTime-prevTime);

	if(isLiveChecked == true)
	{
		printf("[REC] errRoll=%2.2f,errPitch=%2.2f,Vx=%2.2f,Vy=%2.2f,Alt=%2.2f,dt=%f\n",state_error.rollErr,state_error.pitchErr,state_error.vxErr,state_error.vyErr,optical_flow.ground_distance,optical_flow.time/1000000.0);
		fprintf(fp,"%f\t%f\t%f\t%f\t%f\t%f\n",state_error.rollErr,state_error.pitchErr,state_error.vxErr,state_error.vyErr,optical_flow.ground_distance,optical_flow.time/1000000.0);
	}
	else
	{
		printf("errRoll=%2.2f,errPitch=%2.2f,Vx=%2.2f,Vy=%2.2f,Alt=%2.2f,dt=%f\n",state_error.rollErr,state_error.pitchErr,state_error.vxErr,state_error.vyErr,optical_flow.ground_distance,optical_flow.time/1000000.0);
	}

	prevTime = dwTime;
}

DWORD WINAPI ThreadProc(LPVOID lpParameter)
{
	UINT uiTimer = ::SetTimer(NULL, 12354, 80, TimerProc);

	MSG msg;

	while (GetMessage(&msg, NULL, 0, 0))
	{
		/// Message pumping.
		DispatchMessage(&msg);
	}

	KillTimer(NULL,uiTimer);

	return 0;
}

int main(int argc, char** argv)
{
	int portNum = -1;
	char portArgBuf[6];
	char fName[0xff];

	/// Connect VICON
	initialize_VICON();


	srand ( time(NULL) );

	sprintf_s(fName,"data-%d.txt",rand());

	fp = fopen(fName,"w");

	if(fp == NULL)
	{
		printf("Fopen failed!\n");
		return -1;
	}

	/// Timer Thread function.
	HANDLE hThread = ::CreateThread(NULL, NULL, ThreadProc, NULL, NULL, NULL);
	CloseHandle(hThread);

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

	//dcbSerialParams.BaudRate = CBR_115200;
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

	uint8_t szBuff = 0;
	DWORD dwBytesRead = 0;
	DWORD dwBytesWritten = 0;

	/// Mavlink
	mavlink_status_t lastStatus;
	lastStatus.packet_rx_drop_count = 0;

	/// System information
	mavlink_system_t mavlink_system;
	mavlink_system.sysid = 255;                   ///< ID 255
	mavlink_system.compid = 0;     ///< The component sending the message is the IMU, it could be also a Linux process
	
	/// Sending buffer
	mavlink_message_t sendMsg;
	uint8_t sendBuf[MAVLINK_MAX_PACKET_LEN];
	uint16_t len = 0;

	printf("Open success.\n");

	while(!_kbhit())
	{
		mavlink_message_t message;
		mavlink_status_t status;
		uint8_t msgReceived = false;

		if(!ReadFile(hSerial,&szBuff,1,&dwBytesRead,NULL))
		{
			printf("Read error %d\n",GetLastError());
			dwBytesRead = 0;
		}
		else
		{
			msgReceived = mavlink_parse_char(MAVLINK_COMM_0,szBuff,&message,&status);
			//printf("%d ",szBuff);
			//printf("%d bytes read\n",dwBytesRead);

			//if(mavlink_parse_char(0,szBuff,&message,&status))
			//{
				//printf("Received message with ID %d, sequence: %d from component %d of system %d\n", message.msgid, message.seq, message.compid, message.sysid);
			//}
			
			if (lastStatus.packet_rx_drop_count != status.packet_rx_drop_count)			
			{
				printf("ERROR: DROPPED %d PACKETS\n", status.packet_rx_drop_count);
			}

			lastStatus = status;
		}

		if(msgReceived)		
		{
			//printf("Received message with ID %d, sequence: %d from component %d of system %d\n", message.msgid, message.seq, message.compid, message.sysid);

			switch(message.msgid)
			{
				case MAVLINK_MSG_ID_HEARTBEAT:
					if(isLiveChecked == false)
					{
						// Request data stream
						//mavlink_msg_request_data_stream_pack(mavlink_system.sysid, mavlink_system.compid,&sendMsg,message.sysid,message.compid,MAV_DATA_STREAM_RAW_SENSORS,10,1);
						mavlink_msg_request_data_stream_pack(mavlink_system.sysid, mavlink_system.compid,&sendMsg,message.sysid,message.compid,MAV_DATA_STREAM_EXTRA3,50,1);

						len = mavlink_msg_to_send_buffer(sendBuf,&sendMsg);

						if(!WriteFile(hSerial,sendBuf,len,&dwBytesWritten,NULL))
						{
							printf("Send error : %d\n",GetLastError());
						}
						else
						{
							printf("Ask success %d/%d\n",len,dwBytesWritten);
							//isLiveChecked = true;
						}
					}
					break;

				case MAVLINK_MSG_ID_OPTICAL_FLOW:
					isLiveChecked = true;
					mavlink_msg_optical_flow_decode(&message,&optical_flow);
					break;

				case MAVLINK_MSG_ID_STATE_CORRECTION:
					isLiveChecked = true;
					mavlink_msg_state_correction_decode(&message,&state_error);
					break;

				case MAVLINK_MSG_ID_SYS_STATUS:
					printf("[STATUS] mode:%d\n",mavlink_msg_sys_status_get_status(&message));
					break;

				default:
					printf("Unknown message %d\n",message.msgid);
					break;
			}

			msgReceived = false;
		}
	}

	CloseHandle(hSerial);
	fclose(fp);

	printf("Terminated");

	return 0;
}
