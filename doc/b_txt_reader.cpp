    #include <iostream>
    #include <fstream>
    using namespace std;
    struct record_info_struct
	{
		timeval tm;
		unsigned int  size;
        // unsigned int diff;
	};
    int main()
    {
        record_info_struct s;       
        ifstream inFile("test.info",ios::in|ios::binary); //二进制读方式打开
        if(!inFile) {
            cout << "error" <<endl;
            return 0;
        }
        while(inFile.read((char *)&s, sizeof(s))) { //一直读到文件结束
            // int readedBytes = inFile.gcount(); //看刚才读了多少字节
            cout << s.tm.tv_sec << " "<<s.tm.tv_usec<<" " << s.size<< endl;   
        }
        inFile.close();
        return 0;
    }
