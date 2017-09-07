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

#include "core/Logger.h"

#define RECV_BUFFER_LEN	512
#define BIND_PORT		5555
#define TARGET_PORT		5001

struct PACKET
{
	unsigned int mark;
	int width;
	int height;
	int fps;
	int bitrate;
	unsigned char ip0;
	unsigned char ip1;
	unsigned char ip2;
	unsigned char ip3;
};

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

	char buf[RECV_BUFFER_LEN];
	sockaddr_in saFrom;
	socklen_t saFromLen = sizeof(saFrom);
	bool isOpen = false;
	while (1)
	{
		GLOG("Waiting for data...");
		int len = recvfrom(s, buf, RECV_BUFFER_LEN, 0, (sockaddr*)&saFrom, &saFromLen);
		GCHECK_RETVAL(len > 0, -1);
		GLOG("recevied packet. from=%s:%d", inet_ntoa(saFrom.sin_addr), ntohs(saFrom.sin_port));
		GCHECK_CONTINUE(len == sizeof(PACKET));

		PACKET* packet = (PACKET*)buf;
		GCHECK_CONTINUE(packet->mark == 0xe1e1c2c2);

		if (packet->width > 0 && packet->height > 0 && packet->fps > 0 && packet->bitrate > 0)
		{
			if (isOpen)
			{
				GLOG("already opened camera.");
			}
			else
			{
				GLOG("open camera.");
				char command[256];
				sprintf(command, "raspivid -o - -t 0 -w %d -h %d -fps %d -b %d -hf -n | nc %d.%d.%d.%d %d &", packet->width, packet->height, packet->fps, packet->bitrate, packet->ip0, packet->ip1, packet->ip2, packet->ip3, TARGET_PORT);
#ifdef __RASPBERRY__
				system(command);
#else
				printf("system: %s\n", command);
#endif

				isOpen = true;
			}
		}
		else
		{
			if (isOpen)
			{
				GLOG("close camera.");
#ifdef __RASPBERRY__
				//system("killall nc");
				system("killall raspivid");
#else
				printf("system: killall raspivid\n");
#endif

				isOpen = false;
			}
			else
			{
				GLOG("already closed camera.");
			}
		}
	}

	return 0;
}

