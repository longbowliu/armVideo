单独编译的时候要在 decode_video.c 中添加：
extern "C"
{
#include <libavcodec/avcodec.h>
#include <libavutil/opt.h>
#include <libavutil/imgutils.h>
}

g++ decode_video.c -o test -I /usr/local/include -I /usr/local/ffmpeg/include -L /usr/local/ffmpeg/lib/ -lavformat -lavcodec -lavutil  -lpthread -lm -ldl

run : ./test  /home/demo/INNO/repos/FFmpeg-release-4.2/doc/examples/test_original.264 /home/demo/INNO/repos/FFmpeg-release-4.2/doc/examples/test_


g++ seek_test.cpp -o test -I /usr/local/include -I /usr/local/ffmpeg/include -L /usr/local/ffmpeg/lib/ -lavformat -lavcodec -lavutil  -lpthread -lm -ldl -lboost_program_options

