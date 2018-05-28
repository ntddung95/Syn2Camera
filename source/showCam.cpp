#include <iostream>
#include <opencv2/opencv.hpp>
#include <experimental/filesystem>
#include <string.h>
#include <string>

using namespace std;   
using namespace cv;
namespace fs = std::experimental::filesystem;


int main(int argc, char **argv){
	char rtspStream[100];
	namedWindow("Show Video",WINDOW_NORMAL);
	sprintf(rtspStream,"rtsp://admin:1111@%s:554",argv[1]);
	VideoCapture vid(rtspStream);
	if(!vid.isOpened()){
		cout << "Unable stream\n";
		return -1;
	}
	if ((!fs::is_directory("Image") || !fs::exists("Image")) && strcmp(argv[2],"1") == 0){ 
    	fs::create_directory("Image");
	}
	flushVideo(vid);
	Mat img;
	int count = 0;
	char imgSave[100];
	while(true){
		vid >> img;
		sprintf(imgSave,"Image/%06d.jpg",count);
		imshow("Show Video",img);
		int key = waitKey(1);
		imwrite(imgSave,img);
		cout << "Count: " << count << endl;
		count++;
		if(key == 27){
			cout << "Exit!\n";
			break;
		}
	}
	vid.release();
}