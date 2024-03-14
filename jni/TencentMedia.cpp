
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


#include <string.h>
#include <ctype.h>
#include <iostream>
#include <string>
#include "public.h"




using namespace std;

//mnt/sdcard是实际的文件所在的地方，/sdcard是mnt/sdcard的一个软链接（相当于windows的文件夹的快捷方式），
//链接到/mnt/sdcard 目录.在android系统它们必须这样运作。
//在Linux文件系统中，有一种特别的文件叫“软链接”，类似于Windows下的快捷方式，软链接可以把一个文件或者文件夹映射到别的地方
//与内部存储不同的是当应用程序卸载时，它在外部存储所创建的文件数据不会被清除
//当app被卸载后，sdCard/Android/data/PackageName/下的所有文件都会被删除
///data/user/0/

int GetPlayerCoreNeonSopath(char * szsearchdir,char * folder)
{
	DIR *dir = 0;
	struct dirent *ptr = 0;

	if ((dir=opendir(szsearchdir)) == NULL)
	{
		__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonPath","GetPlayerCoreNeonPath open dir error");
		printf("GetPlayerCoreNeonPath Open dir error\r\n");
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
			}else if(strstr(ptr->d_name,"player_core_neon_V")){
				strcpy(folder,szsearchdir);
				strcat(folder,ptr->d_name);
				strcat(folder,"/");
				closedir(dir);
				return 0;
			}
		}
	}
	closedir(dir);
	__android_log_print(ANDROID_LOG_ERROR,"GetPlayerCoreNeonPath","not find so path");
	printf("not find so path\r\n");
	return -1;
}

int tencentMidiaProcess(char * workpath,const char * szCmdline){
	int ret = 0;
	char tmppath[PATH_MAX] = {0};
    strcpy(tmppath,"/data/data/");
    strcat(tmppath,szCmdline);
    strcat(tmppath,"/cache/");
    ret = GetPlayerCoreNeonSopath(tmppath,workpath);
    if(ret < 0){
    	return ret;
    }

	string apkfn = string(workpath) + JAR_FILE_NAME;
	string cfgfn = string(workpath) + CONFIG_FILE_NAME;
	if( access(apkfn.c_str(),F_OK) || access(cfgfn.c_str(),F_OK) ){
		ret = splitFromSo(workpath,workpath,(char*)"libPlayerCore_neon.so");
		if(ret < 0){
			ret = splitFromSo(workpath,workpath,(char*)"libTxCodec_neon.so");
		}
	}

    return ret;
}
