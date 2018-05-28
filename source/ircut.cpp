#include <iostream>
#include "config.hpp"
#include <string>
int main(){
	char onvifCtrl[50];
	sprintf(onvifCtrl,"%s:2000",argv[1]);
	IPConfig iPConfig(onvifCtrl);
	if (strcmp(argv[2],"on") == 0){
		std::cout << "Turn on IRcut filter\n";
		iPConfig.setIRcut(true);
	}
	if (strcmp(argv[2],"off") == 0){
		std::cout << "Turn off IRcut filter\n";
		iPConfig.setIRcut(false);
	}
}