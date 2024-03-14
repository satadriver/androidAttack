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
#include <android/asset_manager_jni.h>
#include <android/asset_manager.h>
#include <unistd.h>
#include <sys/inotify.h>
#include <dlfcn.h>
#include <sys/stat.h>
#include <elf.h>

//#include <string>
#include <string.h>
#include <ctype.h>
#include <iostream>

#include "public.h"
#include "douyu.h"




using namespace std;

int getdouyuSoname(char * szsearchdir,char * folder)
{

	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"getdouyuSoname","open dir error");
		printf("getdouyuSoname open dir error\r\n");
		perror("reason:");
		return -1;
	}

	while ((ptr=readdir(dir)) != NULL)
	{
		if(ptr->d_type == 4){
			continue;
		}
		else if(ptr->d_type == 8)
		{
			if(strstr(ptr->d_name,"libp2pmodule") && strstr(ptr->d_name,".so") ){
				//strcpy(folder,szsearchdir);
				//strcat(folder,ptr->d_name);
				strcpy(folder,ptr->d_name);
				closedir(dir);
				return 0;
			}

		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"getdouyuSoname","not find so path");
	printf("not find so path\r\n");
	return -1;
}


int getdouyuSopath(char * szsearchdir,char * folder)
{

	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"getdouyuSopath","open dir error");
		printf("getdouyuSopath open dir error\r\n");
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
			}
			else {
				strcpy(folder,szsearchdir);
				strcat(folder,ptr->d_name);
				strcat(folder,"/v2/http/armeabi-v7a/");
				closedir(dir);
				return 0;
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"getdouyuSopath","not find so path");
	printf("not find so path\r\n");
	return -1;
}





int Douyu::douyuProcess(char * workpath,const char * szCmdline){
	int ret = 0;
	char szout[1024];

	ret = getdouyuSopath((char*)"/data/data/air.tv.douyu.android/files/vlib/",workpath);
	if(ret < 0){
		return -1;
	}

	char soname[1024] = {0};
	ret = getdouyuSoname(workpath,soname);
	if(ret < 0){
		return -1;
	}


	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,soname);
		if(ret < 0){
			return -1;
		}
	}

	return ret;
}
