#include <experimental/filesystem>
#include <iostream>
#include "config.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <pthread.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
using namespace std;   
using namespace cv;
namespace fs = std::experimental::filesystem;


int main(int argc, char **argv){	

	char rtspStream[100];
	namedWindow("Show Video",WINDOW_NORMAL);
	sprintf(rtspStream,"rtsp://admin:1111@%s:554",argv[1]);
	int offset = atoi(argv[3])*1000;
	VideoCapture vid(rtspStream);
	if(!vid.isOpened()){
		cout << "Unable stream\n";
		return -1;
	}
	if (!fs::is_directory("CapIRCut") || !fs::exists("CapIRCut")){ 
    	fs::create_directory("CapIRCut");
	}
	flushVideo(vid);
	Mat img;
	int count = 0;
	char imgSave[100];
	while(true){
		vid >> img;
		if (count % 10 == 9){
			usleep(offset);
			setIRCut(true);
		}

		if (count % 10 == 5){
			setIRCut(false);
		}
		sprintf(imgSave,"CapIRCut/%06d.jpg",count);
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