/*
 * define.h
 *
 *  Created on: 2017. 9. 11.
 *      Author: mirime
 */

#ifndef DEFINE_H_
#define DEFINE_H_

#define MAX_BUFFER_LEN	512
#define BIND_PORT		5555
#define TARGET_PORT		5001

namespace CAMERA_STATUS
{
	enum ETYPE
	{
		NONE = 0,
		OPENED,
		CLOSED,
	};
}


namespace COMMAND
{
	enum ETYPE
	{
		C_R_CAMERA_STATUS = 1,
		C_R_OPEN_CAMERA,
		C_R_CLOSE_CAMERA,

		R_C_CAMERA_STATUS,
	};
}

struct HEADER
{
	int command;
	int dataSize;

	inline int					getTotalSize() const { return sizeof(*this) + dataSize; }
	inline unsigned char*		getData() { return reinterpret_cast<unsigned char*>(this) + sizeof(*this); }
	inline const unsigned char*	getData() const { return reinterpret_cast<const unsigned char*>(this) + sizeof(*this); }
};

struct DATA_C_R_OPEN_CAMERA
{
	int width;
	int height;
	int fps;
	int bitrate;
	unsigned char ip0;
	unsigned char ip1;
	unsigned char ip2;
	unsigned char ip3;
};

struct DATA_R_C_CAMERA_STATUS
{
	int cameraStatus
};


#endif /* DEFINE_H_ */
