#include <iostream>
#include "config.h"
#include <opencv2/opencv.hpp>
#include <string.h>
#include <unistd.h>
using namespace std;
using namespace cv;


int main(int argc, char **argv){
	IPConfig ipConf1(argv[1]);
	int gop = atoi(argv[2]);
	ipConf1.setGOP(gop);
	cout<<"Set GOP: " << gop << endl;
	return 0;
}