#include <iostream>
#include "config.h"
#include <opencv2/opencv.hpp>
#include <thread>
#include <pthread.h>
#include <unistd.h>
using namespace std;   
using namespace cv;

VideoCapture vid1,vid2,vid3;
Mat img1, img2,img3;
bool done1,done2,done3,start = true;
IPConfig *ipConf1,*ipConf2;

void cam1(){
	
	vid1.open("rtsp://admin:1111@10.11.11.140:554");
	if(vid1.isOpened())
		cout<<"Cam1: OK\n";
	else
		cout<<"Cam1: Fail\n";
	while(start){
		vid1 >> img1;
		done1 = true;
	}
}

void cam2(){
	
	vid2.open("rtsp://admin:1111@10.11.11.154:554");
	if(vid2.isOpened())
		cout<<"Cam2: OK\n";
	else
		cout<<"Cam2: Fail\n";
	while(start){
		vid2 >> img2;
		done2 = true;
	}
}

void cam3(){
	
	vid3.open("rtsp://admin:1111@10.11.11.51:554");
	if(vid3.isOpened())
		cout<<"Cam3: OK\n";
	else
		cout<<"Cam3: Fail\n";
	while(start){
		vid3 >> img3;
		done3 = true;
	}
}

int calculate(int soLeGiay){
	return((int)(100-soLeGiay)/9);
}


int main(int argc, char **argv){
	int gop = 10;
	done1 = false;
	done2 = false;

	IPConfig ipConf("10.11.11.140:2000");
	//ipConf.setGOP(gop);
	
	thread cam1Thr1(cam1);
	// thread cam1Thr2(cam2);


	namedWindow("XXX",WINDOW_NORMAL);

	int count = 0;
	
	Mat imgX1,imgX2,imgMain1,imgMain2,imgMain3;
	usleep(1000000);

	//int soLeGiay = atoi(argv[1]);

	int soLan1 = atoi(argv[1]);//calculate(soLeGiay);
	int soLan = soLan1;
	cout << "So Lan can thay doi: " << soLan << endl;

	while (true){
		/*if ((img1 == NULL) || (img2 == NULL)){
			usleep(200000);
			cout<<"isEmpty\n";
			continue;
		}*/
		if(done1){
			imgMain1 = img1.clone();
			// imgMain2 = img2.clone();
			// hconcat(imgMain1, imgMain2, imgX1);
			cout<<"count: "<<count<<endl;
			imshow("XXX",imgMain1);
			/*char buff[50];
			sprintf(buff,"Image/%05d.jpg",count);
			imwrite(buff,imgX1);*/
			if (soLan == 0){
				start = false;
				break;
			}
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
			
			if (waitKey(900) == 27){
				start = false;
				break;
			}
		}
		else
			usleep(100000);
	}
	cam1Thr1.join();
	cout<<"Finish!\n";
	// cam1Thr2.join();

	return 0;
}