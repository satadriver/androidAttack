
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
#include <unistd.h>

#include "public.h"




using namespace std;

int xunlei_initProcess(char * workpath,const char * szCmdline){

	int ret = 0;
	char szout[1024];

	string filepath = string("/data/data/") + szCmdline + "/files/libso/";
	strcpy(workpath,filepath.c_str());

	string soname = "libwgs2gcj.so";

	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)soname.c_str());
		if(ret < 0){
			return -1;
		}
	}

	return ret;

}
