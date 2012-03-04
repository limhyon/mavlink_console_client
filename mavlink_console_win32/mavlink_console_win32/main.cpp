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

	dcbSerialParams.BaudRate = CBR_115200;
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

	/// some flag
	bool isLiveChecked = false;

	/// System information
	mavlink_system_t mavlink_system;
	mavlink_system.sysid = 255;                   ///< ID 255
	mavlink_system.compid = 0;     ///< The component sending the message is the IMU, it could be also a Linux process
	
	/// Sending buffer
	mavlink_message_t sendMsg;
	uint8_t sendBuf[MAVLINK_MAX_PACKET_LEN];
	uint16_t len = 0;

	while(run == true)
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
			printf("Received message with ID %d, sequence: %d from component %d of system %d\n", message.msgid, message.seq, message.compid, message.sysid);

			switch(message.msgid)
			{
				case MAVLINK_MSG_ID_HEARTBEAT:
					if(isLiveChecked == false)
					{
						// Request data stream
						mavlink_msg_request_data_stream_pack(mavlink_system.sysid, mavlink_system.compid,&sendMsg,message.sysid,message.compid,MAV_DATA_STREAM_RAW_SENSORS,10,1);

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
					printf("Heartbeat\n");
					break;

				case MAVLINK_MSG_ID_OPTICAL_FLOW:
					printf("Optical flow!\n");
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

	return 0;
}
