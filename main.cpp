/*
 * main.cpp
 *
 *  Created on: 2017. 9. 7.
 *      Author: mirime
 */

#include <stdio.h>
#include <stdlib.h>
#include<arpa/inet.h>
#include<sys/socket.h>

#include "define.h"
#include "core/Logger.h"

void sendR_C_CAMERA_STATUS(int s, const sockaddr_in& to, CAMERA_STATUS::ETYPE cameraStatus)
{
	char buffer[MAX_BUFFER_LEN];
	HEADER* header = (HEADER*)buffer;
	header->command = COMMAND::R_C_CAMERA_STATUS;
	header->dataSize = sizeof(DATA_R_C_CAMERA_STATUS);
	DATA_R_C_CAMERA_STATUS* data = (DATA_R_C_CAMERA_STATUS*)header->getData();
	data->cameraStatus = cameraStatus;

	sendto(s, buffer, header->getTotalSize(), 0, (const sockaddr*)&to, sizeof(to));
}

int main(int argc, char* argv[])
{
	Logger::setDev(argc > 1 ? atoi(argv[1]) == 1 : false);

	int s = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	GCHECK_RETVAL(s != -1, -1);

	sockaddr_in sa;
	memset(&sa, 0, sizeof(sa));
	sa.sin_family = AF_INET;
	sa.sin_port = htons(BIND_PORT);
	sa.sin_addr.s_addr = htonl(INADDR_ANY);
	GCHECK_RETVAL(bind(s, (const sockaddr*)&sa, sizeof(sa)) != -1, -1);

	char buf[MAX_BUFFER_LEN];
	sockaddr_in saFrom;
	socklen_t saFromLen = sizeof(saFrom);
	CAMERA_STATUS::ETYPE cameraStatus = CAMERA_STATUS::CLOSED;
	while (1)
	{
		GLOG("Waiting for data...");
		int len = recvfrom(s, buf, MAX_BUFFER_LEN, 0, (sockaddr*)&saFrom, &saFromLen);
		GCHECK_RETVAL(len > 0, -1);
		GLOG("received buffer. from=%s:%d len=%d", inet_ntoa(saFrom.sin_addr), ntohs(saFrom.sin_port), len);
		GCHECK_CONTINUE(len >= sizeof(HEADER));

		HEADER* header = (HEADER*)buf;
		GLOG("received packet. command=%d datasize=%d", header->command, header->dataSize);

		switch (header->command)
		{
		case COMMAND::C_R_CAMERA_STATUS:
			{
				sendR_C_CAMERA_STATUS(s, saFrom, cameraStatus);
			}
			break;
		case COMMAND::C_R_OPEN_CAMERA:
			{
				if (cameraStatus == CAMERA_STATUS::OPENED)
				{
					GLOG("already opened camera.");
				}
				else
				{
					DATA_C_R_OPEN_CAMERA* data = (DATA_C_R_OPEN_CAMERA*)header->getData();

					char command[256];
					sprintf(command, "raspivid -o - -t 0 -w %d -h %d -fps %d -b %d -hf -n | nc %d.%d.%d.%d %d &", data->width, data->height, data->fps, data->bitrate, data->ip0, data->ip1, data->ip2, data->ip3, TARGET_PORT);
#ifdef __RASPBERRY__
					system(command);
#endif
					GLOG("open camera. system=%s", command);
					cameraStatus = CAMERA_STATUS::OPENED;
				}

				sendR_C_CAMERA_STATUS(s, saFrom, cameraStatus);
			}
			break;
		case COMMAND::C_R_CLOSE_CAMERA:
			{
				if (cameraStatus == CAMERA_STATUS::CLOSED)
				{
					GLOG("already closed camera.");
				}
				else
				{
#ifdef __RASPBERRY__
					//system("killall nc");
					system("killall raspivid");
#endif
					GLOG("close camera. system=killall raspivid");
					cameraStatus = CAMERA_STATUS::CLOSED;
				}

				sendR_C_CAMERA_STATUS(s, saFrom, cameraStatus);
			}
			break;
		}
	}

	return 0;
}

