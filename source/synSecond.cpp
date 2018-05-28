#include <iostream>
#include <opencv2/opencv.hpp>
#include "config.hpp"

using namespace std;
using namespace cv;

int calculate(int soLeGiay){
	return((int)(100-soLeGiay)/9);
}

int main(int argc, char **argv){
	char rtspStream[100],onvifCtrl[50];
	sprintf(rtspStream,"rtsp://admin:1111@%s:554",argv[1]);
	VideoCapture vid(rtspStream);
	sprintf(onvifCtrl,"%s:2000",argv[1]);
	IPConfig IPConfig
	if(!vid.isOpened()){
		cout << "Unable stream\n";
		return -1;
	}
	flushVideo(vid);
	Mat img;
	int count = 0;
	int changeGOP = calculate(atoi(argv[2]));
	while(changeGOP > 0){
		vid >> img;
		if( count >= 1 ){
			if(soLan == 1){
				if (soLan1 == 1)
					ipConf.setGOP(60);
				else	
					ipConf.setGOP(50);
			}
			else
				ipConf.setGOP(count *3);
			cout<<"SetGOP: " << count*3  << endl;
			soLan--;
		}

		count++;
	}
}