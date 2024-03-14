#include <jni.h>
#include <stdio.h>
#include <string.h>
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

#include <string>
#include <string.h>
#include <ctype.h>
#include <iostream>



#include "public.h"
#include "neteasenews.h"



using namespace std;


string NeteaseNews::findPath(string szsearchdir)
{
	string ret = "";
	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir.c_str())) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"NeteaseNews","open dir error");
		printf("NeteaseNews open dir error\r\n");
		perror("reason:");
		return ret;
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
				if(*ptr->d_name == 'v'){

					string next = string("LivePlayer_Android_SDK_") + ptr->d_name + "_armeabi/" ;
					string ret = szsearchdir + ptr->d_name + "/" + next;
					closedir(dir);
					return ret;

				}
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"NeteaseNews","not find so path");
	printf("not find so path\r\n");
	return ret;
}

/*
GET /package/LivePlayer_Android_SDK_v1.8.0_armeabi.zip HTTP/1.1
User-Agent: Dalvik/2.1.0 (Linux; U; Android 5.1; HUAWEI TAG-TL00 Build/HUAWEITAG-TL00)
Host: yx-web.nos.netease.com
Connection: Keep-Alive
Accept-Encoding: gzip

HTTP/1.1 200 OK
Content-Type: application/zip
Connection: keep-alive
Content-Length: 854322
*/


int NeteaseNews::neteasenewsProcess(char * workpath,const char * szCmdline){
	int ret = 0;
	int result = 0;

	string strfind = "/data/data/" + string(szCmdline) + "/files/libs/";
	string path = findPath(strfind);
	if(path == ""){
		return -1;
	}

	strcpy(workpath,path.c_str());
	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libneliveplayer.so");
		if(ret < 0){
			ret = splitFromSo(workpath,workpath,(char*)"libnelprender.so");
			if(ret < 0){
				ret = splitFromSo(workpath,workpath,(char*)"libnelpengine.so");
			}
		}
	}

	return ret;
}
