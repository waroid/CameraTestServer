/*
 * Logger.h
 *
 *  Created on: Mar 28, 2015
 *      Author: mirime
 */

#ifndef LOGGER_H_
#define LOGGER_H_

#include <string.h>
#include <errno.h>

class Logger
{
public:
	static bool isDev()
	{
		return s_dev;
	}
	static void setDev(bool dev)
	{
		s_dev = dev;
	}

	static void write(const char* format, ...);

private:
	static bool s_dev;
};

#define GACTION_NONE
#define GACTION_RETURN		return
#define GACTION_RETVAL(x)	return (x)
#define GACTION_CONTINUE	continue

#define GCHECK_DO(exp,action)	{if(!(exp)){Logger::write("<%s,%s,%s(%d),%s(%d)>" "\n", #exp,__FUNCTION__,__FILE__,__LINE__,strerror(errno),errno);action;}}
#define GCHECK_RETURN(exp)		GCHECK_DO(exp,GACTION_RETURN)
#define GCHECK_RETVAL(exp,val)	GCHECK_DO(exp,GACTION_RETVAL(val))
#define GCHECK_RETFALSE(exp)	GCHECK_DO(exp,GACTION_RETVAL(false))
#define GCHECK_RETNULL(exp)		GCHECK_DO(exp,GACTION_RETVAL(NULL))
#define GCHECK_RETZERO(exp)		GCHECK_DO(exp,GACTION_RETVAL(0))
#define GCHECK_CONTINUE(exp)	GCHECK_DO(exp,GACTION_CONTINUE)

#define GCHECKV_DO(exp,action,fmt,...)	{if(!(exp)){Logger::write("<%s,%s,%s(%d),%s(%d)>" fmt "\n", #exp,__FUNCTION__,__FILE__,__LINE__,strerror(errno),errno,__VA_ARGS__);action;}}
#define GCHECKV_RETURN(exp,fmt,...)		GCHECKV_DO(exp,GACTION_RETURN,fmt,__VA_ARGS__)
#define GCHECKV_RETVAL(exp,val,fmt,...)	GCHECKV_DO(exp,GACTION_RETVAL(val),fmt,__VA_ARGS__)
#define GCHECKV_RETFALSE(exp,fmt,...)	GCHECKV_DO(exp,GACTION_RETVAL(false),fmt,__VA_ARGS__)
#define GCHECKV_RETNULL(exp,fmt,...)	GCHECKV_DO(exp,GACTION_RETVAL(NULL),fmt,__VA_ARGS__)
#define GCHECKV_RETZERO(exp,fmt,...)	GCHECKV_DO(exp,GACTION_RETVAL(0),fmt,__VA_ARGS__)
#define GCHECKV_CONTINUE(exp,fmt,...)	GCHECKV_DO(exp,GACTION_CONTINUE,fmt,__VA_ARGS__)

#define GLOG(fmt,...)	Logger::write(fmt "\n", ##__VA_ARGS__)

#endif /* LOGGER_H_ */
