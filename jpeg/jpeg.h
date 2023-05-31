#include <fcntl.h>
#include <getopt.h>
#include <stdio.h>  // wtf
#include <jpeglib.h>
#include <linux/videodev2.h>
#include <malloc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

#include <algorithm>
#include <chrono>
#include <iostream>
#include <asm/types.h>
#include <assert.h>
#include <errno.h>

#define OUTPUT_BUF_SIZE 4096

#define CLEAR(x) memset(&(x), 0, sizeof(x))

#define WIDTH 3840 //1920

#define HEIGHT 2160 //1080

struct buffer {
  void *start;

  size_t length;
};

typedef struct {
  struct jpeg_destination_mgr pub;

  JOCTET *buffer;

  unsigned char *outbuffer;

  int outbuffer_size;

  unsigned char *outbuffer_cursor;

  int *written;

} mjpg_destination_mgr;

class jpeg_encoder{

  public : 
    int compress_yuyv_to_jpeg(unsigned char *buf, unsigned char *buffer, int size,
                          int quality) ;
    

};