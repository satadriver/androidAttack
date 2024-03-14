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
#include "iqiyi.h"



using namespace std;






int IqiyiVideo::iqiyiProcess(char * workpath,const char * szCmdline){
	int ret = 0;
	int result = 0;

	string path = "/data/data/" + string(szCmdline) + "/files/app/download/sv/so/";
	strcpy(workpath,path.c_str());
	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libvince++.so");
		if(ret < 0){
			ret = splitFromSo(workpath,workpath,(char*)"libvinceopengl.so");
			if(ret < 0){
				ret = splitFromSo(workpath,workpath,(char*)"libmediaedit.so");
				if(ret < 0){
					ret = splitFromSo(workpath,workpath,(char*)"libvinceavformat60.so");
					if(ret < 0){
						ret = splitFromSo(workpath,workpath,(char*)"libvincecore.so");
						if(ret < 0){
							ret = splitFromSo(workpath,workpath,(char*)"libvincenormalize.so");
							if(ret < 0){
								ret = splitFromSo(workpath,workpath,(char*)"libvincegtk2.so");
							}
						}
					}
				}
			}
		}
	}

	return ret;
}
