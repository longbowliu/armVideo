#include<opencv2/opencv.hpp>
#include<iostream>
#include<unistd.h>
 
using namespace std;
using namespace cv;
 
int main()
{
	
	VideoWriter video("test.avi", CV_FOURCC('X', 'V', 'I', 'D'), 30, Size(480, 320));
 
	String img_path = "/home/demo/catkin_ws/src/images/";
	vector<String> img;
 
	glob(img_path, img, false);
 
	size_t count = img.size();
    cout <<"*****************"<<count << endl;
    usleep(2000000);
	for (size_t i = 0; i < count; i++)
	{
        string tmp = "000";
        if(i>=10 && i <100 ){
             tmp = "00";
        }
        if(i>=100 && i <1000 ){
             tmp = "0";
        }
          if(i>=1000  ){
             tmp = "";
        }
		stringstream str;
        // string s;
		str << i << ".jpg";
        // str >>s;
        string img_name = img_path + "frame"+tmp+str.str();
        cout << img_name << endl;
		Mat image = imread(img_name);
        // usleep(100000);
		if (!image.empty())
		{
			resize(image, image, Size(480, 320));
			video << image;
			// cout << "正在处理第" << i << "帧" << endl;
		}
	}
	cout << "处理完毕！" << endl;
}
 