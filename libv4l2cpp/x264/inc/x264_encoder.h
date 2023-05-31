#include "x264.h"
#include <stdio.h>


typedef struct
{
	x264_param_t *param;
	x264_t *handle;
	x264_picture_t *picture; //说明一个视频序列中每帧特点
	x264_nal_t *nal;
}Encoder;




class x264_encoder{
    public:
        x264_encoder(int width, int height);
        int x264_param_apply_preset(x264_param_t *param, const char *preset);
        void compress_begin(Encoder *en, int width, int height);
        void compress_end(Encoder *en);
        int compress_frame(Encoder *en, int type, uint8_t *in, uint8_t *out);
        int encode_frame(uint8_t *yuv_frame);
         Encoder my_encoder;
      //  FILE  * file ;
      //  char const* fifo;
      //  FILE *h264_fp;

        ~x264_encoder();

        unsigned char *encoded_frame=NULL;
        bool file_test =true;
};