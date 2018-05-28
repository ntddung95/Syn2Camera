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

#define DEFAULT_PORT 5000

VideoCapture vid1,vid2,vid3;
Mat img1, img2,img3;
bool done1,done2,done3,start;

struct sockaddr_in address;
int sock = 0, valread;
struct sockaddr_in serv_addr;

void onPattern(){
	char buffer[10];
	sock = socket(AF_INET, SOCK_STREAM, 0);	
	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	send(sock , "4\0" , 1 , 0 );
	shutdown(sock, SHUT_WR);
	//printf("Message sent to %s: %s\n",ip, message);
	valread = read(sock , buffer, 1024);

	if(strlen(buffer) <= 0){
		printf("\nxxx\n");
	return;
	}

	printf("XXX: %s\n",buffer);
	close(sock);
}

void offPatterIR(){
	char buffer[10];
	sock = socket(AF_INET, SOCK_STREAM, 0);	
	connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
	send(sock , "5\0" , 1 , 0 );
	shutdown(sock, SHUT_WR);
	// printf("Message sent to %s: %s\n",ip, message);
	valread = read(sock , buffer, 1024);

	if(strlen(buffer) <= 0){
		printf("\nxxx\n");
	return;
	}

	printf("XXX: %s\n",buffer);
	close(sock);
}

void cam1(){
	
	vid1.open("rtsp://admin:1111@10.11.11.140:554");
	if(vid1.isOpened())
		cout<<"Cam1: OK\n";
	else
		cout<<"Cam1: Fail\n";
	while(true){
		vid1 >> img1;
		done1 = true;
		if(!start)
			break;
	}
	cout<<"Finish thread\n";
}

void cam2(){
	
	vid2.open("rtsp://admin:1111@10.11.11.164:554");
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




int main(int argc, char **argv){
	int gop = 10;
	start = true;
	IPConfig ipConf("10.11.11.140:2000");
	done1 = false;
	done2 = false;
	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, "10.11.11.74", &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
	
	//ipConf.setGOP(gop);
	
	thread cam1Thr1(cam1);
	// thread cam1Thr2(cam2);


	namedWindow("XXX",WINDOW_NORMAL);

	int count = 0;
	
	Mat imgX1,imgX2,imgMain1,imgMain2,imgMain3;
	while (true){
		/*if ((img1 == NULL) || (img2 == NULL)){
			usleep(200000);
			cout<<"isEmpty\n";
			continue;
		}*/
		if(done1){
			time_t start = clock();
			imgMain1 = img1.clone();
		//	imgMain2 = img2.clone();
		//	hconcat(imgMain1, imgMain2, imgX1);
			cout<<"count: "<<count<<endl;
			imshow("XXX",imgMain1);
			// char buff[50];
			// sprintf(buff,"Image/%05d.jpg",count);
			// imwrite(buff,imgMain1);
			/*if(count == 5){
				cout<<"setGOP!\n";
				ipConf.setGOP(60);
				
			}
			

			if(count == 5){
				usleep(100000);
				ipConf.setGOP(70);
				cout<<"On Blink!\n";
				onPattern();
			}
*/
			count++;
			int t = (int)(clock() - start)/1000;
			cout << t << endl;
			if (waitKey(1000 - t) == 27){
				start = false;
				break;
			}
		}
		else
			usleep(100000);
	}
	cam1Thr1.join();
	offPatterIR();
	// cam1Thr2.join();

	return 0;
}