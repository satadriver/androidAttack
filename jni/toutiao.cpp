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




using namespace std;

int toutiaoProcess(char * workpath,const char * szCmdline){
	int ret = 0;
	int result = 0;

	char soname[1024] = {0};
	char sopath[1024];

	strcpy(workpath,"/data/data/");
	strcat(workpath,szCmdline);
	strcat(workpath,"/files/ss_plugins/com.bytedance.common.plugin.wschannel/lib/");

	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libPushManager.so");
		if(ret < 0){
			strcpy(workpath,"/data/data/");
			strcat(workpath,szCmdline);
			strcat(workpath,"/files/ss_plugins/com.bytedance.common.plugin.cronet/lib/");
			ret = splitFromSo(workpath,workpath,(char*)"libsscronet.so");
			if(ret < 0){
				return ret;
			}else{
				//result = getWeixinSoPath((char*)"/proc/self/maps",sopath,soname,(char*)"libsscronet.so");
			}
		}else{
			//result = getWeixinSoPath((char*)"/proc/self/maps",sopath,soname,(char*)"libPushManager.so");
			if(result < 0){

			}
		}
	}

//	std::string rootsopname = string(sopath ) + string(soname);
//
//	string oldsoname = string(workpath) + OLDSO_FILE_NAME;
//
//	soinfo gKeepSoInfo;
//	result = replaceSoInfo(oldsoname.c_str(),rootsopname.c_str(),&gKeepSoInfo);
//	if(result == 0){
//		sprintf(szout,"replace soinfo:%s with soinfo:%s ok\r\n",rootsopname.c_str(),oldsoname.c_str());
//		writeLogFile(szout,szCmdline);
//	}else{
//		sprintf(szout,"replace soinfo:%s with soinfo:%s error\r\n",rootsopname.c_str(),oldsoname.c_str());
//		writeLogFile(szout,szCmdline);
//	}

	return ret;
}
