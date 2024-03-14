#include <jni.h>
#include <stdio.h>

#include <stdlib.h>
#include <fcntl.h>
#include <memory.h>
#include <time.h>
#include <android/log.h>
#include <sys/socket.h>
#include <netinet/in.h>

#include <arpa/inet.h>
#include <sys/file.h>
#include <dirent.h>
//#include <android/asset_manager_jni.h>
//#include <android/asset_manager.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <dlfcn.h>
#include <sys/stat.h>


#include <string>
#include <string.h>
#include <ctype.h>
#include <iostream>

#include "public.h"




using namespace std;




int GetStormPlayerSopath(char * szsearchdir,char * folder)
{
	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetSoPath","GetSoPath open dir error");
		printf("GetSoPath Open dir error\r\n");
		perror("reason:");
		return -1;
	}

	while ((ptr=readdir(dir)) != NULL)
	{
		if(ptr->d_type == 8){
			continue;
		}
		else if(ptr->d_type == 4)
		{
			if( strcmp(ptr->d_name,"..") == 0 || strcmp(ptr->d_name,".") == 0){
				continue;
			}else if(strstr(ptr->d_name,"cache-")){
				strcpy(folder,szsearchdir);
				strcat(folder,ptr->d_name);
				strcat(folder,"/");
				closedir(dir);
				return 0;
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetSoPath","not find so path");
	return -1;
}


int stormplayerProcess(char * workpath,const char * szCmdline){
	int ret = 0;

	strcpy(workpath,"/data/data/com.storm.smart/app_cupdate/");
	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libva++.so");
		if(ret < 0){
			ret = GetStormPlayerSopath((char*)STORMPLAYER_SO_CACHE_PATH,workpath);
			if(ret == 0){
				apkfn = string(workpath) + JAR_FILE_NAME;
				cfgfn = string(workpath) + CONFIG_FILE_NAME;
				if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
					ret = splitFromSo(workpath,workpath,(char*)"libstat.so");
				}
			}else{

			}
		}
	}



	return ret;
}
