/* ---------------------------------------------------------------------------
** This software is in the public domain, furnished "as is", without technical
** support, and with no warranty, express or implied, as to its usefulness for
** any purpose.
**
** V4l2MmapDevice.h
** 
** V4L2 source using mmap API
**
** -------------------------------------------------------------------------*/


#ifndef V4L2_MMAP_DEVICE
#define V4L2_MMAP_DEVICE
 
#include "V4l2Device.h"
#include "../x264/inc/x264_encoder.h"
#include "../jpg/inc/yuv_to_jpg.h"
#include <../sw/redis++/redis++.h>
#include<fstream>
#include<iostream>
#include<queue>
#include <opencv2/opencv.hpp>
#include "Utils.hpp"


// using namespace cv;


using namespace std;
#define V4L2MMAP_NBBUFFER 10
using namespace sw::redis;
using namespace utils_ns;



class V4l2MmapDevice : public V4l2Device
{	
	protected:	
		size_t writeInternal(char* buffer, size_t bufferSize);
		bool   startPartialWrite();
		size_t writePartialInternal(char*, size_t);
		bool   endPartialWrite();
		size_t readInternal(char* buffer, size_t bufferSize);

		
			
	public:
		V4l2MmapDevice(const V4L2DeviceParameters & params, v4l2_buf_type deviceType);		
		virtual ~V4l2MmapDevice();

		virtual bool init(unsigned int mandatoryCapabilities);
		virtual bool isReady() { return  ((m_fd != -1)&& (n_buffers != 0)); }
		virtual bool start();
		virtual bool stop();
	private:
		 x264_encoder *encoder_;
		 Redis * redis_;
		 bool need_record = false;
		//  unsigned long record_start_time=0 ;
		 std::string record_path ;
		 int record_pack_size = 1024*1024*1204;   // M 
		 unsigned long long packed_size;
		 int pre_record_seconds = 15*30+10;
		 
		 FILE *record_file; 
		 ifstream  * record_file_dictt = nullptr;
		 ifstream  * pcd_file_dictt = nullptr;
		 ofstream record_infor;
		 std::queue<raw_ts>  raw_queue; 
		 cv::VideoCapture  cap;
		 cv::Mat frame;
		 bool play_model = false;
		 string find_file_by_id(string id);
		 int frames_video = 0;
		//  std::mutex mtx_replay;    
		 std::mutex mtx_record;
		 string cali_time_n_str = "";
		 bool got_new_cali_time = false;
		 std::chrono::_V2::system_clock::time_point last_pic_time ;
		 std::string device_name;


	
	protected:
		unsigned int  n_buffers;
	
		struct buffer 
		{
			void *                  start;
			size_t                  length;
		};
		buffer m_buffer[V4L2MMAP_NBBUFFER];

		

		
};

#endif

