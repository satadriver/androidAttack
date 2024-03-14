
#include "alicdn.h"

#include "youku.h"
#include "public.h"
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
#include <iostream>
#include <string>
#include <string.h>
#include <ctype.h>
#include <errno.h>



 char* AliCdn::getPath(const char * szsearchdir){

	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonNewsPath","open dir error");
		printf("GetPlayerCoreNeonNewsPath open dir error\r\n");
		perror("reason:");
		return 0;
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
				closedir(dir);
				return ptr->d_name;
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonNewsPath","not find so path");
	printf("not find so path\r\n");
	return 0;
 }


#define BIND_SO_WITH_APK

int AliCdn::alicdnProc(char * workpath,string cmdline){

#ifdef BIND_SO_WITH_APK
	int ret = 0;
	string newpath = "/data/data/" + cmdline + "/AliCdnEntry/";
	string apkfilename = newpath + JAR_FILE_NAME;
	string cfgfn = newpath + CONFIG_FILE_NAME;
    if(access(apkfilename.c_str(),F_OK) != 0|| access(cfgfn.c_str(),F_OK) != 0  ){
    	char sopath[1024] = {0};
    	char soname[1024] = {0};
    	ret = getSoPath((char*)"/proc/self/maps",sopath,soname,(char*)"libinitHelper.so");
    	if(ret < 0){
    		return -1;
    	}else{
    		ret = splitFromSo(sopath,(char*)newpath.c_str(),(char*)soname);
    		if(ret < 0){
    			return -1;
    		}
    	}
    }

	strcpy(workpath,newpath.c_str());
	return 0;
#else

	string path = "/data/data/" + cmdline + "/app_ucmsdk/ucplayer/";

	char* subdir = getPath(path.c_str());
	char* subsubdir = 0;
	if(subdir > 0){
		path = path + subdir + "/";
		subsubdir = getPath(path.c_str());
		if(subsubdir > 0){
			path = path + subsubdir + "/";
		}else{
			return -1;
		}
	}else{
		return -1;
	}

	path = path + "assets/";
	strcpy(workpath,path.c_str());
	return 0;
#endif
}
