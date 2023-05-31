locate libopencv*.so
pkg-config --modversion opencv
g++ cvtest.cpp -o cvtest2  `pkg-config --cflags --libs opencv`



##### 详解pkg-config --cflags --libs glib-2.0的作用

https://blog.csdn.net/jllongbell/article/details/88422054

##### Ubuntu18下OpenCV的安装及使用示例

https://blog.csdn.net/m0_59416558/article/details/120710454



##### opencv 编码解码

https://blog.csdn.net/liang_baikai/article/details/101055428
