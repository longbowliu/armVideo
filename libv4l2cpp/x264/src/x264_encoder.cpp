#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <pthread.h>
#include <stdlib.h>
#include <sys/select.h>
#include <sys/types.h>
#include <unistd.h>
#include <signal.h>
#include <linux/videodev2.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <poll.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <string.h>
#include <stdint.h>
#include "../inc/x264_encoder.h"
 

 

 
/*设置视频录制相关参数*/
 int x264_encoder::x264_param_apply_preset(x264_param_t *param, const char *preset)
{
    char *end;
    int i = strtol( preset, &end, 10 );
    if( *end == 0 && i >= 0 && i < sizeof(x264_preset_names)/sizeof(*x264_preset_names)-1 )
        preset = x264_preset_names[i];
	/*快4*/
    if( !strcasecmp( preset, "ultrafast" ) )
    {
		param->i_sync_lookahead=0;
		param->b_sliced_threads =1;
		param->b_vfr_input = 0;
		// param->b_annexb = 1;

        param->i_frame_reference = 1;
        param->i_scenecut_threshold = 0;
        param->b_deblocking_filter = 0;
        param->b_cabac = 0;
        param->i_bframe = 0;
        param->analyse.intra = 0;
        param->analyse.inter = 0;
        param->analyse.b_transform_8x8 = 0;
        param->analyse.i_me_method = X264_ME_DIA;
        param->analyse.i_subpel_refine = 0;
        param->rc.i_aq_mode = 0;
        param->analyse.b_mixed_references = 0;
        param->analyse.i_trellis = 0;
        param->i_bframe_adaptive = X264_B_ADAPT_NONE;
        param->rc.b_mb_tree = 0;
        param->analyse.i_weighted_pred = X264_WEIGHTP_NONE;
        param->analyse.b_weighted_bipred = 0;
        param->rc.i_lookahead = 0;
    }
	/*快3*/
    else if( !strcasecmp( preset, "superfast" ) )
    {
        param->analyse.inter = X264_ANALYSE_I8x8|X264_ANALYSE_I4x4;
        param->analyse.i_me_method = X264_ME_DIA;
        param->analyse.i_subpel_refine = 1;
        param->i_frame_reference = 1;
        param->analyse.b_mixed_references = 0;
        param->analyse.i_trellis = 0;
        param->rc.b_mb_tree = 0;
        param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
        param->rc.i_lookahead = 0;
    }
	/*快2*/
    else if( !strcasecmp( preset, "veryfast" ) )
    {
        param->analyse.i_me_method = X264_ME_HEX;
        param->analyse.i_subpel_refine = 2;
        param->i_frame_reference = 1;
        param->analyse.b_mixed_references = 0;
        param->analyse.i_trellis = 0;
        param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
        param->rc.i_lookahead = 10;
    }
	/*快1*/
    else if( !strcasecmp( preset, "faster" ) )
    {
        param->analyse.b_mixed_references = 0;
        param->i_frame_reference = 2;
        param->analyse.i_subpel_refine = 4;
        param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
        param->rc.i_lookahead = 20;
    }
	/*快速*/
    else if( !strcasecmp( preset, "fast" ) )
    {
        param->i_frame_reference = 2;
        param->analyse.i_subpel_refine = 6;
        param->analyse.i_weighted_pred = X264_WEIGHTP_SIMPLE;
        param->rc.i_lookahead = 30;
    }
	/*中等*/
    else if( !strcasecmp( preset, "medium" ) )
    {
        /* Default is medium */
    }
	/*慢速*/
    else if( !strcasecmp( preset, "slow" ) )
    {
        param->analyse.i_me_method = X264_ME_UMH;
        param->analyse.i_subpel_refine = 8;
        param->i_frame_reference = 5;
        param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
        param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_AUTO;
        param->rc.i_lookahead = 50;
    }
    else if( !strcasecmp( preset, "slower" ) )
    {
        param->analyse.i_me_method = X264_ME_UMH;
        param->analyse.i_subpel_refine = 9;
        param->i_frame_reference = 8;
        param->i_bframe_adaptive = X264_B_ADAPT_TRELLIS;
        param->analyse.i_direct_mv_pred = X264_DIRECT_PRED_AUTO;
        param->analyse.inter |= X264_ANALYSE_PSUB8x8;
        param->analyse.i_trellis = 2;
        param->rc.i_lookahead = 60;
    }
    else if( !strcasecmp( preset, "veryslow" ) )
    {
        param->analyse.i_me_method = X264_ME_UMH;
        param->analyse.i_subpel_refine = 10;
	}
    else
    {
        return -1;
    }
 
    return 0;
}
 
 
/*开始视频压缩*/
x264_encoder::x264_encoder( int width, int height) 
{
	Encoder *en = &my_encoder;
	en->param = (x264_param_t *) malloc(sizeof(x264_param_t));
	en->picture = (x264_picture_t *) malloc(sizeof(x264_picture_t));
 
	x264_param_default(en->param); //编码器默认设置
 
	/*订制编码器压缩的性能*/
	x264_param_apply_preset(en->param,"faster");
	en->param->i_width = width; //设置图像宽度
	en->param->i_height = height; //设置图像高度
	en->param->i_fps_num = 30;
	en->param->i_fps_den = 1;
	// Intra refres:
	en->param->i_keyint_max = 30;
	en->param->b_intra_refresh = 1;
	//Rate control:
	en->param->rc.i_rc_method = X264_RC_CRF;
	en->param->rc.f_rf_constant = 25;
	en->param->rc.f_rf_constant_max = 30;
	en->param->i_sps_id = 7;
	//For streaming:
	en->param->b_repeat_headers = 1;
	// en->param->b_annexb = 1;
	if((en->handle = x264_encoder_open(en->param)) == 0)
	{
		return;
	}
 
	x264_picture_alloc(en->picture, X264_CSP_I420, en->param->i_width,en->param->i_height);
	en->picture->img.i_csp = X264_CSP_I420;
	en->picture->img.i_plane = 3;

	encoded_frame=(uint8_t *)malloc(sizeof(uint8_t)*width*height*3);
	if(encoded_frame==NULL)printf("X264缓冲区申请失败!\n");
	// std::chrono::system_clock::time_point now = std::chrono::system_clock::now();
	// int64_t timepoint = std::chrono::duration_cast<std::chrono::milliseconds>(now.time_since_epoch()).count();
	// std::string fn = "/home/demo/";
	// fn += std::to_string(timepoint);
	// file = fopen(fn.c_str(),"wa+");
	// fifo =  "/tmp/fifo";
	// mkfifo(fifo, 0777);
	// h264_fp = fopen(fifo, "wa+");
}
 
/*结束压缩*/
void x264_encoder::compress_end(Encoder *en)
{
	if (en->picture)
	{
		x264_picture_clean(en->picture);
		free(en->picture);
		en->picture = 0;
	}
	
	if(en->param)
	{
		free(en->param);
		en->param = 0;
	}
	if(en->handle)
	{
		x264_encoder_close(en->handle);
	}
	// free(en);
}
 
 

 
 
/*压缩一帧数据*/
int x264_encoder::compress_frame(Encoder *en, int type, uint8_t *in, uint8_t *out) {
	x264_picture_t pic_out;
	int nNal = 0;
	int result = 0;
	int i = 0 , j = 0 ;
	uint8_t *p_out = out;
	en->nal=NULL;
	uint8_t *p422;
 
	char *y = (char*)en->picture->img.plane[0];
	char *u = (char*)en->picture->img.plane[1];
	char *v = (char*)en->picture->img.plane[2];
 
 
//
	int widthStep422 = en->param->i_width * 2;
	for(i = 0; i < en->param->i_height; i += 2)
	{
		p422 = in + i * widthStep422;
		for(j = 0; j < widthStep422; j+=4)
		{
			*(y++) = p422[j];
			*(u++) = p422[j+1];
			*(y++) = p422[j+2];
		}
		p422 += widthStep422;
		for(j = 0; j < widthStep422; j+=4)
		{
			*(y++) = p422[j];
			*(v++) = p422[j+3];
			*(y++) = p422[j+2];
		}
	}
 
	switch (type) {
	case 0:
		en->picture->i_type = X264_TYPE_P;
		break;
	case 1:
		en->picture->i_type = X264_TYPE_IDR;
		break;
	case 2:
		en->picture->i_type = X264_TYPE_I;
		break;
	default:
		en->picture->i_type = X264_TYPE_AUTO;
		break;
	}
 
	/*开始264编码*/
	if (x264_encoder_encode(en->handle, &(en->nal), &nNal, en->picture,
			&pic_out) < 0) {
		return -1;
	}
	en->picture->i_pts++;
 
	
	for (i = 0; i < nNal; i++) {
		memcpy(p_out, en->nal[i].p_payload, en->nal[i].i_payload);
		p_out += en->nal[i].i_payload;
		result += en->nal[i].i_payload;
	}
 
	return result;
	//return nNal;
}
 
//编码并写入一帧数据
int x264_encoder::encode_frame(uint8_t *yuv_frame)
{
	int h264_length = 0;
	//压缩一帧数据
	h264_length = compress_frame(&my_encoder, -1, yuv_frame, encoded_frame);
	if(h264_length > 0)
	{
		// printf("h264_length=%d\n",h264_length);
		//写入视频文件
		// if(file_test){
		// 	fwrite(encoded_frame, h264_length,1,file);
		// }
	}
	return h264_length;
}

x264_encoder::~x264_encoder(){
	// fclose(file);
	compress_end(&my_encoder);
	if(encoded_frame){
		free(encoded_frame);
	}
	// fclose(h264_fp);
}
 
 

