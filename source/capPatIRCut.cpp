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



int main(int argc, char **argv){

	int m = atoi(argv[3]);
	int x = atoi(argv[4])*1000;
	int n = atoi(argv[5]);
	int y = atoi(argv[6])*1000;

	if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }
  
    memset(&serv_addr, '0', sizeof(serv_addr));
  

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(DEFAULT_PORT);
      
    // Convert IPv4 and IPv6 addresses from text to binary form
    if(inet_pton(AF_INET, argv[2], &serv_addr.sin_addr)<=0) 
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }
	

	char rtspStream[100];
	namedWindow("Show Video",WINDOW_NORMAL);
	sprintf(rtspStream,"rtsp://admin:1111@%s:554",argv[1]);
	
	VideoCapture vid(rtspStream);
	if(!vid.isOpened()){
		cout << "Unable stream\n";
		return -1;
	}
	if (!fs::is_directory("CapIR") || !fs::exists("CapIR")){ 
    	fs::create_directory("CapIR");
	}
	flushVideo(vid);
	Mat img;
	int count = 0;
	char imgSave[100];
	while(true){
		vid >> img;
		if (count % 10 == 9){
			usleep(x);
			onPattern();
		}
		if (count %10 == (9-(m-n))){
			usleep(y);
			setIRCut(true);
		}

		if (count % 10 == 5){
			offPattern();
			setIRCut(false);
		}
		sprintf(imgSave,"CapIR/%06d.jpg",count);
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