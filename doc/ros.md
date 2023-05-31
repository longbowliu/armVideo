
从topic提取图片 (提取的帧数不够，下面另一种方法没问题)

rosrun image_view extract_images image:=/v4l2_camera/camera2/image  _image_transport:=compressed
rosrun image_view extract_images image:=/v4l2_camera/camera2/image  _image_transport:=compressed _sec_per_frame:=0.033

https://zhuanlan.zhihu.com/p/406079846




https://www.coder.work/article/7823657
另一种方法是解压缩数据然后保存。（**********）
1, roscore
2, rosbag play  xxx.bag  (after 3,4)
3, rosrun image_transport republish compressed in:=/v4l2_camera/camera2/image raw out:=image/raw   (如果有问题可以手动输入)
4, rosrun image_view image_saver image:=image/raw _save_all_image:=all _filename_format:=%04d.%s
5, ffmpeg -r 30 -f image2 -i %4d.jpg test.mp4
6, copy video file into  /home/demo/data/video_record/video0 , create dict.info to map id and video file
7, run webrtc-streamer
8, run v4l2rtspserver 


$ rosbag play <bagfile>



https://www.cnblogs.com/y-z-h/p/15918902.html

