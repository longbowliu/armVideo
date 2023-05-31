/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** V4l2ReadWriteDevice.h
** 
** V4L2 source using read/write API
**
** -------------------------------------------------------------------------*/


#ifndef V4L2_RW_DEVICE
#define V4L2_RW_DEVICE
 
#include "V4l2Device.h"
// #include "../x264/inc/x264_encoder.h"
// #include "../jpg/inc/yuv_to_jpg.h"
#include <../sw/redis++/redis++.h>
#include<fstream>
#include<iostream>
// #include<queue>
#include <opencv2/opencv.hpp>
#include "Utils.hpp"

using namespace sw::redis;
using namespace std;
using namespace utils_ns;

class V4l2ReadWriteDevice : public V4l2Device
{	
	private :
		Redis * redis_;
		std::mutex mtx_file_;   
		condition_variable cond_var_;

	protected:	
		virtual size_t writeInternal(char* buffer, size_t bufferSize);
		virtual size_t readInternal(char* buffer, size_t bufferSize);
		virtual bool init(unsigned int mandatoryCapabilities);
		cv::VideoCapture  cap;
		std::string record_file_name;
		cv::Mat frame;
		std::string record_path ;
		ifstream  * record_file_dictt = nullptr;
		ifstream  * pcd_file_dictt = nullptr;
		std::mutex mtx_replay;    
		int frames_video = 0;
		string cali_time_n_str = "";
		bool got_new_cali_time = false;
		bool readable = false;
		std::chrono::_V2::system_clock::time_point last_pic_time ;
		
	public:
		V4l2ReadWriteDevice(const V4L2DeviceParameters&  params, v4l2_buf_type deviceType);
};


#endif

