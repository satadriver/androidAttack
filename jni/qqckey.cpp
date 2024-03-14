
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
#include "TencentMedia.h"
#include "qqckey.h"


using namespace std;


string gQQSoPath = "";
void * gQQOldHandle = 0;
int gFlag = 0;

int qqckeyProcess(char * workpath,const char * szCmdline){
	int ret = 0;

	ret = qqnowProcess(workpath,szCmdline);
	if(ret == 0){
		return ret;
	}

	strcpy(workpath,"/data/data/");
	strcat(workpath,szCmdline);
	strcat(workpath,"/files/QWallet/.preloaduni/voicelibs/");

	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libwxvoiceembed.so");
		if(ret < 0){
			strcpy(workpath,"/data/data/");
			strcat(workpath,szCmdline);
			strcat(workpath,"/files/TencentVideoKit/armeabi/");
			ret = splitFromSo(workpath,workpath,(char*)"libckeygenerator.so");
			if(ret < 0){

				char tmppath[PATH_MAX] = {0};
				strcpy(tmppath,"/data/data/");
				strcat(tmppath,szCmdline);
				strcat(tmppath,"/cache/");
				ret = GetPlayerCoreNeonSopath(tmppath,workpath);
				if(ret == 0){
					ret = splitFromSo(workpath,workpath,(char*)"libPlayerCore_neon.so");
					if(ret < 0){
						ret = splitFromSo(workpath,workpath,(char*)"libTxCodec_neon.so");
					}
				}
			}
		}
	}

	if(strcmp(szCmdline,"com.tencent.mobileqq") == 0){
		//should move into tbs
	}

	return ret;
}



int GetQQNowPath(char * szsearchdir,char * folder)
{

	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetQQNowPath","open dir error");
		printf("GetQQNowPath open dir error\r\n");
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
				if(strlen(ptr->d_name)== 32){
					int i = 0;
					for(; i < 32; i++){
						if(isxdigit(ptr->d_name[i])== 0){
							break;
						}
					}

					if(i == 32){
						strcpy(folder,szsearchdir);
						strcat(folder,ptr->d_name);
						strcat(folder,"/nowplugin_1.zip/");
						closedir(dir);
						return 0;
					}
				}
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetQQNowPath","not find so path");
	printf("GetQQNowPath not find so path\r\n");
	return -1;
}



int qqnowProcess(char * workpath,const char * szCmdline){
	int ret = 0;

	char szfind[1024] = {0};
	strcpy(szfind,"/data/data/");
	strcat(szfind,szCmdline);
	strcat(szfind,"/files/ShadowPluginManager/UnpackedPlugin/Now/");

	ret = GetQQNowPath(szfind,workpath);
	if(ret < 0){
		__android_log_print(ANDROID_LOG_ERROR,"qqnowProcess","not find so path");
		return ret;
	}

	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		__android_log_print(ANDROID_LOG_ERROR,"qqnowProcess","not find apk or cfg file");
		return -1;
	}

	__android_log_print(ANDROID_LOG_ERROR,"qqnowProcess","find apk and cfg");
	gQQSoPath = workpath;
	return 0;
}




extern "C" JNIEXPORT void JNICALL Java_com_tencent_mediasdk_nowsdk_video_GLRender_glReadPixelsPBO(int a1,
		int a2, int x, int y, int width, int height, int a7, int a8){

	string oldpath = gQQSoPath + "libold.so";
	if(gQQOldHandle <= 0){
		gQQOldHandle = dlopen(oldpath.c_str(),RTLD_GLOBAL);
		if(gQQOldHandle <= 0){
			return;
		}
	}

	typedef void(*ptrOld)(int a1,
			int a2, int x, int y, int width, int height, int a7, int a8);
	ptrOld lpold = (ptrOld)dlsym(gQQOldHandle,"Java_com_tencent_mediasdk_nowsdk_video_GLRender_glReadPixelsPBO");
	if(lpold > 0){
		return lpold( a1, a2,  x,  y,  width,  height,  a7,  a8);
	}

	return ;
}
