/*
 * Logger.cpp
 *
 *  Created on: Mar 28, 2015
 *      Author: mirime
 */

#include <stdio.h>
#include <stdarg.h>
#include <syslog.h>

#include "Logger.h"

namespace LOGGER
{
	const int MAX_BUFFER_SIZE = 1024;
}
using namespace LOGGER;

bool Logger::s_dev = false;

void Logger::write(const char* format, ...)
{
	char temp[MAX_BUFFER_SIZE] = {0};

	va_list vl;
	va_start(vl, format);
	int len = vsnprintf(temp, MAX_BUFFER_SIZE - 2, format, vl);
	va_end(vl);

	if (len > 0)
	{
		temp[len] = 0;
		syslog(LOG_USER, temp);
		if (s_dev)
		{
			printf("%s", temp);
		}
	}
}
